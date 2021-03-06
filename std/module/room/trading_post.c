/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trading_post.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-13
 * Note   : 自由買賣所動作繼承物件
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_list";
inherit __DIR__"inherit/_action_buy";
inherit __DIR__"inherit/_action_sell";

#define TRADING_POST_CAPACITY	-1

nosave mapping allowable_trade = 
([

	"農作":
		({
			"/obj/farming/seed/rice_seedling", "-1",
			"/obj/farming/seed/herbage_seed", "-1",
			"/obj/farming/seed/barley_seed", "-1",
			"/obj/farming/seed/corn_seed", "-1",
			"/obj/farming/seed/rose_seed", "-1",
			"/obj/farming/seed/sunflower_seed", "-1",
			"/obj/farming/seed/lily_seed", "-1",
		}),
	"牧場":
		({
			"/obj/pasture/breed/chicken_stud", "-1",
			"/obj/pasture/breed/pig_stud", "-1",
			"/obj/pasture/breed/sheep_stud", "-1",
			"/obj/pasture/breed/horse_stud", "-1",
			"/obj/pasture/breed/cattle_stud", "-1",
			"/obj/pasture/breed/deer_stud", "-1",
		}),
	"養殖":
		({
			"/obj/fishfarm/breed/clam", "-1",
			"/obj/fishfarm/breed/young_crab", "-1",
			"/obj/fishfarm/breed/young_shrimp", "-1",
		}),
	"釣魚":
		({
			"/obj/fishing/rod/toy_rod", "-1",
			"/obj/fishing/rod/bamboo_rod", "-1",
			"/obj/fishing/rod/wood_rod", "-1",
			"/obj/fishing/rod/plastic_rod", "-1",
			"/obj/fishing/rod/carbon_fiber_rod", "-1",
			"/obj/fishing/rod/microelectronic_rod", "-1",
			"/obj/fishing/bait/bread_bug", "-1",
			"/obj/fishing/bait/prawn", "-1",
			"/obj/fishing/bait/tsou_chip", "-1",
			"/obj/fishing/bait/worm", "-1",
		}),
	"採集":
		({
			"/obj/collection/tool/coarse_pickax", "-1",
			"/obj/collection/tool/coarse_drill", "-1",
			"/obj/collection/tool/coarse_bucket", "-1",
			"/obj/collection/tool/coarse_saw", "-1",
			"/obj/collection/tool/coarse_shovel", "-1",
			
		}),
	"其他":
		({
                        "/obj/etc/newspaper", "-1",
                        "/obj/etc/firework", "-1",
                        "/obj/systemproducts/flatbed_tricycle", "-1",
		}),

]);

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIW"買賣大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    買賣大廳是玩家購買與販賣商品的地方，由於是由政府開設，僅會以五成價格向玩家收購商品。
HELP,

"list":
@HELP
列出所有販賣中的商品的指令，用法如下：
  list			- 列出所有販賣中的商品
HELP,

"buy":
@HELP
購買商品的指令，用法如下：
  buy all rice			- 買下所有的 rice 商品
  buy 3 rice			- 買下 3 件 rice 商品
  buy rice			- 買下 1 件 rice 商品
  buy 3 2			- 買下 3 件編號為 2 的商品
  buy 2				- 買下 1 件編號為 2 的商品
  buy rice to center 1 51,22	- 買下 1 件 rice 商品並送達 center 城市第 1 衛星都市座標(51,22)處的建築物
HELP,

"sell":
@HELP
販賣商品的指令，用法如下：
  sell all		- 賣出所有身上可販賣的商品
  sell all rice		- 賣出所有的 rice 商品
  sell 3 rice		- 賣出 3 件 rice 商品
HELP,
			]),
		"action":
			([
				//"sell"		: (: do_trading_post_sell($1, $2, 50) :),
				"list"		: (: do_list($1, $2, "products", TRADING_POST_CAPACITY) :),
				"buy"		: (: do_buy($1, $2, "products", "GOVERNMENT") :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	NOR YEL"物流中心"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// 開張儀式費用
	,"5000000"
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,10
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
