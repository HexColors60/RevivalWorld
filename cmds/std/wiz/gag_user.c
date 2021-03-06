/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gag_user.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-02-05
 * Note   : force 指令
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
        gag_user <id> 設定某人不可使用 channels
        gag_user -d <id> 解除某人不可使用 channels
HELP;

private void command(object me, string arg)
{
	object you;

	if( !is_command() ) return;

	if(!arg) return tell( me, help );

	if( sscanf(arg,"-d %s",arg) == 1 )
	{
		if( !objectp( you = find_player(arg) ))
			return tell( me, "線上沒有 " + arg + " 這個人\n");

                delete("no_channel", you);
		tell( me, you->query_idname() + "開始可以使用公用頻道\n" );
		tell( you, "您從現在起解除禁止使用公用頻道\n");
		return;
	}	

	if( objectp( you = find_player(arg) ) )
	{
                set( "no_channel", 1, you);
		tell( me, you->query_idname() + "已經無法始用公用頻道\n" );
		tell( you, "您從現在起無法使用公用頻道, 如有任何問題請向巫師反應\n");
		return;
	} else
		return tell( me, "線上沒有 " + arg + " 這個人\n");


}
