/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ed.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

// save_ed_setup and restore_ed_setup are called by the ed to maintain
// individual options settings. These functions are located in the master
// object so that the local admins can decide what strategy they want to use.
/* 儲存一個使用者的編輯程式設定或組態設定 */
int save_ed_setup(object user, int code)
{
    	if ( !intp(code) ) return 0;

	return set("edit_setup", code, user);
}

// Retrieve the ed setup. No meaning to defend this file read from
// unauthorized access.
/* 取得使用者的編輯程式設定 (setup) 或組態設定 */
int retrieve_ed_setup(object user)
{
    	return query("edit_setup", user);
}

/* 使用者不正常斷線時，備份其編輯內容 */
string get_save_file_name( string file, object user ) 
{
	return wiz_home(user->query_id(1))+"/unfinished_edit";
}

/* 讓 ed() 轉換欲讀寫檔案的相對路徑名稱為絕對路徑名稱 */
/* 路徑紀錄未定 by clode */
string make_path_absolute(string file)
{
	return file;
}