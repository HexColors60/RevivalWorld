/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv3.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-26
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>

#define CONDITION_ID		"sightseebless-adv3"
#define CONDITION_NAME		HIG"王者"NOR GRN"天下"NOR
#define CONDITION_TYPE		"加持"
#define DEFAULT_TIME		7200
#define DEFAULT_HEART_BEAT	0

// 回傳狀態英文名稱
string query_condition_id()
{
	return CONDITION_ID;
}

// 回傳狀態中文名稱
string query_condition_name()
{
	return CONDITION_NAME;
}

// 回傳狀態型態名稱
string query_condition_type()
{
	return CONDITION_TYPE;
}

// 回傳狀態預設持續時間
int query_default_condition_time()
{
	return DEFAULT_TIME;
}

// 回傳狀態預設心跳時間
int query_default_condition_heart_beat()
{
	return DEFAULT_HEART_BEAT;
}

// 啟動狀態時的效果
void start_effect(object ob)
{
	set("condition/"+replace_string(base_name(this_object()), "/","#")+"/"+BUFF_STR, 1, ob);
	set("condition/"+replace_string(base_name(this_object()), "/","#")+"/"+BUFF_PHY, 1, ob);
	set("condition/"+replace_string(base_name(this_object()), "/","#")+"/"+BUFF_INT, 1, ob);
	set("condition/"+replace_string(base_name(this_object()), "/","#")+"/"+BUFF_CHA, 1, ob);
	set("condition/"+replace_string(base_name(this_object()), "/","#")+"/"+BUFF_AGI, 1, ob);
	msg("$ME進入「"CONDITION_NAME"」的"CONDITION_TYPE"狀態。\n", ob, 0, 1);
}

// 結束狀態時的效果
void stop_effect(object ob)
{
	msg("$ME脫離「"CONDITION_NAME"」的"CONDITION_TYPE"狀態。\n", ob, 0, 1);
}

// 狀態進行中的效果
void heart_beat_effect(object ob)
{


}