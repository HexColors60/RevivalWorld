/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : newspaper.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : 報紙
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <mudlib.h>
#include <message.h>
#include <condition.h>

#define TIME 		0
#define MESSAGE		1

inherit STANDARD_OBJECT;

mapping actions;

string help = @HELP
readpaper
readpaper <編號>	查閱指定的新聞篇幅
readpaper <版面>	列出所有該版面可以檢閱的新聞
readpaper <版面> <編號>	列出所有該版面指定編號的新聞篇幅
HELP;

int alphabet_count(string arg)
{
	int n;
	for(int i=0; i<strlen(arg); i++)
	{
		if( arg[i] > (2 << 6) )
		{
			i += 1;
			continue;
		}
		if( arg[i] >= 32 && arg[i] <= 126 ) n++;
	}
	return n;
}

string string_wrap(string arg, int i)
{
	if( strlen(arg) <= i ) return arg;
	i = i - 3;
	if( !(i%2) && alphabet_count(arg[0..i])%2 ) i--;
	while(arg[i] > 128) i--;
	return arg[0..i] + "...";
}

string paper_style(string content)
{
	string *lines;
	string msg = @HEAD
╭──────────────────────────────────────╮
｜ %-45s%30s｜
｜ %-75s｜
├──────────────────────────────────────┤
HEAD;

	msg = sprintf(msg,
		MUD_FULL_NAME"電子報", 
       		"訂閱數 "+HIG+(sizeof(children(base_name(this_object())))-1)+NOR" 份  ",
       		TIME_D->game_time_description());
	lines = explode(content, "\n");
	foreach(string line in lines)
	{
		msg += sprintf("｜ %-75s｜\n", line);
	}

	msg += @TAIL
╰──────────────────────────────────────╯
TAIL;

	return msg;
}

void do_readpaper(object me, string arg)
{
	int i;
	array newsd_news, real_news;
	string section, tnews="", onews="", rnews="", pop_product = "";
	string type, msg = @PAPER
╭──────────────────────────────────────╮
｜ %-45s%30s｜
｜ %-75s｜
├──────────────────────────────────────┤
%s
╰──────────────────────────────────────╯
PAPER;

	type = query("env/newspaper", me);
	if( !type ) type = "bahamut";
	if( arg && (sscanf(arg, "%s %d", type, i) == 2 || sscanf(arg, "%d", i)) )
	{
		string lines;

		real_news = REALNEWS_D->query_news_titles(type);
		if( i < 1 || i > sizeof(real_news) )
			return tell(me, "沒有這個新聞篇幅。\n");

		lines = sprintf("\n\n%s\n\n%s\n",
			HIW + real_news[i-1] + NOR,
			multiwrap(REALNEWS_D->query_news_article(type, i-1), 72));

		me->more(paper_style(lines));

       		if( !me->in_condition(KNOWLEDGEBLESS) )
       			me->start_condition(KNOWLEDGEBLESS);
		return;
	}
	if( arg && sscanf(arg, "%s", type) )
	{
		real_news = REALNEWS_D->query_news_titles(type);
		
		if( !pointerp(real_news) || !sizeof(real_news) )
			 return tell(me, "沒有這個版面的新聞。\n");
	       	foreach(string news in real_news )
	       	{
	       		string time;
	       		int wrap_size;
	       		sscanf(news, "%s (%s)", news, time);
	       		
	       		wrap_size = 68-strlen(time);
	       		news = string_wrap(news, wrap_size);
	       		news = sprintf(" %2d %-"+wrap_size+"s %s", ++i, news, "- " + time);
	       		rnews += NOR WHT+news+NOR"\n";
	       	}
	       	me->more(paper_style(rnews));

       		if( !me->in_condition(KNOWLEDGEBLESS) )
       			me->start_condition(KNOWLEDGEBLESS);
		return;
	}

	/* News_D news */
	newsd_news = NEWS_D->query_news();
	reverse_array(ref newsd_news);
       	foreach(array news in newsd_news)
       	{
       		i = 0;
       		if( time() - news[TIME] > 60*60*24 )
       		{
       			foreach( section in explode(cwrap(news[MESSAGE], 60, 0), "\n") )
       				onews += "  "CYN+(!i++ ? TIME_D->replace_ctime(news[TIME])[0..4] :"     ")+" "+section+NOR"\n";;
       		}
       		else
       			foreach( section in explode(cwrap(news[MESSAGE], 60, 0), "\n") )
       				tnews += "  "NOR CYN+(!i++ ? TIME_D->replace_ctime(news[TIME])[0..4] :"     ")+" "+section+NOR"\n";
       	}
       	
       	/* Popular Product */
       	if( SHOPPING_D->query_pop_product() )
		pop_product = "  "MAG+"熱門商品 "+load_object(SHOPPING_D->query_pop_product())->query_idname()+NOR"\n";

	/* Realnews_D news */
	real_news = REALNEWS_D->query_news_titles(type);
       	real_news = real_news[0..7];
       	i = 0;
       	foreach( string news in real_news )
       	{
       		string time;
       		sscanf(news, "%s (%s)", news, time);
       		if( strlen(news) > 42 )
       			news = news[0..37 + (alphabet_count(news[0..37])%2)] + "...";
       			
       		news = sprintf(" %2d %-42s%29s", ++i, news, "- " + time);
       		rnews += NOR WHT+news+NOR"\n";
       	}
       	if( onews == "" ) onews = "  無\n";
       	if( tnews == "" ) tnews = "  無\n";
       	if( rnews == "" ) rnews = "  無\n";
	if( pop_product == "" ) pop_product = "  無\n";

       	msg  = HIY"頭條"NOR YEL"新聞"NOR"\n" + tnews + "\n";
       	msg += HIC"舊聞"NOR CYN"軼事"NOR"\n" + onews + "\n";
       	msg += HIM"流行"NOR MAG"特刊"NOR"\n" + pop_product + "\n";
       	msg += HIW"即時"NOR WHT"新聞"NOR"\n" + rnews + "\n";
       	msg += "目前新聞版面有: thg, bahamut\n";
       	msg += "使用說明請查閱 help newspaper\n";
       	me->more(paper_style(msg));

       	if( !me->in_condition(KNOWLEDGEBLESS) )
       		me->start_condition(KNOWLEDGEBLESS);
}


void create()
{
	set_idname("newspaper", MUD_CHINESE_NAME+"電子報");
	
	if( clonep() ) 
		set("shadow_ob", find_object(base_name(this_object())));
	else
	{
		set("unit", "份");
		set("value", 500);
		set("mass", 2);
		set("badsell", 1);
	}
	
	actions = ([ "readpaper" : (: do_readpaper :) ]);
}
