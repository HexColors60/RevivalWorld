/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chair.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
 * Note   : 傢俱 - 椅子
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <material.h>

inherit OBJECT_ACTION_MOD;

// 坐
void do_sit(object me, string arg)
{
	
	
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"sit"	: (: do_sit :),
		]),
]);


// 設定物件資料
nosave array object_info = ({
	
	// 基本 name
	"椅子"
	
	// 基本 id
	,"chair"

	// 基本材料需求
	,([ WOOD:10 ])
	
	// 製造此產品所需要的科技值
	,300
	
	// 基本製造速度(每位員工/每遊戲時間一分鐘)
	,30
	
	// 基本價值
	,"300"
	
	// 科技樹資料
	,0
});
