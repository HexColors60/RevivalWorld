/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : whois_d.c
 * Author : sinji@Revival World
 * Date   : 2000-00-00
 * Note   : whois 精靈
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <socket.h>
#include <socket_err.h>
#include <message.h>

string strs="";

// string simplex; /* 查詢的單字 */
// object tu;/* 把訊息傳給user看到 */
mapping connects = allocate_mapping(0);
private void get_data(int);

void create() { }

void find_ip(object user, string word)
{
	int err,fd;

	if(!user) return ;
	if(!word) return ;        

	/* 開啟網路連結 */
	fd = socket_create(STREAM, "read_callback","close_socket");

	if ( fd < 0 )
	{
		return tell( user, HIR"查詢失敗, 請稍後在試。\n"NOR, CMDMSG);             
	}

	err = socket_connect(fd, "210.17.9.228 80","read_callback","write_callback" );

	if( err!=EESUCCESS )
	{
		return tell( user, HIR"查詢失敗, 請稍後在試。\n"NOR, CMDMSG);     
	}

	connects[fd] = allocate_mapping(3);
	connects[fd]["data"] = "";
	connects[fd]["user"] = user;
	connects[fd]["simplex"] = word;

	return ;
}

int close_socket(int fd)
{
	socket_close(fd);
	get_data(fd);
}

void read_callback(int fd, mixed message)
{
	/* 把每次取得的資料存進 strs 裡 */
	connects[fd]["data"] += message;
	return ;
}

void write_callback(int fd)
{
	string code="";
	code = connects[fd]["simplex"];
	/* 輸入指令抓取該頁內容 */
	socket_write(fd,
	    sprintf("GET /whois.cgi?sld=IP&query=%s\n\n", code));
	return ;
}

private void get_data(int fd)
{
        connects[fd]["user"]->more(connects[fd]["data"] + "\n");
	return ;
}

string query_name()
{
	return "WHOIS 系統(WHOIS_D)";
}
