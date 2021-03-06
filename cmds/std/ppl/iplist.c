/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : iplist.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-12
 * Note   : iplist 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ip.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
檢查目前本遊戲的 IP 限制設定
HELP;

private void do_command(object me, string arg)
{
	string ip, msg, option;
	mapping ipdata = IP_D->query_ipdata();
	mapping data; 
	msg  = HIW"IP                  設定           設定時間         取消時間\n"NOR;
	msg += WHT"─────────────────────────────────────\n"NOR;

	if( sizeof(ipdata) )
		foreach(ip in sort_array(keys(ipdata), 1))
	{
		data = ipdata[ip];
		switch(data["status"])
		{
		case BAN: 		option = HIR"禁止登入"NOR; break;
		case CMDLIMIT:		option = HIY"指令速度限制"NOR; break;
		case OK:		continue;
		}
		msg += sprintf("%-20s%-15s"HIM"%-17s"NOR MAG"%s\n"NOR, ip, option, TIME_D->replace_ctime(data["recordtime"]), data["releasetime"] ? TIME_D->replace_ctime(data["releasetime"]) : "無期");
	}
	else
		msg += "沒有任何 IP 限制設定。\n";

	msg += WHT"─────────────────────────────────────\n"NOR;
	msg += "若"+pnoun(2, me)+"未違反任何遊戲規則但 IP 卻遭到系統限制，可隨時向巫師反應\n";
	//return tell(me, msg);
	return me->more(msg);
}
