/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : command.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : command 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
  指令查詢指令

  這個指令可查詢目前重生的世界中所有可使用的指令，項目可分為

  wizard	巫師指令
  player	玩家指令
  channel	頻道指令
  emotion	表情指令
  other	其他

  指令格式:
  command <項目代號>

  相關指令:
HELP;

mapping chinese =
([
	"wiz"			:	"巫師指令",
	"ppl"			:	"玩家指令",
	"chn"			:	"頻道指令",
	"emt"			:	"表情指令",
	"city"			:	"城市地圖指令",
	"individual/mayor"	:	"市長私人指令",
]);

private void command(object me, string arg)
{	
	int i, size;
	mixed all_commands;
	string *commands, msg="";

	if( !is_command() ) return;
	
	switch(arg)
	{
		case "wizard"	: arg = "wiz"; break;
		case "player"	: arg = "ppl"; break;
		case "channel"	: arg = "chn"; break;
		case "emotion"	: arg = "emt"; break;
		case "other"	: arg = "sub"; break;
		default:
			return tell(me, "請輸入查詢指令項目: wizard, player, channel, emotion, other。\n", CMDMSG);
	}
	all_commands = COMMAND_D->query_commands();

	if( arg == "sub" )
	{
		foreach( string dir, mapping cmds_map in all_commands[arg] )
		{
			commands = sort_array(keys(cmds_map), 1);
			
			msg += sprintf(HIC"%-12s"NOR CYN"─────────────────────────────\n"NOR, chinese[dir]);
					
			size = sizeof(commands);
			i = 0;
			while(i<size)
			{
				msg += sprintf("%@-15s\n", commands[i..i+4]);
				i += 5;
			}
		}
	}
	else if( arrayp(commands = all_commands[arg]) )
	{
		commands = sort_array(commands, 1);
				
		msg += sprintf(HIC"%-12s"NOR CYN"─────────────────────────────\n"NOR, chinese[arg]);
				
		size = sizeof(commands);
				
		while(i<size)
		{
			msg += sprintf("%@-15s\n", commands[i..i+4]);
			i += 5;
		}
	}
	else
		return error("指令列表格式錯誤。\n");

	msg += NOR CYN"───────────────────────────────────\n"NOR;
	me->more(msg);
}