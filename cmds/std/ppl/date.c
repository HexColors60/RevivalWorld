/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : realtime.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-4
 * Note   : realtime 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
查詢日期

這個指令可以查詢真實世界的日期與時間

相關指令: time
HELP;

private void do_command(object me, string arg)
{
	tell(me, TIME_D->real_time_description()+" ( "+TIME_D->replace_ctime(time())+" )。\n", CMDMSG);
}
