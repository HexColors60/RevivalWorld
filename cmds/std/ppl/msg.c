/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : msg.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : msg 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    顯示最近收到的 1000 個聊天/謠言頻道或是密語訊息。

msg +種類		增加某類訊息的紀錄
msg -種類		停止某類訊息的紀錄
msg 筆數		設定訊息紀錄筆數，最少紀錄 10 筆，最多紀錄 1000 筆
msg clear		刪除所有訊息
msg info		查詢目前的訊息紀錄項目

訊息種類包括：公開頻道(chat, city, rumor, news, ...), emotion, say, tell

HELP;

private void do_command(object me, string arg)
{
	int i, size;
	array data;
	object target;
	string type;
		
	if( arg == "clear" )
	{
		me->clear_msg_log();
		return tell(me, pnoun(2, me)+"清除掉所有的訊息紀錄。\n");
	}

	if( arg == "info" )
	{
		mapping setup = query("msg", me);
		
		if( !mapp(setup) || !sizeof(keys(setup)-({ "size" })) )
			return tell(me, pnoun(2, me)+"沒有設定紀錄任何訊息。\n");
		
		map_delete(setup, "size");
		
		return tell(me, pnoun(2, me)+"目前設定的訊息紀錄項目："+implode(keys(setup), ", ")+"\n");
	}
	
	if( arg && (size = to_int(arg)) )
	{
		if( size < 10 || size > 1000 )
			return tell(me, "訊息最少紀錄 10 筆，最多紀錄 1000 筆。\n");
		
		set("msg/size", size, me);
		tell(me, pnoun(2, me)+"設定訊息紀錄筆數為 "+size+" 筆。\n");
		me->save();
		return;
	}

	if( arg && sscanf(arg, "+%s", type) == 1 )
	{
		if( type == "say" || type == "tell" || type == "emotion" || type == "announce" || CHANNEL_D->valid_channel(SECURE_D->level_num(me->query_id(1)), type) )
		{
			set("msg/"+type, 1, me);
			tell(me, pnoun(2, me)+"開始紀錄 "+type+" 的訊息。\n");
			me->save();
			return;
		}
		else
			return tell(me, "無法設定 "+type+" 這種訊息。\n");
	}
	else if( arg && sscanf(arg, "-%s", type) == 1 )
	{
		if( type == "say" || type == "tell" || type == "emotion" || type == "announce" || CHANNEL_D->valid_channel(SECURE_D->level_num(me->query_id(1)), type) )
		{
			delete("msg/"+type, me);
			tell(me, pnoun(2, me)+"停止紀錄 "+type+" 的訊息。\n");
			me->save();
			return;
		}
		else
			return tell(me, "無法設定 "+type+" 這種訊息。\n");
	}
	
	if( wizardp(me) && arg && objectp(target = find_player(arg)) && SECURE_D->level_num(target) <= SECURE_D->level_num(me) )
		data = target->query_msg_log();
	else
		data = me->query_msg_log();
    	
    	size = sizeof(data);
    	
    	if( size )
    	{
    		string *str;
    			
    		str = ({ (target?target->query_idname():"")+"共有 "+(size/3)+" 筆舊訊息紀錄：\n─────────────────────────────────────\n" });
    		
    		if( arg && !target )
    		{
    			for(i=0;i<size;i+=3)
    				if( data[i+1] == arg )
    					str += ({ HIW+ctime(data[i])[11..15]+NOR"-"+data[i+2] });
    		}
    		else
    		{
    			for(i=0;i<size;i+=3)
    				str += ({ HIW+ctime(data[i])[11..15]+NOR"-"+data[i+2] });
    		}
   
   		str += ({ "────────────────────────────目前時刻 "HIW+ctime(time())[11..15]+NOR"──\n" });
   		me->more(implode(str, ""));
	}	
    	else
    		return tell(me, (target?target->query_idname():"")+"目前沒有任何訊息紀錄。\n");
}
