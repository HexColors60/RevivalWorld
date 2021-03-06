/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lily.c
 * Author : Tzj@RevivalWorld
 * Date   : 2003-05-30
 * Note   : 百合花
 * Update :
 *  o 2000-05-30
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("lily", "百合花");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "百合花花語：純潔•純潔的心•百年好合•偉大的愛 。\n");
        set("unit", "朵");
        set("mass", 5);
        set("value", 800);
}