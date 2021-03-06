/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_2.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-19
 * Note   : 標準房間
 * Update :
 *  2006-09-07 遷位 by Tzj@RevivalWorld
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
	set("short", GRN"農舍"NOR);
	set("long",@TEXT
    這裡是農舍裡的小臥室，地上鋪著一大片的草蓆，應該就是老農夫用來當作床
舖用的，木窗隨著微風噶噶作響，另人有種蕭然的感覺。
TEXT);

	set("exits", ([
		//原設定出口："north" : "/quest/old_farmer/room_1",
		
		//以下為`06-09-07，由 tzj 遷位所變更的位置。
		"west"  : "/quest/old_farmer/room_1",
		"north" : ({ 75, 39, 0, "lightforest", 0, 0, "75/39/0/lightforest/0/0" }),
	]));
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
