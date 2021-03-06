/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : car.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
 * Note   : 車輛 - 小客車
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <material.h>

inherit OBJECT_ACTION_MOD;

inherit VEHICLE_ACTION_MOD;

// 設定物件動作
nosave mapping action_info =
([
	"help"	:
		([
"topics":
@HELP
    車輛。
HELP,
		]),
	"action":
		([
			"enter"	: (: ::do_enter_action($1, $2, $3) :),
		]),
]);


// 設定物件資料
nosave array object_info = ({
	
	// 基本 name
	"小客車"
	
	// 基本 id
	,"car"

	// 基本材料需求
	,([ GOLD : 100, IRON : 1000 ])
	
	// 製造此產品所需要的科技值
	,300000
	
	// 基本製造速度(每位員工/每遊戲時間一分鐘)
	,500
	
	// 基本價值
	,"500000"
	
	// 科技樹資料
	,0
});
