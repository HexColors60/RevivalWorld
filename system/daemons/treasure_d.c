/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : treasure_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-07
 * Note   : 稀有珍寶精靈
 * Update :
 *  o 2006-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <map.h>

void create()
{

}

private string *treasure_books = 
({
    "/obj/book/skill/architectonic-adv",
    "/obj/book/skill/leadership-adv",
    "/obj/book/skill/strength-adv",
    "/obj/book/skill/physique-adv",
    "/obj/book/skill/intelligence-adv",
    "/obj/book/skill/agility-adv",
    "/obj/book/skill/charisma-adv",
});

void get_treasure_books(object me, int amount)
{
	object ob;

	// 稀有物品獲得率與智力有關
	if( random(1500000 - me->query_int()*random(1000)) > pow(amount, 1.2) ) return;

	ob = new(treasure_books[random(sizeof(treasure_books))]);

	msg("$ME突然在收集的材料中發現"+ob->query_idname()+"。\n", me, 0, 1);
	CHANNEL_D->channel_broadcast("city", me->query_idname()+"在收集的材料中發現"+ob->query_idname()+"。", me);

	ob->set_keep();
	ob->move(me);
}

void get_treasure_growth(object me, int growth_level, int land_type)
{
	int treasure_level;
	object ob;

	if( growth_level < 5 ) return;

	if( random(18000 - growth_level*500 - me->query_int()*random(40)) ) return;

	switch(land_type)
	{
	case FARM:
		{
			if( growth_level >= 10 && !random(3) )
			{
				treasure_level = 10;
				ob = new("/obj/farming/treasure/buddha_wood");
			}
			else if( growth_level >= 9 && !random(3) )
			{
				treasure_level = 9;
				ob = new("/obj/farming/treasure/stem_lotus");	
			}
			else if( growth_level >= 8 && !random(3) )
			{
				treasure_level = 8;
				ob = new("/obj/farming/treasure/beauty_hibiscus");	
			}
			else if( growth_level >= 7 && !random(3) )
			{
				treasure_level = 7;
				ob = new("/obj/farming/treasure/night_lotus");
			}
			else if( growth_level >= 6 && !random(3) )
			{
				treasure_level = 6;
				ob = new("/obj/farming/treasure/thousandleaf_lotus");
			}
			else
			{
				treasure_level = 5;
				ob = new("/obj/farming/treasure/blood_orchid");
			}

			msg("$ME的農田生產出了"+ob->query_idname()+"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"的 "+growth_level+" 級"HIY"農田"NOR"產出了 "+HIW+treasure_level+NOR" 級"HIC"稀"NOR CYN"有"NOR"物品「"+ob->query_idname()+"」");
			log_file("command/grow", me->query_idname()+"得到"+ob->query_idname());

			ob->set_keep();
			ob->move(me);
			break;
		}
	case PASTURE:
		{
			if( growth_level >= 10 && !random(3) )
			{
				treasure_level = 10;
				ob = new("/obj/pasture/treasure/justice");
			}
			else if( growth_level >= 9 && !random(3))
			{
				treasure_level = 9;
				ob = new("/obj/pasture/treasure/giant_eagle");
			}
			else if( growth_level >= 8 && !random(3))
			{
				treasure_level = 8;
				ob = new("/obj/pasture/treasure/kylin");
			}
			else if( growth_level >= 7 && !random(3))
			{
				treasure_level = 7;
				ob = new("/obj/pasture/treasure/fly_horse");
			}
			else if( growth_level >= 6 && !random(3))
			{
				treasure_level = 6;
				ob = new("/obj/pasture/treasure/mammoth");
			}
			else
			{
				treasure_level = 5;
				ob = new("/obj/pasture/treasure/pack_animal");
			}

			msg("$ME的牧場生產出了"+ob->query_idname()+"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"的 "+growth_level+" 級"HIY"牧場"NOR"產出了 "+HIW+treasure_level+NOR" 級"HIC"稀"NOR CYN"有"NOR"物品「"+ob->query_idname()+"」");
			log_file("command/grow", me->query_idname()+"得到"+ob->query_idname());

			ob->set_keep();
			ob->move(me);
			break;
		}
	case FISHFARM:
		{
			if( growth_level >= 10 && !random(3) )
			{
				treasure_level = 10;
				ob = new("/obj/fishfarm/treasure/king_stone");
			}
			else if( growth_level >= 9 && !random(3) )
			{
				treasure_level = 9;
				ob = new("/obj/fishfarm/treasure/null_stone");	
			}
			else if( growth_level >= 8 && !random(3) )
			{
				treasure_level = 8;
				ob = new("/obj/fishfarm/treasure/bamboo_stone");	
			}
			else if( growth_level >= 7 && !random(3) )
			{
				treasure_level = 7;
				ob = new("/obj/fishfarm/treasure/purple_stone");
			}
			else if( growth_level >= 6 && !random(3) )
			{
				treasure_level = 6;
				ob = new("/obj/fishfarm/treasure/intelligence_stone");
			}
			else
			{
				treasure_level = 5;
				ob = new("/obj/fishfarm/treasure/integrity_stone");
			}

			msg("$ME的養殖場生產出了"+ob->query_idname()+"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"的 "+growth_level+" 級"HIY"養殖場"NOR"產出了 "+HIW+treasure_level+NOR" 級"HIC"稀"NOR CYN"有"NOR"物品「"+ob->query_idname()+"」");
			log_file("command/grow", me->query_idname()+"得到"+ob->query_idname());

			ob->set_keep();
			ob->move(me);
			break;
		}
	}
}
string query_name()
{
	return "稀有寶藏系統(TREASURE_D)";
}
