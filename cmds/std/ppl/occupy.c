/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : occupy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-02-27
 * Note   : 佔領指令
 * Update :
 *  o 2005-00-00  
 *
 -----------------------------------------
 */
 
#include <ansi.h>
#include <feature.h>
#include <location.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    佔領城市指令，無市籍的玩家可佔領並成立新城市，有市籍之玩家則需由市長
來佔領城市的其他衛星都市。

occupy city	- 佔領此座城市(佔領時需支付 100,000,000 的整頓費用)

HELP;

string WARNING = @LONG

    請確定您已經詳細閱讀相關文件並了解有關建立新城市後可能遇到的各種狀況
包括佔領時需要支付 $RW 100,000,000 的鉅額整頓費用，以及後續基礎建設需要的
各項支出，初期經營不善將容易導致城市毀滅。
LONG;


// 佔領建立全新城市
void confirm_occupy_new_city(object me, mapping info, string arg)
{
	if( !arg || (lower_case(arg) != "y" && lower_case(arg) != "yes") )
	{
		tell(me, pnoun(2, me)+"取消了佔領程序。\n");
		return me->finish_input();
	}
	
	CITY_D->occupy_new_city(me, info["id"], info["name"], info["moneyunit"]);
	
	me->finish_input();
}


// 成立衛星都市
void confirm_occupy_section_city(object me, string arg)
{
	int needed_flourish;
	int total_num;
	string city;
	
	if( !arg || (lower_case(arg) != "y" && lower_case(arg) != "yes") )
	{
		tell(me, pnoun(2, me)+"取消了佔領程序。\n");
		return me->finish_input();
	}

	city = query("city", me);
	
	total_num = CITY_D->query_city_num(city);
	needed_flourish = 50000 + (total_num-1)*5000;

	for(int num=0;num<total_num;num++)
	{
		if( CITY_D->query_section_info(city, num, "flourish") < needed_flourish )
			return tell(me, "現有衛星都市都必須到達 "+needed_flourish+" 的繁榮度才能再佔領新城市。\n");
	}

	CITY_D->occupy_section_city(me);
	
	me->finish_input();
}


void input_money_unit(object me, mapping info, string arg)
{
	if( !arg || !arg[0] )
	{
		tell(me, pnoun(2, me)+"取消了佔領程序。\n");
		return me->finish_input();
	}

	arg = ansi(arg);

	info["moneyunit"] = arg;
	tell(me, pnoun(2, me)+"將新城市的貨幣單位設定為「"+arg+NOR"」\n");

	tell(me, NOR CYN"\n第四步："HIC"以上設定是否正確無誤？(Yes/No)"NOR CYN"："NOR);
	input_to((: confirm_occupy_new_city, me, info :));
}		

void input_color_id(object me, mapping info, string arg)
{
	if( !arg || !arg[0] )
	{
		tell(me, pnoun(2, me)+"取消了佔領程序。\n");
		return me->finish_input();
	}

	arg = ansi(arg);
	arg = replace_string(arg, " ", "");

	info["id"] = arg;
	tell(me, pnoun(2, me)+"將新城市的彩色英文名稱取名為「"+arg+NOR"」\n");

	tell(me, NOR CYN"\n第三步："HIC"請替"+pnoun(2, me)+"的新城市設定貨幣的單位(2 個英文字)\n"NOR CYN"："NOR);
	input_to((: input_money_unit, me, info :));
}


void input_color_name(object me, mapping info, string arg)
{
	if( !arg || !arg[0] )
	{
		tell(me, pnoun(2, me)+"取消了佔領程序。\n");
		return me->finish_input();
	}

	if( query("encode/gb", me) )
		arg = B2G(arg);

	arg = ansi(arg);
	arg = replace_string(arg, " ", "");

	info["name"] = arg;
	tell(me, pnoun(2, me)+"將新城市的彩色中文名稱取名為「"+arg+NOR"」\n");

	tell(me, NOR CYN"\n第二步："HIC"請替"+pnoun(2, me)+"的新城市取一個彩色英文名稱(3 - 12 個英文字)\n"NOR CYN"："NOR);
	input_to((: input_color_id, me, info :));
}

private void do_command(object me, string arg)
{
	string mycity;
	object env = environment(me);
	array loc = query_temp("location", me);
	
	if( arg != "city" )
		return tell(me, pnoun(2, me)+"想要佔領什麼？\n");

	if( !env->is_maproom() || !CITY_D->query_city_info(loc[CITY], "fallen") )
		return tell(me, pnoun(2, me)+"只能佔領廢棄的城市。\n");

	if( CITY_D->query_city_info(loc[CITY], "no_occupy") )
		return tell(me, "目前無法佔領此城市。\n");
	
	mycity = query("city", me);

	if( !mycity )
	{
		mapping info = allocate_mapping(0);
			
		if( query("total_online_time", me) < 30*24*60*60 )
			return tell(me, pnoun(2, me)+"的上線時間至少必須超過一個月才能佔領新城市。\n");
			
		if( count(MONEY_D->query_all_cash(me->query_id(1), MONEY_D->query_default_money_unit()), "<", "10000000000") )
			return tell(me, pnoun(2, me)+"的必須擁有至少 $"+MONEY_D->query_default_money_unit()+" 100 億的現金才能佔領新城市。\n");
			
		CHANNEL_D->channel_broadcast("news", me->query_idname()+"正在考慮於"+CITY_D->query_city_idname(loc[CITY])+"建立全新城市");
		tell(me, WARNING+NOR CYN"\n第一步："HIC"請替"+pnoun(2, me)+"的新城市取一個彩色中文名稱(1 - 5 個中文字)\n"NOR CYN"："NOR);
		input_to((: input_color_name, me, info :));
	}
	else if( CITY_D->is_mayor(mycity, me) )
	{
		CHANNEL_D->channel_broadcast("news", CITY_D->query_city_idname(mycity)+"市長"+me->query_idname()+"正在考慮佔領"+CITY_D->query_city_idname(loc[CITY])+"作為衛星都市");
		tell(me, WARNING+NOR CYN"\n警告："HIC"佔領衛星都市後將無法個別放棄衛星都市，若經營不善將導致整個"+CITY_D->query_city_idname(mycity)+HIC"滅亡\n是否確定佔領此座城市作為"+CITY_D->query_city_idname(mycity)+HIC"之衛星都市？(Yes/No)"NOR CYN"："NOR);
		input_to((: confirm_occupy_section_city, me :));
	}
	else
		tell(me, pnoun(2, me)+"是"+CITY_D->query_city_idname(mycity)+"的市民，若要建立全新城市"+pnoun(2, me)+"必須先註銷本市市籍，若是想佔領衛星都市則必須由市長佔領。\n");
}