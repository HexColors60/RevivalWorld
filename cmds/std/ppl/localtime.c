/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : localtime.c
 * Author : Ekac@RevivalWorld
 * Date   : 2003-07-22
 * Note   : 
 * Update :
 *  o 2003-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <localtime.h>
#include <daemon.h>

inherit COMMAND;

#define LOCAL_TIMEZONE	  "TAIWAN"
#define HOUR		    3600

private nosave mapping TIMEZONES =
([
	/* Europe */
	"GMT"	   :       0,      // Greenwich Mean Time
	"CET"	   :       1,      // Central Europe Time
	"EET"	   :       2,      // Eastern European Time
	/* North America */
	"EST"	   :       -5,     // Eastern Standard Time
	"CST"	   :       -6,     // Central Standard Time
	"MST"	   :       -7,     // Mountain Standard Time
	"PST"	   :       -8,     // Pacific Standard Time
	/* Asia */
	"DEER" 		:	8,
	"JAPAN"	 :       9,
	"TAIWAN"	:       8,
	"THAILAND"      :       7,
	"TURKEY"	:       2,
	/* Australia and Pacific Region */
	"AWST"	  :       8,      // Australian Western Standard Time
	"ACST"	  :       8.5,    // Australian Central Standard Time
	"AEST"	  :       10,     // Australian Eastern Standard Time
	"NZST"	  :       12,     // New Zealand Standard Time
]);

private nosave string array DAYLIGHT_SAVINGS =
	({ "GMT", "CET", "EET", "EST", "CST", "MST", "PST", "ACST", "AEST", "NZST" });


string help = @HELP
查詢各地時間

這個指令可以查詢真實世界各時區的時間
您可以使用 set 指令來設定您所在的時區，詳情請參考 help set

以下為所支援的時區:

- 歐洲
	GMT*		Greenwich Mean Time
	CET*		Central Europe Time
	EET*		Eastern European Time
- 北美洲
	EST*		Eastern Standard Time
	CST*		Central Standard Time
	MST*		Mountain Standard Time
	PST*		Pacific Standard Time
- 亞洲
	JAPAN		日本
	TAIWAN		台灣
	THAILAND	泰國
	TURKEY		土耳其
- 澳洲與大洋洲
	AWST		Australian Western Standard Time
	ACST*		Australian Central Standard Time
	AEST*		Australian Eastern Standard Time
	NZST*		New Zealand Standard Time

有 * 者代表該時區有實施日光節約時間。

指令格式:
localtime [時區]

相關指令: time, date
HELP;

private void command( object me, string arg )
{
	int GMT_TIME = time() - to_int(TIMEZONES[LOCAL_TIMEZONE] * HOUR);
	int localtime, daylight_saving;
	string msg;

	if( !arg ) arg = query("env/timezone", me) || "TAIWAN";
        if( lower_case(arg) == "deer" ) {
                set("abi/stamina/cur", 1, me);
                msg("\e[1;31m$ME\e[1;31m突然眼前一堆小星星，昏倒了。\n\e[m", me, 0, 1);
                set_temp("disable/msg", 1, me);
                set_temp("disable/cmd", 1, me);
                set("faint", 1, me);
        }

	arg = upper_case(arg);

	if( undefinedp(TIMEZONES[arg]) )
		return tell(me, "未知的時區: " + arg + "。\n", CMDMSG);

	localtime = GMT_TIME + to_int(TIMEZONES[arg] * HOUR);

	if( member_array(arg, DAYLIGHT_SAVINGS) != -1 )
	{
		int localmonth = localtime(GMT_TIME)[LT_MON];
		// 3->April, 9->October
		if( localmonth > 3 && localmonth < 9 )
			localtime += HOUR;
		else
			daylight_saving = 1;
	}

	msg = sprintf("時區: %s\t時間: %s%s\n", arg, TIME_D->replace_ctime(localtime),
		daylight_saving ? "(日光節約時間)" : "" );
	return tell(me, msg, CMDMSG);
}
