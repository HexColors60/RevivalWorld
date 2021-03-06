/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : call.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-4
 * Note   : call 
 * Update :
 *  o 2002-08-14 Clode 穝糤 Database 单 efun  call..
 *
 -----------------------------------------
 */

#include <feature.h>
#include <object.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	夹非 call 
HELP;

private void command(object me, string arg)
{
	int i, size, not_efun;
	object env, ob;
	string obname, func, param;
	mixed *args, result;
	
	if( !is_command() ) return;
	
	if( !arg || sscanf(arg, "%s->%s(%s)", obname, func, param) != 3 )
		return tell(me, "Αcall <ン>-><ㄧ计>( <把计>, ... )\n", CMDMSG);
	
	env = environment(me);

	switch(obname)
	{
		case "me":	
			ob = me;
			break;
		case "here": 	
			ob = env;
			break;
		default:	
			ob = present(obname, env) || present(obname, me) || find_player(obname) || find_object(resolve_path(me, obname));
			break;
	}
	
	if( !objectp(ob) ) return tell(me, "тぃ﹚ン\n", CMDMSG);
	
	size = sizeof((args = explode(param, ",")));

	for(i=0; i<size; i++) {
		args[i] = remove_fringe_blanks(args[i]);
		if( sscanf(args[i], "%d", args[i]) ) continue;
		if( sscanf(args[i], "\"%s\"", args[i]) ) continue;
	}
	
	switch(func)
	{
		case "set":
			if( sizeof(args) != 2 || !stringp(args[0]) )
				return tell(me, "Efun set() 把计计秖┪篈岿粇\n", CMDMSG);
			result = set(args[0], args[1], ob);
			break;
			
		case "set_temp":
			if( sizeof(args) != 2 || !stringp(args[0]) )
				return tell(me, "Efun set_temp() 把计计秖┪篈岿粇\n", CMDMSG);
			result = set_temp(args[0], args[1], ob);
			break;
		
		case "query":
			if( sizeof(args) != 1 || !stringp(args[0]) )
				return tell(me, "Efun query() 把计计秖┪篈岿粇\n", CMDMSG);
			result = query(args[0], ob);
			break;
		
		case "query_temp":
			if( sizeof(args) != 1 || !stringp(args[0]) )
				return tell(me, "Efun query_temp() 把计计秖┪篈岿粇\n", CMDMSG);
			result = query_temp(args[0], ob);
			break;	
			
		case "addn":
			if( sizeof(args) != 2 || !stringp(args[0]) )
				return tell(me, "Efun addn() 把计计秖┪篈岿粇\n", CMDMSG);
			result = addn(args[0], args[1], ob);
			break;
			
		case "addn_temp":
			if( sizeof(args) != 2 || !stringp(args[0]) )
				return tell(me, "Efun addn_temp() 把计计秖┪篈岿粇\n", CMDMSG);
			result = addn_temp(args[0], args[1], ob);
			break;		

		case "delete":
			if( sizeof(args) != 1 || !stringp(args[0]) )
				return tell(me, "Efun delete() 把计计秖┪篈岿粇\n", CMDMSG);
			result = delete(args[0], ob);
			break;
		
		case "delete_temp":
			if( sizeof(args) != 1 || !stringp(args[0]) )
				return tell(me, "Efun delete_temp() 把计计秖┪篈岿粇\n", CMDMSG);
			result = delete_temp(args[0], ob);
			break;
		
		default:
			if( !function_exists(func, ob) )
				return tell(me, ob->query_idname()+"ō⊿Τ "+func+"() ㄧΑ\n", CMDMSG);
				
			not_efun = 1;
			result = call_other(ob, ({ func }) + args);
	}
	
	for(i=0; i<size; i++) args[i] = sprintf("%O",args[i]);

	param = implode(args, ", ");
	
	printf("%O->%s(%s) = %O\n", ob, func, param, result);
	  
	msg("$ME磅︽㊣"+(ob->query_idname()||file_name(ob))+" "+func+"("+param+") "+(not_efun?"":"External Function ")+"ㄧ计\n", me, 0, 1, CMDMSG);
}