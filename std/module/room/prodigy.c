
/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : prodigy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-07
 * Note   : 世界奇觀動作繼承物件
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

void do_buff(object me, string arg)
{
	
	
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"room"	:
	([
		"short"	: HIR"奇觀內部"NOR,
		"help"	:
			([
"topics":
@HELP
    奇觀內部可提供遊客休憩。
HELP,

"buff":
@HELP
擺設傢俱的指令，用法如下：
  未完成
HELP,

			]),
		"action":
			([
				"buff"	: (: do_buff :),
			]),
	]),

]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIR"世界"NOR RED"奇觀"NOR

	// 開張此建築物之最少房間限制
	,16

	// 城市中最大相同建築數量限制(0 代表不限制)
	,1

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,"300000000"
	
	// 建築物關閉測試標記
	,1

	// 繁榮貢獻度
	,400

	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,1
	
	// 建築物時代
	,1
});

