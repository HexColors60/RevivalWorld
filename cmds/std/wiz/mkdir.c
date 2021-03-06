/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mkdir.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-8
 * Note   : mkdir 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 mkdir 指令。
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "請輸入路徑名稱。\n", CMDMSG);
	
	arg = resolve_path(me, arg);
		
	if( mkdir(arg) )
		tell(me, "建立目錄 "+arg+" 完畢。\n", CMDMSG);
	else
		tell(me, pnoun(2, me)+"沒有權利建立目錄 "+arg+"。\n", CMDMSG);
}