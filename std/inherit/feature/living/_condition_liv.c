/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _condition_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : 狀態
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

// 詢問狀態
nomask int in_condition(string condition_file)
{
	return mapp(query("condition/"+replace_string(condition_file, "/","#")));
}

// 啟動狀態, 1 秒為單位
varargs nomask void start_condition(string condition_file, int time, int heart_beat)
{
	mapping condition_setup = allocate_mapping(0);

	// 若已經在相同狀態內則先停止先前狀態
	if( in_condition(condition_file) )
		condition_file->stop_effect(this_object());

	// 設定狀態持續時間或使用預設時間
	if( !undefinedp(time) && time > 0 )
		condition_setup["time"] = time;
	else
		condition_setup["time"] = condition_file->query_default_condition_time();
		
	// 設定狀態心跳時間或使用預設心跳時間
	if( !undefinedp(heart_beat) && heart_beat > 0 )
		condition_setup["heart_beat"] = heart_beat;
	else
		condition_setup["heart_beat"] = condition_file->query_default_condition_heart_beat();

	set("condition/"+replace_string(condition_file, "/","#"), condition_setup);
	
	// 執行狀態啟動時的效果
	condition_file->start_effect(this_object());
}


// 移除狀態
nomask void stop_condition(string condition_file)
{
	delete("condition/"+replace_string(condition_file, "/","#"));
	
	// 執行狀態結束時的效果
	condition_file->stop_effect(this_object());
}

// 改變狀態時間
nomask void change_condition_time(string condition_file, int time)
{
	if( !in_condition(condition_file) ) return;
		
	addn("condition/"+replace_string(condition_file, "/","#")+"/time", time);
}

// 回傳狀態資料
varargs nomask mapping query_condition(string condition_file)
{
	if( undefinedp(condition_file) )
		return query("condition");

	return query("condition/"+replace_string(condition_file, "/","#"));
}
