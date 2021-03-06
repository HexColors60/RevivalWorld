/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _attribution.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-12
 * Note   : 屬性繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
#include <message.h>
#include <buff.h>

//
// 裝備物件的 buff
//
int query_equipment_buff(string key)
{
	int buff = 0;
	
	foreach(object ob in this_object()->query_equipment_objects())
		buff += query(key, ob);
	
	return buff;
}

//
// 角色狀態的 buff
//
int query_condition_buff(string key)
{
	int buff = 0;
	
	mapping condition = query("condition");

	if( mapp(condition) )
	foreach(string c, mapping data in condition)
		buff += query("condition/"+c+"/"+key);
	
	return buff;
}

//
// 所有加成總數
//
int query_all_buff(string key)
{
	return query_equipment_buff(key) + query_condition_buff(key);
}

//
// 五大屬性
//
int query_str()
{
	return query("attr/str") + query_all_buff(BUFF_STR);
}

int query_phy()
{
	return query("attr/phy") + query_all_buff(BUFF_PHY);
}

int query_int()
{
	return query("attr/int") + query_all_buff(BUFF_INT);
}

int query_agi()
{
	return query("attr/agi") + query_all_buff(BUFF_AGI);
}

int query_cha() 
{
	return query("attr/cha") + query_all_buff(BUFF_CHA);
}


//
// 三大數值最大值
//
int query_stamina_max()
{
	return query("abi/stamina/max") + query_all_buff(BUFF_STAMINA_MAX);
}
int query_health_max()
{
	return query("abi/health/max") + query_all_buff(BUFF_HEALTH_MAX);
}
int query_energy_max()
{
	return query("abi/energy/max") + query_all_buff(BUFF_ENERGY_MAX);
}


//
// 三大數值目前值
//
int query_stamina_cur() { return query("abi/stamina/cur"); }
int query_health_cur() { return query("abi/health/cur"); }
int query_energy_cur() { return query("abi/energy/cur"); }


//
// 將三大數值目前值設定為最大值
//
int set_stamina_full() { return set("abi/stamina/cur", query_stamina_max()); }
int set_health_full() { return set("abi/health/cur", query_health_max()); }
int set_energy_full() { return set("abi/energy/cur", query_energy_max()); }
void set_all_ability_full()
{
	set_stamina_full();
	set_health_full();
	set_energy_full();
}

//
// 將三大數值目前值設定為最小值
//
int set_stamina_zero() { return set("abi/stamina/cur", 0); }
int set_health_zero() {	return set("abi/health/cur", 0); }
int set_energy_zero() {	return set("abi/energy/cur", 0); }
void set_all_ability_zero()
{
	set_stamina_zero();
	set_health_zero();
	set_energy_zero();
}

//
// 三大數值之恢復速度
//
int stamina_regen()
{
	return query_str() + query_temp("rest_regen/stamina") + query_all_buff(BUFF_STAMINA_REGEN);
}
int health_regen()
{
	return query_phy() + query_temp("rest_regen/health") + query_all_buff(BUFF_HEALTH_REGEN);
}
int energy_regen()
{
	return query_int() + query_temp("rest_regen/energy") + query_all_buff(BUFF_ENERGY_REGEN);
}



//
// 檢查三大數值是否全滿
//
int is_stamina_full() { return query_stamina_cur() >= query_stamina_max(); }
int is_health_full() { return query_health_cur() >= query_health_max();	}
int is_energy_full() { return query_energy_cur() >= query_energy_max();	}
int is_all_ability_full()
{
	return is_stamina_full() && is_health_full() && is_energy_full();
}


//
// 變動五大屬性
//
int add_str(int i) { return addn("attr/str", i); }
int add_phy(int i) { return addn("attr/phy", i); }
int add_int(int i) { return addn("attr/int", i); }
int add_agi(int i) { return addn("attr/agi", i); }
int add_cha(int i) { return addn("attr/cha", i); }


//
// 五大屬性外加 buff
//
int add_temp_str(int i) { addn("attr/temp/str", i); }
int add_temp_phy(int i) { addn("attr/temp/phy", i); }
int add_temp_int(int i) { addn("attr/temp/int", i); }
int add_temp_agi(int i) { addn("attr/temp/agi", i); }
int add_temp_cha(int i) { addn("attr/temp/cha", i); }


//
// 食物狀態
//
int query_food_max()
{
	return query("stat/food/max") + query_all_buff(BUFF_FOOD_MAX);
}

int query_food_cur()
{
	return query("stat/food/cur");
}

void cost_food(int i)
{
	if( addn("stat/food/cur", -i) < 0 )
		set("stat/food/cur", 0);
}

int add_food(int i)
{
	if( query_food_cur() + i > query_food_max() )
		return 0;
	
	return addn("stat/food/cur", i);
}

//
// 飲水狀態
//

int query_drink_max()
{
	return query("stat/drink/max") + query_all_buff(BUFF_DRINK_MAX);
}

int query_drink_cur()
{
	return query("stat/drink/cur");
}

int cost_drink(int i)
{
	if( addn("stat/drink/cur", -i) < 0 )
		set("stat/drink/cur", 0);
}

int add_drink(int i)
{
	if( query_drink_cur() + i > query_drink_max() )
		return 0;
	
	return addn("stat/drink/cur", i);
}

//
// 社會經驗運算
//
string query_social_exp_cur() { return query("exp/social/cur"); }
string query_social_exp_tot() { return query("exp/social/tot"); }
string add_social_exp(int exp)
{
	float fexp = to_float(exp);
	string cur = query_social_exp_cur();
	string tot = query_social_exp_tot();

	if( fexp <= 0 ) return 0;

	// 大於 5000 萬則減少經驗值收入機會
	if( random(to_int(count(cur, "/", 50000000))+1) ) return 0;

	// 輔助裝備法術加成
	fexp += fexp*(query_int()*2-20+query_all_buff(BUFF_SOCIAL_EXP_BONUS))/100.;

	// 總經驗值每 10 億降 1 倍經驗值
	fexp /= to_int(count(tot, "/", 1000000000))+1;

	exp = ceil(fexp);

	set("exp/social/tot", copy(count(exp, "+", tot)));

	return set("exp/social/cur", copy(count(exp, "+", cur)));
}

string cost_social_exp(int exp)
{
	string newexp, oldexp = query("exp/social/cur");
	
	if( exp <= 0 ) return 0;
	
	newexp = count(oldexp, "-", exp);
	
	if( count(newexp, "<", 0) )
		return 0;
	
	set("exp/social/cur", newexp);
	
	return newexp;
}

//
// 體力運算
//
int cost_stamina(int str)
{
	int curstr = query_stamina_cur();
	
	if( curstr < str ) return 0;
	
	addn("abi/stamina/cur", -str);

	return 1;	
}

void earn_stamina(int str)
{
	int curstr = query_stamina_cur();
	int maxstr = query_stamina_max();
	
	if( curstr == maxstr ) return;
	
	if( curstr + str >= maxstr )
		set_stamina_full();
	else
		addn("abi/stamina/cur", str);
}


//
// 生命運算
//
int cost_health(int hp)
{
	int curhp = query_health_cur();
	
	if( curhp < hp )
	{
		this_object()->faint();
		return 0;
	}

	addn("abi/health/cur", -hp);	
	return 1;
}
void earn_health(int hp)
{
	int curhp = query_health_cur();
	int maxhp = query_health_max();
	
	if( curhp == maxhp ) return;
	
	if( curhp + hp >= maxhp )
		set_health_full();
	else
		addn("abi/health/cur", hp);
}



//
// 精神運算
//
int cost_energy(int ep)
{
	int curep = query_energy_cur();
	
	if( curep < ep ) return 0;
	
	addn("abi/energy/cur", -ep);

	return 1;	
}

void earn_energy(int ep)
{
	int curep = query_energy_cur();
	int maxep = query_energy_max();
	
	if( curep == maxep ) return;
	
	if( curep + ep >= maxep )
		set_energy_full();
	else
		addn("abi/energy/cur", ep);
}
