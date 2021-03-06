/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recover.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-26
 * Note   : recover 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <secure.h>
#include <message.h>

inherit COMMAND;

#define LOG	"command/recover"

string help = @HELP
	標準 recover 指令。
HELP;

private void command(object me, string arg)
{
	string path, option, msg;
	int overwrite, stat_time;
	int target_level_num, my_level_num;
	object target;
	
	if( !is_command() ) return;
	
	if( !arg ) 
		return tell(me, pnoun(2,me)+"想要恢復哪位使用者的資料？\n", CMDMSG);
	
	sscanf( arg, "%s -%s", arg, option );
	
	if( !option )
		return tell(me, "請輸入 -backup 或 -deleted 的參數。\n", CMDMSG);
	
	if( sscanf( option, "%s -overwrite", option ) == 1 ) overwrite = 1;

	target_level_num = SECURE_D->level_num(arg);
	my_level_num = SECURE_D->level_num(me->query_id(1));
	
	switch( option )
	{
		case "backup":
			if( file_size(path = user_backup_path(arg)) != -2 )
				return tell(me, "沒有 "+arg+" 這位使用者的備份資料。\n", CMDMSG);
			
			msg = me->query_idname()+"以 "+TIME_D->replace_ctime(stat(path+"/data.o")[1])+" 的備份檔案取代了 "+capitalize(arg)+" 現有的資料。\n";
			
			if( file_size(user_path(arg)) == -2 )
			{
				if( overwrite )
				{
					if( my_level_num < target_level_num )
						return tell(me, pnoun(2,me)+"的權限不足以覆蓋 "+arg+" 的現有使用者資料。\n", CMDMSG);
					else if( !sizeof(rmtree(user_path(arg))[0]) )
						return tell(me, pnoun(2,me)+"的權限無法刪除 "+arg+" 的現有使用者資料。\n", CMDMSG);
					
				}
				else return tell(me, "若要覆蓋現有資料必須加上 -overwrite 參數。\n", CMDMSG);
			}
			
			if( file_size(user_path(arg)) == -2 || !sizeof(cptree(path, user_path(arg))[0]) )
				return tell(me, "恢復 "+arg+" 存檔失敗。\n", CMDMSG);
			
			stat_time = stat(path+"/data.o")[1];
			
			break;
		case "deleted":
			if( file_size(path = user_deleted_path(arg)) != -2 )
				return tell(me, "沒有 "+arg+" 這位使用者的刪除後備份資料。\n", CMDMSG);
			
			
			
			msg = me->query_idname()+"將整個佈滿灰塵的檔案室翻了一遍，終於從陰暗的角落找到儲存 "+capitalize(arg)+" 資料的磁片，並安裝回系統中。\n";
			
			if( file_size(user_path(arg)) == -2 )
			{
				if( overwrite )
				{
					if( my_level_num < target_level_num )
						return tell(me, pnoun(2,me)+"的權限不足以覆蓋 "+arg+" 的現有使用者資料。\n", CMDMSG);
					else if( !sizeof(rmtree(user_path(arg))[0]) )
						return tell(me, pnoun(2,me)+"的權限無法刪除 "+arg+" 的現有使用者資料。\n", CMDMSG);
						
					rmtree(user_backup_path(arg));
				}
				else return tell(me, "若要覆蓋現有資料必須加上 -overwrite 參數。\n", CMDMSG);
			}
			
			if( file_size(user_backup_path(arg)) == -2 || !sizeof(cptree(path, user_backup_path(arg))[0]) )
				return tell(me, "恢復 "+arg+" 存檔失敗。\n", CMDMSG);
				
			if( file_size(user_path(arg)) == -2 || !sizeof(cptree(path, user_path(arg))[0]) )
				return tell(me, "恢復 "+arg+" 存檔失敗。\n", CMDMSG);
			
			stat_time = stat(path+"/data.o")[1];
			
			rmtree(path);
			
			target = load_user(arg);
			
			PASSWORD_D->set_password(arg, copy(query("password", target)));
			
			delete("password", target);
			
			target->save();
			
			destruct(target);
			
			break;
		default:
			return tell(me, "請輸入 -backup 或 -deleted 的參數。\n", CMDMSG);
	}
	
	CHANNEL_D->channel_broadcast("news", msg);
	log_file(LOG, me->query_idname()+"恢復 "+arg+" 在 "+TIME_D->replace_ctime(stat_time)+" 的存檔。");
	
	if( objectp(target = find_player(arg)) ) 
	{
		tell(target, "\n"+me->query_idname()+"恢復了"+pnoun(2, me)+"在 "+TIME_D->replace_ctime(stat_time)+" 的存檔。\n"HIY"請重新登入連線。\n"NOR, CMDMSG);
		destruct(target);
	}
}	