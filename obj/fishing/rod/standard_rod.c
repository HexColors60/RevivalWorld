/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : rod.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : Just For Funny
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */


#include <ansi.h>
#include <message.h>
#include <inherit.h>
#include <map.h>
#include <daemon.h>
#include <delay.h>

inherit	STANDARD_OBJECT;
// 處理放魚餌
void do_hook(object me, string arg, object rod)
{
	object bait;

	if ( !arg )
		return tell(me,	pnoun(2, me)+"想要放什麼東西在"+rod->short(1)+"的釣鉤上？\n");

	if( !objectp(bait = present(arg, me)) )
		return tell(me,	pnoun(2, me)+"身上沒有 "+arg+" 這種東西\n");

	if( !bait->is_bait() )
		return tell(me,	rod->query_idname()+"不能夠當作魚餌。\n");

	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());
		
	if( !query_temp("bait", rod) )
	{
		msg("$ME把"+bait->query_idname()+"放到"+rod->query_idname()+"的魚鉤上。\n", me,	0, 1);
		set_temp("bait", query("bait", bait), rod);
		set_temp("status", NOR"- "HIY"掛著"+bait->query_name()+NOR, rod);
		destruct(bait, 1);
	}
	else
		return tell(me,	rod->query_idname()+"上已經有魚餌了。\n");
}

// 處理寶箱事件
void check_yn(object me, string arg)
{
	object ob;
	int i;

	me->remove_delay(FISHING_DELAY_KEY);

	if( arg == "" || lower_case(arg) == "y" )
	{
		switch( random(7) )
		{
			case 0:
				i = range_random(4, 10);
				ob = new("/obj/fishing/sp_fish/fish"+random(3));
				set_temp("amount", i, ob);
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"居然毫髮未傷。\n", me, 0, 1);
				ob->move(me);
				break;
			case 1:
				i = range_random(8, 16);
				ob = new("/obj/fishing/sp_fish/fish"+random(3));
				set_temp("amount", i, ob);
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"發出『啪』的一聲，聽起來似乎受到很大的傷害。\n", me, 0, 1);
				if( addn_temp("endurance", -1000) < 1 )
					set_temp("endurance", 10);
				ob->move(me);
				break;
			case 2:
				i = range_random(4, 10);
				ob = new("/obj/fishing/adv_fish/fish"+random(4));
				set_temp("amount", i, ob);
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"居然毫髮未傷。\n", me, 0, 1);
				ob->move(me);
				break;
			case 3:
				i = range_random(8, 16);
				ob = new("/obj/fishing/adv_fish/fish"+random(4));
				set_temp("amount", i, ob);
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"發出『啪』的一聲，聽起來似乎受到很大的傷害。\n", me, 0, 1);
				if( addn_temp("endurance", -600) < 1 )
					set_temp("endurance", 10);
				ob->move(me);
				break;
			case 4:
				if( query_temp("endurance") > 1000 )
				{
					check_yn(me, "y");
					return;
				}
				ob = new("/obj/fishing/other/box");
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"卻因此無法承受巨力而斷掉了。\n", me, 0, 1);
				set_temp("endurance", -1);
				ob->move(me);
				break;
			case 5:
				ob = new("/obj/fishing/other/box");
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"發出『啪』的一聲，聽起來似乎受到很大的傷害。\n", me, 0, 1);
				if( addn_temp("endurance", -600) < 1 )
					set_temp("endurance", 10);
				ob->move(me);
				break;
			case 6:
				ob = new("/obj/fishing/other/box");
				msg("\n$ME心一橫，奮力的一拉，把"+ob->short(1)+"從水中拉了起來，而"+this_object()->query_name()+"居然毫髮未傷。\n", me, 0, 1);
				ob->move(me);
				break;
		}
	}
	else
		tell(me, pnoun(2, me)+"放棄使勁用力拉，把魚線放鬆，讓纏在魚鉤上的魚給跑了。\n");
}
// 處理將要釣到東西
void get_fish(object me)
{
	int rod_int, bait_int, fish_sk, break_int;
	object ob;
	if( !objectp(me) ) return;
	
	me->remove_delay(FISHING_DELAY_KEY);

	rod_int = query("rod_level");
	bait_int = query_temp("bait");
	fish_sk	= to_int(me->query_skill_level("fishing")/1.5);

	if( !random(200) )
		fish_sk += 50;

	// 亂數(魚竿等級) + 釣魚技能等級 + 魚餌等級
	switch(random(rod_int)+fish_sk+bait_int)
	{
		case 0..19:
			ob = new("/obj/trash/trash"+random(6));
			msg("\n$ME一拉"+this_object()->query_name()+"起來一看，居然釣到了．．．．"+ob->query_idname()+"。\n", me, 0, 1);
			break;
		case 20..39:
			msg("\n$ME一拉"+this_object()->query_name()+"起來一看，居然什麼都沒釣到。\n", me, 0, 1);
			break;
		case 40..89:
			ob = new("/obj/fishing/fish/fish"+random(9));
			msg("\n$ME一拉"+this_object()->query_name()+"起來一看，居然釣到了．．．．"+ob->query_idname()+"。\n", me, 0, 1);
			break;
		case 90..109:
			ob = new("/obj/fishing/adv_fish/fish"+random(4));
			msg("\n$ME一拉"+this_object()->query_name()+"起來一看，居然釣到了．．．．"+ob->query_idname()+"。\n", me, 0, 1);
			break;
		case 110..129:
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			msg("\n一股前所未有的強力拉勁使$ME差點失去平衡被拉入水裡，而$ME經過許久纏鬥，終於把"+ob->query_idname()+"拉起來了。\n",	me, 0, 1);
			break;
		case 130..132:
                  	msg("\n只見$ME一直使勁用力的拉，可是還是完全拉不起來這釣到的東西。\n", me, 0, 1);
			tell(me, pnoun(2, me)+"要使盡全力用力的拉嗎？("HIY"Y"NOR"/n)\n");
			input_to( (: check_yn, me :) );
			break;
		case 133..9999:
			ob = new("/obj/fishing/sp_fish/fish"+random(3));
			msg("\n一股前所未有的強力拉勁使$ME差點失去平衡被拉入水裡，而$ME經過許久纏鬥，終於把"+ob->query_idname()+"拉起來了。\n",	me, 0, 1);
			break;
	}

	delete_temp("bait");
	delete_temp("status");
	
	if( objectp(ob)	&& !me->get_object(ob)	)
	{
		msg("$ME拿不動那麼多東西了，只見釣起來的"+ob->query_idname()+"又落回河裡。\n", me, 0, 1);
		destruct(ob);
	}
	// 處理釣竿是否要斷的機率
	break_int = (100/(fish_sk||1));
	if( break_int < 5 ) break_int = 5;
	if( break_int > 15 ) break_int = 15;
	
	if( addn_temp("endurance", range_random(-break_int, 1),this_object() ) < 0 )
	{
		switch( random(3) )
		{
			case 0..1:
				msg("\n$ME手上的"+this_object()->query_name()+"發出『啪』的一聲，聽起來似乎已經快斷了。\n", me, 0, 1);
				break;
			case 2:
				msg("\n$ME手上的"+this_object()->query_name()+"發出『啪』的一聲，一靠近察看發現它已經斷了。\n", me, 0, 1);
				set_temp("endurance", -1);
				break;
		}
	}	
}

void fishing(object me)
{
	if( !objectp(me) ) return;
	
	if( !me->cost_stamina(50) )
	{
		me->remove_delay(FISHING_DELAY_KEY);
		return tell(me,	pnoun(2, me)+"太累了，無法繼續專心的釣魚。\n");
	}

	me->add_social_exp(150+random(150));

	// 處理釣魚事件
	switch(	random(10) ) {
		case 0..2:
			msg("\e[1;"+3+random(8)+"m浮標在水上飄啊飄的，似乎沒有什麼異狀發生．．．\n"NOR,	me, 0, 1);
			call_out( (: fishing, me :), 6);
			break;
		case 3..5:
			msg("\e[1;"+3+random(8)+"m浮標似乎微微的動了，但沒多久又停止擺動了．．．\n"NOR,	me, 0, 1);
			call_out( (: fishing, me :), 6);
			break;
		case 6:
			if(!query_temp("bait"))
				msg("\n\e[1;"+3+random(8)+"m浮標突然猛力的沉入水底，可是$ME卻來不及拉上魚竿，魚就這樣跑掉了．．．\n"NOR, me, 0,	1);
			else 
			{
				msg("\n\e[1;"+3+random(8)+"m浮標突然猛力的沉入水底，可是$ME卻來不及拉上魚竿，魚餌就這樣被吃掉了．．．\n"NOR, me, 0, 1);
				delete_temp("bait");
				delete_temp("status");
			}
			
			me->remove_delay(FISHING_DELAY_KEY);
			break;
		case 7..9:
			msg("突然間，浮標不停的激烈上下擺動著，$ME順手用力一拉．．．\n", me, 0,	1);
			if( !me->cost_stamina(50) )	
			{
				me->remove_delay(FISHING_DELAY_KEY);
				msg("當$ME拉到一半時，突然感覺雙手無力，只好放棄釣起這尾魚了。\n", me, 0, 1);
			}
			else
				call_out( (:get_fish ,me :), 2);
			break;
	}
}
void do_fishing(object me, string arg)
{
	int a = me->query_skill_level("fishing");
	int b = query("level");
	object env = environment(me);
	array loc = query_temp("location", me);

	// 特權
	if( wizardp(me) ) {
		msg("$ME拿出"+this_object()->query_name()+"，往前方的河面用力一甩出竿。\n", me, 0,	1);
		call_out( (: fishing, me :), 1);
		return;
	}

	// 斷掉的魚竿無法釣魚
	if( query_temp("endurance") < 0 )
		return tell(me, pnoun(2, me)+"無法用斷掉的"+this_object()->query_name()+"來釣魚。\n");

	// 無法在非地圖中釣魚	
	if( !env->is_maproom() )
		return tell(me,	pnoun(2, me)+"無法在房間裡釣魚。\n");

	if( environment() != me )
		return tell(me, pnoun(2, me)+"必須將"+this_object()->query_name()+"拿在手上才能開始釣魚。\n");

	// 技能等級不到無法拿魚竿
	if( a < b )
		return tell(me, pnoun(2, me)+"的釣魚經驗不足，不懂該如何使用這個魚竿。\n");

	// Delay 中不能下指令
	if( me->is_delaying() )
		return tell(me,	me->query_delay_msg());

	if( member_array( RIVER	, values((MAP_D->query_map_system(loc))->query_coor_range(loc, TYPE, 1))) != -1	)
	{
		msg("$ME拿出"+this_object()->query_name()+"，往前方的河面用力一甩出竿。\n", me, 0, 1);
		call_out( (: fishing, me :), 5);
		me->start_delay(FISHING_DELAY_KEY, 999, pnoun(2, me)+"正忙著釣魚。\n");
	}
	else 
		return tell(me,	"這附近沒有可以釣魚的地方。\n");
}

int remove()
{
	object env = environment();
	
	if( objectp(env) )
		env->remove_delay(FISHING_DELAY_KEY);
}

void leave(object ob)
{
	if( objectp(ob) )
		ob->remove_delay(FISHING_DELAY_KEY);
}

mapping	actions	= 
([
	"fishing" 	: (: do_fishing :),
	"hook" 		: (: do_hook :)
]);
