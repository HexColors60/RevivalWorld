/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : firework.c
 * Author : 
 * Date   : 2000-00-00
 * Note   : 煙火
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <feature.h>
#include <object.h>
#include <daemon.h>
#include <message.h>
#include <inherit.h>
#include <ansi.h>

inherit STANDARD_OBJECT;

int do_light( object me, string arg );

mapping actions;

string *color = ({
    RED"", YEL"", GRN"", BLU"", MAG"", CYN"", WHT"",
    HIR"", HIG"", HIY"", HIB"", HIM"", HIC"", HIW""
});

string *msg = ({
    "五彩繽紛的火光在天空中散開，好不美麗。\n",
    "剎那天空被七彩光芒舖滿，令人眼花潦亂。\n",
    "橘紅色的火焰往上直射，又爆出無數銀色的帶狀火花。\n",
    "四射的火花又立刻各自爆開來，發出霹霹拍拍的細碎爆音。\n",
    "散放出千萬朵飛燄，向四處紛飛而去，讓你目不暇給。\n",
    "像一片瀑布般撒下七彩的火花，緩緩地從天頂飛旋而降。\n"
});

void create()
{
        set_idname( "firework", "煙火" );

        if( clonep() ) 
                set("shadow_ob", find_object(base_name(this_object())));
        else
        {
                set("long", "這是一用來慶祝的煙火，你可以點燃它(light firework)。\n");
                set("unit", "支");
                set("value", 100);
                set("mass", 2);
                set("badsell", 1);
        }

        actions = 
        ([ 
            "light"     : (: do_light :)
        ]);
}

int do_light( object me, string arg)
{
        string reason;
        int random_msg, random_color;

        if( !arg ) 
                return tell( me, "你想點燃什麼呢？\n");
        else if( arg == this_object()->query_id() ) 
        {
                random_msg = random(sizeof(msg));
                reason = msg[random_msg];
        } 
        else if( sscanf(arg, this_object()->query_id() + " %s", reason) == 1 ) 
        {
                reason = "火花和爆煙在空中排出『" + reason + "』\n";
        } 
        else return tell( me, "你想點燃什麼呢？\n");

	if( time() - query_temp("firework/lasttime", me) < 3600 )
	{
		if( addn_temp("firework/times", 1, me) > 10 )
			return tell(me, "短時間內不能再點燃煙火了。\n");
	}
	else
	{
		set_temp("firework/lasttime", time(), me);
		delete_temp("firework/times", me);
	}

        random_color = random(sizeof(color));

        shout( me->query_idname() + "點燃了一根煙火, 咻的一聲飛上天空...\n" + color[random_color] + "一道閃光劃過天際，一個煙火『碰』地一聲在空中爆開•••\n" + reason + NOR);
	
        destruct( this_object(), 1);
        return 1;
}
