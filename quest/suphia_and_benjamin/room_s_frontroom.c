/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_s_frontroom.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-09
 * Note   : 蘇菲亞家的客廳
 * Update :
 *  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIW"客廳"NOR);
	set("long",@TEXT
　　這裡四週打掃的很乾淨，房內牆壁漆著令人感到溫暖的淡淡鵝黃色，透過窗
戶可清楚看到戶外的風景。在房間的中央放置著個舖著象牙白餐巾的餐桌及四張
木製餐椅，而在窗戶旁邊則放置著一張舖有軟墊的舒適躺椅，由這裡似乎可以看
到屋子的另一側是廚房。
TEXT);


	set("exits", ([
		//往東走是蘇菲亞他弟的房間
		"east"  : "/quest/suphia_and_benjamin/room_s_alvin_room",
		
		//往南走是門口
		"south"  : ({ 80, 68, 0, "lightforest", 0, 0, "80/68/0/lightforest/0/0" }),
		
		//往西走是牆壁 = ="
		//"west"  : ,
		
		//往北走是廚房
		"north"   : "/quest/suphia_and_benjamin/room_s_kitchen",
		
	]));

	set("lock/east", 1);
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));


	::reset_objects();
	replace_program(STANDARD_ROOM);
}
