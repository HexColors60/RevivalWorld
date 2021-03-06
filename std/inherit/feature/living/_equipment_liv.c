/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _equipment_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   :
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <equipment.h>

private nosave mapping equipments = allocate_mapping(0);

//
// 回傳所有裝備資訊
//
mapping query_equipments()
{
	return equipments;
}



//
// 回傳所有裝備物件
//
object *query_equipment_objects()
{
	return implode(values(equipments), (: $1+$2 :)) || allocate(0);
}



//
// 1: 此物件不是裝備
// 2: 無法裝備在此物件上
// 3: 不知此物件該裝備在何處
// 4: 已經有同種類的裝備
// 5: 已經裝備在其他的部位上
//
int equip(object ob, int ref status)
{
	array partset;
	int partamount;
	string partid;
	

	// 此物件不是裝備
	if( !ob->is_equipment() )
	{
		status = 1;
		return 0;	
	}

	// 無法裝備在此物件上
	if( !ob->valid_equip(this_object()) )
	{
		status = 2;
		return 0;
	}

	// 不知此物件該裝備在何處
	if( !arrayp(partset = ob->query_part_set()) )
	{
		status = 3;
		return 0;
	}

	partid = partset[PART_ID];
	partamount = partset[PART_AMOUNT];
	
	// 已經無法再裝備更多同種類的裝備
	if( !undefinedp(equipments[partid]) && sizeof(equipments[partid]) >= partamount )
	{
		status = 4;
		return 0;
	}

	foreach(string p, object *eqs in equipments)
	{
		if( member_array(ob, eqs) != -1 )
		{
			status = 5;
			return 0;
		}
	}
	
	if( undefinedp(equipments[partid]) )
		equipments[partid] = allocate(0);
	
	equipments[partid] |= ({ ob });

	ob->set_equipping(this_object());

	return 1;
}


//
// 解除某個物件裝備
//
// 1: 並無裝備此物件
// 2: 無法解除此項裝備
//
int unequip(object ob, int ref status)
{
	foreach(string partid, object *eqs in equipments)
	{
		if( member_array(ob, eqs) != -1 )
		{
			// 無法解除此項裝備
			if( !ob->valid_unequip(this_object()) )
			{
				status = 2;
				return 0;
			}
			
			ob->delete_equipping(this_object());
			
			equipments[partid] -= ({ ob });
			
			if( !sizeof(equipments[partid]) )
				map_delete(equipments, partid);

			return 1;
		}
	}

	// 並無裝備此物件
	status = 1;
	return 0;
}



//
// 解除所有裝備
//
object *unequip_all()
{
	object ob;
	object *all_equipments = allocate(0);
	
	foreach(string partid, object *eqs in equipments)
	{
		foreach(ob in eqs)
			ob->delete_equipping(this_object());
		
		all_equipments += eqs;
	}
	
	equipments = allocate_mapping(0);
	
	return all_equipments;
}



//
// 解除某個部位的所有裝備
//
// 1: 此部位並無任何裝備
// 2: 無法解除此項裝備
//
object *unequip_part(string partid)
{
	object *unequipped_equipments = allocate(0);

	if( undefinedp(equipments[partid]) )
		return 0;
	
	foreach( object ob in equipments[partid] )
	{
		// 無法解除這項裝備
		if( !ob->valid_unequip(this_object()) )
			continue;
	
		ob->delete_equipping(this_object());
		
		unequipped_equipments |= ({ ob });
	}
	
	map_delete(equipments, partid);
	
	return unequipped_equipments;
}



//
// 檢查此物件是否正在裝備
//
int is_equipping_object(object ob)
{
	foreach(string part, object *eqs in equipments)
		if( member_array(ob, eqs) != -1 )
			return 1;
	
	return 0;
}


//
// 檢查某部位是否有裝備
//
int is_equipping_part(string part)
{
	if( !undefinedp(equipments[part]) )
		return 1;

	return 0;
}



//
// 回傳某部位的裝備物件
//
object *query_equipping_object(string partid)
{
	if( !undefinedp(equipments[partid]) )
		return equipments[partid];

	return 0;
}



//
// 回傳某裝備的部位
//
string query_equipping_part(object ob)
{
	foreach(string part, object *eqs in equipments)
		if( member_array(ob, eqs) != -1 )
			return part;
	return 0;
}


