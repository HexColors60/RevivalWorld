/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tell.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-02
 * Note   : tell 指令
 * Update :
 *  o 2000-00-00  
 *  o 跟 say 顏色太近常搞混換個顏色
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
溝通指令

這個指令可以讓您與目前正在線上的其他使用者溝通。

指令格式:
tell				查詢上一位向您說話的使用者
tell <使用者代號> <訊息>	與其他使用者說話
tell -r <訊息>			回覆上一位向您說話的使用者

tell -r 的預設指令別名為 reply

相關指令:
HELP;

#define WARNING_IDLE_TIME	300

private void do_command(object me, string arg)
{
	string target, msg, *ignore;
	object target_ob;
	int reply;

	if( !arg || sscanf(arg, "%s %s", target, msg) != 2 )  
	{
		if( !(target = query_temp("reply_teller", me)) )
			return tell(me, "剛才並沒有任何人跟你講過話。\n", CMDMSG);

		target_ob = find_player(target);

		return tell(me, "上位跟你講話的使用者為: " + (target_ob ? target_ob->query_idname():target + " [已離線]") + "\n", CMDMSG);
	}

	if( target == "-r" )
	{
		if( !(target = query_temp("reply_teller", me)) )
			return tell(me, "剛才並沒有任何人跟你講過話。\n", CMDMSG);
		reply = 1;
	}

	if( !target_ob = find_player(target) )
		return tell(me, "目前線上沒有 " + target + " 這位使用者。\n", CMDMSG);

	ignore = query("deny_user", target_ob);

	if( arrayp(ignore) )
	{
		if( member_array( me->query_id(1), ignore ) >= 0 )
			return tell( me, HIG + target_ob->query_idname()+ "現在拒絕跟你說話 .\n"NOR);
	}

	if( target_ob == me )
		return tell(me, pnoun(2, me)+"對著自己喃喃的自言自語。\n", CMDMSG);

	msg("$ME告訴$YOU："HIY+msg+NOR+"\n", me, target_ob, 0, TELLMSG);
	target_ob->add_last_msg(me->query_idname()+"告訴"+pnoun(2, target_ob)+"："HIY+msg+NOR+"\n");
	me->add_last_msg(pnoun(2, me)+"告訴"+target_ob->query_idname()+"："HIY+msg+NOR+"\n");

	if( !interactive(target_ob) ) 
		return tell(me, target_ob->query_idname()+"已經斷線了，聽不到"+pnoun(2, me)+"講話。\n", CMDMSG);

	if( !wizardp(me) && !wizardp(target_ob) )
	{
		log_file("command/tell", me->query_idname(1)+"告訴"+target_ob->query_idname()+": "+msg, -1);
		//CHANNEL_D->channel_broadcast("nch", me->query_idname(1)+"告訴"+obj->query_idname()+"："+msg+"\n");
	}
	if( query_idle(target_ob) > WARNING_IDLE_TIME )
		tell(me, target_ob->query_idname()+"已經發呆超過 "+(WARNING_IDLE_TIME/60)+" 分鐘，可能暫時不會回應"+pnoun(2, me)+"。\n", CMDMSG);

	set_temp("reply_teller", me->query_id(1), target_ob);
}
