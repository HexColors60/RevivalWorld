/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chfn.c
 * Author : Sinji@RevivalWorld
 * Date   : 2006-09-07
 * Note   : chfn 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <ansi.h>

#define CH_ID		(1<<0)
#define CH_NAME		(1<<1)
#define CH_EMAIL	(1<<2)

#define DEFAULT		CH_ID | CH_NAME | CH_EMAIL
#define HELP		"/doc/help/color"

inherit COMMAND;

string help = @HELP
更改使用者資訊

此指令可用來更改使用者資訊如下：

-i	修改玩家英文代號色彩
-n	修改玩家名稱及色彩
-e	修改玩家電子郵件信箱

如不加參數則表示全部修改

指令格式:
chfn [-i] [-n] [-e]

相關指令: finger, nick
HELP;

private void change_prompt(object me, int flag);
private void change_input(object me, int flag, string arg);
private void do_command(object me, string arg)
{
	int flag;
	if( arg )
	{
		if( sscanf(arg, "%*s-i%*s", arg) == 2 )	flag |= CH_ID;
		if( sscanf(arg, "%*s-n%*s", arg) == 2 )	flag |= CH_NAME;
		if( sscanf(arg, "%*s-e%*s", arg) == 2 ) flag |= CH_EMAIL;
	}
	if( !flag ) flag = DEFAULT;
	tell(me, "跳過請輸入 [ENTER]，取消請輸入 '~q'，色彩協助請輸入 '~h'\n");
	tell(me, WHT"["HIW"Change Information"NOR + WHT"]────────────────\n"NOR);
	change_prompt(me, flag);
}
private void change_prompt(object me, int flag)
{
	if( flag & CH_ID )
	{
		tell(me, "\nID - [" + me->query_id() + "]\n");
		tell(me, "請輸入" + pnoun(2, me) + "的彩色英文代號：");
		input_to( (: change_input, me, flag :), 2 );
	}
	else if( flag & CH_NAME )
	{
		tell(me, "\nNAME - [" + me->query_name() + "]\n");
		tell(me, "請輸入" + pnoun(2, me) + "的彩色名稱：");
		input_to( (: change_input, me, flag :), 2 );
	}
	else if( flag & CH_EMAIL )
	{
		tell(me, HIR"\n注意：信箱位址將作為日後索取角色備份檔案之依據。\n"NOR);
		tell(me, "EMAIL - [" + query("email", me) + "]\n");
		tell(me, "請輸入" + pnoun(2, me) + "的電子郵件信箱：");
		input_to( (: change_input, me, flag :), 2 );
	}
	return;
}
private void change_input(object me, int flag, string arg)
{
	string *res;

	arg = remove_ansi(arg);

	if( arg && query("encode/gb", me) ) arg = G2B(arg);

	if( !arg ) arg = "";

	if( arg == "~q" )
	{
		tell(me, "取消輸入。\n");
		return me->finish_input();
	}
	if( arg == "~h" )
	{
		tell(me, read_file(HELP));
		return change_prompt(me, flag);
	}
	if( flag & CH_ID )
	{
		if( arg == "" )
			tell(me, "跳過彩色英文代號設定。\n");
		else
		{
			if( !(res = me->set_idname(ansi(arg), 0)) )
			{
				tell(me, "新設定的英文代號與原有英文代號不同，請重新設定。\n");
				return change_prompt(me, flag);
			}
			tell(me, "彩色英文代號 '"+ res[0] +"' 設定完成。\n");
		}
		flag ^= CH_ID;
	}
	else if( flag & CH_NAME )
	{
		int len;

		if( arg == "" )
			tell(me, "跳過彩色名稱設定。\n");
		else
		{
			arg = ansi(arg);
			len = noansi_strlen(arg);
			if( len > 12 || len < 2 )
			{
				tell(me, "名稱必須在 1 到 12 個字母之間，請重新設定。\n");
				return change_prompt(me, flag);
			}
			foreach(int i in remove_ansi(arg))
			{
				if( i <= ' ' )
				{
					tell(me, "請勿始用控制碼或空白，請重新設定。\n");
					return change_prompt(me, flag);
				}
			}
			res = me->set_idname(0, ansi(arg));
			tell(me, "彩色名稱 '" + res[1] + "' 設定完成。\n");
		}
		flag ^= CH_NAME;
	}
	else if( flag & CH_EMAIL )
	{
		if( arg == "" )
			tell(me, "跳過電子郵件信箱設定。\n");
		else
		{
			if( sscanf(arg, "%*s@%*s.%*s") != 3 )
			{
				tell(me, "此信箱位址格式有問題。\n");
				return change_prompt(me, flag);
			}
			if( !set("email", arg, me) )
				tell(me, "電子郵件信箱 '" + arg + "' 設定失敗。\n");
			else
				tell(me, "電子郵件信箱 '" + arg + "' 設定完成。\n");
		}
		flag ^= CH_EMAIL;
	}
	if( flag ) return change_prompt(me, flag);
	return me->finish_input();
}
