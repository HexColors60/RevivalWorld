/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lotto_machine.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-21
 * Note   : 樂透彩簽注機
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#define DATA_PATH		"/data/object/lotto.o"

#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <secure.h>

#define MAX_LOTTO_AMOUNT                10000
#define MAX_SINGLE_TIME_LOTTO		10000

#define MAX_LOTTO_OPEN_PER_ROUND	2500
#define MAX_LOTTO_PER_ROUND		2500

#define MONEY_PER_LOTTO			500
#define RECYCLE_PERCENT			80	// RECYCLE_PERCENT% 作為獎金, 剩下的錢系統回收

#define FIFTH_REWARD			2000	// 第 5 獎獎金
inherit STANDARD_OBJECT;

private nosave mapping table;
private mapping lotto_info;
private nosave mapping add_number_schedule;

void do_lotto(object me, string arg);

nosave string help = @HELP
    每注 500 元，每十分鐘自動開獎一次，開獎後所有剩餘獎金無限期數
累積至下期頭獎，祝您幸運中大獎！

lotto				列出你目前的投注資料
lotto 1,2,3,4,5,6		共六組數字 1 到 42, 請依照逗點格式輸入
lotto N				自動電腦選號 N 注
lotto info 			顯示目前各家下注資料

lotto open			立即開獎(巫師權限)

頭獎：總彩金 38% + 上期剩餘之所有獎金  (六號全中)
二獎：總彩金 12%                       (六號全中其中一號為特別號)
三獎：總彩金 15%                       (五星)
四獎：總彩金 35%                       (四星)
五獎：一律 2000 元, 此 2000 元額外發放 (三星)

獎金金額依照中獎人數平均分配

機體版本：v2.4
HELP;

nosave mapping actions = 	
([
    "lotto"			:(: do_lotto :),
]);

// 隨意挑選 6 組數字
int *random_number()
{
	int *num = allocate(6), i, j, k;

	reset_eval_cost();

	for(j=i=0;j<6;j=++i)
	{
		k = random(42)+1;
		while( j-- && k!=num[j] );

		if( j != -1 )
		{
			i--; continue;
		}
		else num[i] = k;
	}
	return num;
}

void lotto_open_finish(int *open_num, int open_spec_num, mapping prize, int *total_prize, int total_lotto)
{
	string id;
	mapping map;
	string msg, *money = allocate(5);
	object ppl;

	// 頭獎彩金為本期彩金 38% + 上期剩餘所有彩金

	money[0] = count(count(count(lotto_info["total_prize"],"*",38),"/",100), "+", lotto_info["first_prize"]);
	money[1] = count(count(lotto_info["total_prize"],"*",12),"/",100);
	money[2] = count(count(lotto_info["total_prize"],"*",15),"/",100);
	money[3] = count(count(lotto_info["total_prize"],"*",35),"/",100);
	money[4] = FIFTH_REWARD+"";

	msg =  HIY"\n第 "+lotto_info["time"]+" 期樂透彩的七顆彩球已經全部開出了！！\n"NOR;
	msg += HIW"開獎號碼為："HIC+open_num[0]+","+open_num[1]+","+open_num[2]+","+open_num[3]+","+open_num[4]+","+open_num[5]+HIW" 特別號為："HIC+open_spec_num+"\n"NOR;

	msg += HIG"────────────────────────\n"NOR;

	msg += "總共投注："	+sprintf("%18s",NUMBER_D->number_symbol(total_lotto))+" 注\n";
	msg += "總彩金為："	+sprintf("%18s",NUMBER_D->number_symbol(count(lotto_info["total_prize"],"+",lotto_info["first_prize"])))+" 元\n";
	msg += "頭彩彩金："	+HIR+sprintf("%18s",NUMBER_D->number_symbol(money[0]))+" 元"HIC"(上期累積 "+NUMBER_D->number_symbol(lotto_info["first_prize"])+" 元)\n"NOR;
	msg += "二獎彩金："	+HIG+sprintf("%18s",NUMBER_D->number_symbol(money[1]))+" 元\n"NOR;
	msg += "三獎彩金："	+HIG+sprintf("%18s",NUMBER_D->number_symbol(money[2]))+" 元\n"NOR;
	msg += "四獎彩金："	+HIG+sprintf("%18s",NUMBER_D->number_symbol(money[3]))+" 元\n"NOR;
	msg += "五獎彩金："	+HIG+sprintf("%18s",money[4])+" 元\n"NOR;

	msg += HIG"────────────────────────\n"NOR;

	msg += "本期樂透彩統計中獎人數如下：\n";
	msg += "頭獎： "HIM+sprintf("%6d",total_prize[0])+NOR"人，每注可得 "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[0],"/",total_prize[0]||1)))+NOR" 元\n";
	msg += "二獎： "HIM+sprintf("%6d",total_prize[1])+NOR"人，每注可得 "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[1],"/",total_prize[1]||1)))+NOR" 元\n";
	msg += "三獎： "HIM+sprintf("%6d",total_prize[2])+NOR"人，每注可得 "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[2],"/",total_prize[2]||1)))+NOR" 元\n";
	msg += "四獎： "HIM+sprintf("%6d",total_prize[3])+NOR"人，每注可得 "HIG+sprintf("%18s",NUMBER_D->number_symbol(count(money[3],"/",total_prize[3]||1)))+NOR" 元\n";
	msg += "五獎： "HIM+sprintf("%6d",total_prize[4])+NOR"人，每注可得 "HIG+sprintf("%18s",FIFTH_REWARD+"")+NOR" 元\n";

	msg += HIG"────────────────────────\n"NOR;

	lotto_info["total_prize"] = total_prize[0] ? count(count(lotto_info["total_prize"],"*",62),"/",100) : lotto_info["total_prize"];
	lotto_info["total_prize"] = count(lotto_info["total_prize"], "-", total_prize[1]?money[1]:"0");
	lotto_info["total_prize"] = count(lotto_info["total_prize"], "-", total_prize[2]?money[2]:"0");
	lotto_info["total_prize"] = count(lotto_info["total_prize"], "-", total_prize[3]?money[3]:"0");

	lotto_info["first_prize"] = total_prize[0] ? lotto_info["total_prize"] : count(lotto_info["total_prize"],"+",lotto_info["first_prize"]);

	if( count(lotto_info["first_prize"],"<",0) )
		lotto_info["first_prize"] = "0";

	msg += "本期剩餘獎金："+HIC+NUMBER_D->number_symbol(lotto_info["first_prize"])+" 元"NOR"，將累積至下期頭獎獎金。\n";

	msg(msg, this_object(),0,1);

	reset_eval_cost();

	foreach(id, map in prize)
	{
		string ppl_money;

		if( !sizeof(map) ) continue;

		if( objectp(ppl=find_player(id)) )
		{
			tell(ppl,"\n第 "+lotto_info["time"]+" 期樂透彩開獎已經完成，"+pnoun(2, ppl)+"的中獎資訊如下：\n"HIY+
			    (map[0]?"頭獎 "+map[0]+" 注 ":"")+
			    (map[1]?"二獎 "+map[1]+" 注 ":"")+
			    (map[2]?"三獎 "+map[2]+" 注 ":"")+
			    (map[3]?"四獎 "+map[3]+" 注 ":"")+
			    (map[4]?"五獎 "+map[4]+" 注 ":"")+NOR);
		}
		else 
		{
			ppl = load_user(id);
		}

		if( !objectp(ppl) ) continue;

		ppl_money = count(map[0],"*",count(money[0],"/",total_prize[0]||1));
		ppl_money = count(ppl_money,"+",count(map[1],"*",count(money[1],"/",total_prize[1]||1)));
		ppl_money = count(ppl_money,"+",count(map[2],"*",count(money[2],"/",total_prize[2]||1)));
		ppl_money = count(ppl_money,"+",count(map[3],"*",count(money[3],"/",total_prize[3]||1)));
		ppl_money = count(ppl_money,"+",count(map[4],"*",FIFTH_REWARD));

		if( userp(ppl) )
			tell(ppl, "\n共得中獎獎金 "HIG"$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(ppl_money)+NOR" 元！！\n\n");

		if( map[0] )
		{
			shout("\n"+repeat_string(HIY"樂透開獎："+ppl->query_idname()+HIY"獲得頭獎 "+map[0]+" 注共 "+NUMBER_D->number_symbol(count(map[0],"*",count(money[0],"/",total_prize[0]||1)))+" 元！！！！\n",5)+"\n"NOR);

			ppl->add_title( random(2) ? HIC"樂透"NOR CYN"大帝"NOR : HIC"樂透"NOR CYN"之星"NOR );
		}
		
		catch {
			if( objectp(ppl) )
			{
				ppl->earn_money(MONEY_D->query_default_money_unit(), ppl_money);
				ppl->save();
			}
		};

		if( objectp(ppl) && !userp(ppl) )
			destruct(ppl);
	}

	lotto_info["time"]++;
	table = allocate_mapping(0);
	lotto_info["lotto_opening"] = 0;
	lotto_info["total_prize"] = "0";

	set_temp("status", HIY"頭獎累積獎金："+NUMBER_D->number_symbol(lotto_info["first_prize"])+NOR);
	save_object(DATA_PATH);

	set_heart_beat(6000);
}

void lotto_open_schedule(int *open_num, int *no_sort_open_num, int open_spec_num, mapping prize, int *total_prize, int total_lotto, int sec, int total_sec, int open)
{
	int *num, i, j, fit;
	string id;
	array arr1, arr2;

	if( !(--sec % (total_sec/7)) )
	{
		if( open < 6 )
			msg("目前已經開出第 "+(++open)+" 個號碼："+HIY+no_sort_open_num[open-1]+"號！"NOR+"\n", this_object(), 0, 1);
		else if( open == 6 )
		{
			msg("最後一球特別號："+HIY+open_spec_num+" 號！"NOR+"\n", this_object(), 0, 1);
			msg("目前正在計算中獎結果，請稍候。\n", this_object(), 0, 1);
			open++;
		}
	}


	foreach( id, arr1 in table )
	{
		if( !sizeof(arr1) )
			continue;

		if( !arrayp(prize[id]) )
			prize[id] = allocate(5);

		reset_eval_cost();

		foreach( arr2 in arr1 )
		{
			foreach( num in arr2 )
			{
				fit = 0;
				foreach( i in num )
				{
					foreach( j in open_num )
					{
						if( i == j )
						{
							fit++;
							break;
						}
					}
				}

				if( fit > 2 )
				{
					if( fit == 3 )
					{
						prize[id][4]++;
						total_prize[4]++;
					}
					else if( fit == 4 )
					{
						prize[id][3]++;
						total_prize[3]++;
					}
					else if( fit == 5 )
					{
						//檢查特別獎
						if( member_array( open_spec_num, num ) != -1 )
						{
							prize[id][1]++;
							total_prize[1]++;
						}
						else
						{
							prize[id][2]++;
							total_prize[2]++;
						}
					}
					else if( fit == 6 ) 
					{
						prize[id][0]++;
						total_prize[0]++;
					}
				}
			}

			table[id] = table[id][1..];

			if( !sizeof(table[id]) )
				map_delete(table, id);

			call_out( (:lotto_open_schedule ,open_num, no_sort_open_num, open_spec_num, prize, total_prize, total_lotto, sec, total_sec, open:), 1); 
			return;
		}
	}

	if( open == 7 )
		lotto_open_finish(open_num, open_spec_num, prize, total_prize, total_lotto);
	else
		call_out( (:lotto_open_schedule ,open_num, no_sort_open_num, open_spec_num, prize, total_prize, total_lotto, sec, total_sec, open:), 1); 
}

int no_save(string file)
{
	return 1;
}

void lotto_open()
{
	int open_sec;
	int *open_num, *no_sort_open_num, open_spec_num, total_lotto;

	if( sizeof(add_number_schedule) )
		return tell(this_player(), "目前下注資料尚未處理統計完畢，請稍後再行開獎。\n");

	if( lotto_info["lotto_opening"] )
		return tell(this_player(), "目前已經在開獎了。\n");

	//計算所有投注數
	foreach(string id, array arr1 in table)
	foreach(array arr2 in arr1)
	total_lotto += sizeof(arr2);

	if( !total_lotto )
		return tell(this_player(), "無任何投注資料，無法開獎。\n");

	open_sec = (total_lotto/MAX_LOTTO_OPEN_PER_ROUND) < 7 ? 7 : (total_lotto/MAX_LOTTO_OPEN_PER_ROUND);
	msg(HIY"\n第 "+lotto_info["time"]+" 期樂透彩開始開獎了！！\n目前四十二顆彩球已經開始吹動，請大家耐心等候開獎結果("+open_sec+" 秒)。\n\n"NOR, this_object(),0,1);

	// 開獎號碼
	no_sort_open_num = random_number();
	open_num = sort_array(no_sort_open_num, (: $1 > $2 ? 1 : -1 :));
	// 特別號
	while( member_array((open_spec_num=random(42)+1), open_num) != -1 );

	lotto_info["lotto_opening"] = 1;
	lotto_open_schedule(
	    open_num, 
	    no_sort_open_num,
	    open_spec_num, 
	    allocate_mapping(0), 
	    allocate(5),
	    total_lotto,
	    open_sec,
	    open_sec,
	    0,
	);
}

void lotto_info()
{
	int i, count;
	string info = "";

	foreach(string id, array arr1 in table)
	{
		count = 0;
		foreach(array arr2 in arr1)
		count += sizeof(arr2);

		if( !count ) continue;

		info += sprintf(HIW"%-4s"NOR"%-30s"HIW"%10d"NOR WHT" 注\n"NOR, ++i+".", find_player(id) ? find_player(id)->query_idname() : capitalize(id), count);
	}
	if( sizeof(add_number_schedule) )
		info += "(下注資料正在計算中)\n";

	tell(this_player(), info==""?"目前無人下注。\n\n":"目前共有 "+i+" 人下注：\n"+info+"\n");
}

void lotto_clean()
{
	table = allocate_mapping(0);
	add_number_schedule = allocate_mapping(0);

	lotto_info = ([ 
	    "first_prize":"0",
	    "total_prize":"0", 
	    "history":allocate_mapping(0),
	    "time":1,
	    "lotto_opening":0 
	]);

	save_object(DATA_PATH);

	msg("$ME在樂透彩簽注機上進行視網膜掃瞄，經確認後已清除所有彩卷簽注資料。\n", this_player(),0,1);
}

// 對 id 加入新的數字組
void add_number()
{
	int count=0;

	foreach( string id, mixed num in add_number_schedule )
	{
		if( intp(num) )
		{
			while(num--)
			{
				if( ++count > MAX_LOTTO_PER_ROUND )
				{
					add_number_schedule[id] = num+1;
					break;
				}

				if( !num )
				{
					if( sizeof(table[id][sizeof(table[id])-1]) >= MAX_LOTTO_PER_ROUND )
						table[id] += allocate(1, allocate(0));
					table[id][<1] += ({ random_number() });
					map_delete(add_number_schedule, id);
					break;
				}

				if( sizeof(table[id][sizeof(table[id])-1]) >= MAX_LOTTO_PER_ROUND )
					table[id] += allocate(1, allocate(0));

				table[id][<1] += ({ random_number() });
			}
		}
		else if( arrayp(num) )
		{
			if( sizeof(table[id][sizeof(table[id])-1]) >= MAX_LOTTO_PER_ROUND )
				table[id] += allocate(1, allocate(0));

			table[id][<1] += ({ num });

			map_delete(add_number_schedule, id);

			if( ++count > MAX_LOTTO_PER_ROUND )
				break;
		}
		else map_delete(add_number_schedule, id);
	}

	if( sizeof(add_number_schedule) )
		call_out( (: add_number :), 1);
}

void do_lotto(object me, string arg)
{
	string money, id = me->query_id(1);

	int *num = allocate(6), i, count, continuing;

	switch( arg )
	{
	case 0:
		{
			string msg="";
			mapping mylotto;

			mylotto = table[id];

			if( !sizeof(mylotto) )
				return tell(me, pnoun(2, me)+"沒有任何投注資料。\n");

			foreach(array arr in mylotto)
			msg += implode(map(arr, (: implode($1, (: $1+","+$2 :)) :)), "\n");

			me->more(pnoun(2, me)+"所有的投注資料如下：\n"+msg);
			return;
			break;
		}
	case "info":	
		return lotto_info();
		break;
	case "clean":	
		//if( !wizardp(me) ) 
		//return tell(me, pnoun(2, me)+"沒有權利清除樂透資料。\n");
		//else 
		//return lotto_clean();
		break;
	case "open":
		if( lotto_info["lotto_opening"] )
			return tell(me, "目前正在開獎，不能做任何事。\n");

		if( !wizardp(me) ) 
			return tell(me, pnoun(2, me)+"沒有權利執行樂透開獎活動。\n");
		else 
			return lotto_open();
		break;
	default:
		break;
	}

	if( query("lotto_fixing") )
		return tell(me, "機台維修中，請稍後再行投注。\n");

	if( lotto_info["lotto_opening"] )
		return tell(me, "目前正在開獎，不能做任何事。\n");

	if( !arrayp(table[id]) )
		table[id] = allocate(1, allocate(0));

	// 計算 id 投注數目
	if( arrayp(table[id]) )
	{
		foreach( array answer in table[id] )
		count += sizeof(answer);
	}

	// 若是輸入六組數字
	if( sscanf(arg, "%d,%d,%d,%d,%d,%d", num[0],num[1],num[2],num[3],num[4],num[5]) == 6 
	    || sscanf(arg, "%d,%d,%d,%d,%d,%d -c %d", num[0],num[1],num[2],num[3],num[4],num[5],continuing) == 7)
	{
		if( count+1 > MAX_LOTTO_AMOUNT ) 
			return tell(me, "目前每人每期最高投注數為 "+MAX_LOTTO_AMOUNT+" 注。\n");

		if( wizardp(me) && count > 0 )
			return tell(me, "巫師每期只能投一注，避免干擾玩家經濟市場。\n");

		num = sort_array(num, (:$1>$2?1:-1:));

		for(i=0;i<6;i++)
		{
			if( num[i] < 1 || num[i] > 42 ) 
				return tell(me, "號碼不得低於 1 或高於 42。\n");
			if( i && num[i] <= num[i-1] )
				return tell(me, "號碼不可重覆簽注。\n");
		}

		if( sizeof(add_number_schedule) )
			return tell(me, "前一位投注者的資料還在處理中，請稍後再試。\n");

		if( !me->spend_money(MONEY_D->query_default_money_unit(), MONEY_PER_LOTTO) )
			return tell(me, pnoun(2, me)+"的 $"+MONEY_D->query_default_money_unit()+" 錢買不起任何一張彩券了！\n");

		lotto_info["total_prize"] = count(MONEY_PER_LOTTO*RECYCLE_PERCENT/100, "+", lotto_info["total_prize"]);

		add_number_schedule[id] = num;

		add_number();

		msg("$ME選好了六個數字：「"+num[0]+","+num[1]+","+num[2]+","+num[3]+","+num[4]+","+num[5]+"」，並花了 $"+MONEY_D->query_default_money_unit()+" "+MONEY_PER_LOTTO+" 元下注。\n",me,0,1);
	}
	else if( (big_number_check(arg) && sscanf(arg, "%d", i)) == 1 || sscanf(arg, "%d -c %d", i , continuing) == 2 )
	{
		if( i < 1 || i > MAX_SINGLE_TIME_LOTTO )
			return tell(me, "電腦選號注數不得低於 1 注或是一次下注高於 "+MAX_SINGLE_TIME_LOTTO+" 注。\n");

		if( count+i > MAX_LOTTO_AMOUNT ) 
			return tell(me, "目前每人每期最高投注數為 "+MAX_LOTTO_AMOUNT+" 注。\n");

		if( wizardp(me) && count+i > 1 )
			return tell(me, "巫師每期只能投一注，避免干擾玩家經濟市場。\n");

		if( sizeof(add_number_schedule) )
			return tell(me, "前一位投注者下注資料還在處理中，請稍後再試。\n");

		money = count(MONEY_PER_LOTTO, "*", i);

		if( !me->spend_money(MONEY_D->query_default_money_unit(), money) )
			return tell(me, pnoun(2, me)+"身上的 $"+MONEY_D->query_default_money_unit()+" 錢買不起 "+i*(continuing||1)+" 張彩卷！\n");

		lotto_info["total_prize"] = count(count(count(money, "*", RECYCLE_PERCENT),"/",100), "+", lotto_info["total_prize"]);

		set_temp("status", HIY"頭獎累積獎金："+NUMBER_D->number_symbol(count( copy(count(count(lotto_info["total_prize"],"*",38),"/",100)), "+", lotto_info["first_prize"])))+NOR;
		save_object(DATA_PATH);
		add_number_schedule[id] = i;

		add_number();

		msg("$ME利用電腦選號下了 "+NUMBER_D->number_symbol(i)+" 注，共花了 $"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(count(i,"*",MONEY_PER_LOTTO))+" 元。\n",me,0,1);
	}
	else return tell(me, "投注指令錯誤，請詳細閱讀 help lotto。\n");
}

void heart_beat()
{
	// 若尚有下注資料為處理則 set_heart_beat(50); 5 秒後再開獎
	if( !sizeof(add_number_schedule) )
	{
		if( lotto_info["total_prize"] == "0" )
			return;

		msg(HIY"十分鐘一次的樂透彩卷自動開獎時間到了！！\n"NOR, this_object(),0,1);
		set_heart_beat(0);
		lotto_open();
	}
	else
		set_heart_beat(50);
}

void create()
{
	restore_object(DATA_PATH);

	if( !mapp(lotto_info) )
		lotto_info = ([ 
		    "first_prize":"0",
		    "total_prize":"0", 
		    "history":allocate_mapping(0),
		    "time":1,
		    "lotto_opening":0,
		]);

	set_idname("lotto machine",HIC"樂透彩"NOR CYN"簽注機"NOR);


	set_temp("status", HIY"頭獎累積獎金："+NUMBER_D->number_symbol(lotto_info["first_prize"])+NOR);
	set("flag/no_amount", 1);
	set("long",help);
	set("unit","台");
	set("mass",-1);

	table = allocate_mapping(0);
	add_number_schedule = allocate_mapping(0);



	if( file_size(DATA_PATH) < 0 ) save_object(DATA_PATH);

	// 十分鐘 open 一次
	set_heart_beat(6000);
}
