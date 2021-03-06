/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : which.c
 * Author : Ekac@RevivalWorld
 * Date   : 2003-03-04
 * Note   : 尋找某指令所在位置。
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
        標準 which 指令。
HELP;

private void command(object me, string arg)
{
	function fp;
	object cmd_ob;
	string cmd_file;
	if( !is_command() ) return;

	if( !arg )
		return tell(me, "指令格式：which 指令\n");

	if( functionp(fp = COMMAND_D->find_wiz_command(arg)) &&
	    objectp(cmd_ob = function_owner(fp)) &&
	    strlen( cmd_file = base_name(cmd_ob) ) &&
	    cmd_file[0..5] == "/cmds/" )
		return tell(me, sprintf("指令 %s 位於 %s\n", arg, cmd_file) );

	return tell(me, "路徑中沒有為 "+arg+" 的指令。\n");
}
