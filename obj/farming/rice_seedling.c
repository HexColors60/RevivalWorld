/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice seedlings.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-20
 * Note   : 稻秧
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("rice seedling","稻秧");

	if( this_object()->set_shadow_ob() ) return;

	set("unit", "株");
	set("mass", 10);	// 重量
	set("value", 1);	// 價錢
	set("badsell", 1);

	// 種子資訊
	set("seed_data",
		([
			//插秧播種, 灌溉, 收割所耗體力設定
			"str_cost": ([
				"sow"		:5, //插秧或播種時需要的體力
				"irrigate"	:10, //灌溉每升水需要的體力 
				"reap"		:10 //收割時需要的體力
				]),

			//種子種類
			"seed":"稻米",

			//設定插秧或是播種
			"sow_type":"插秧",

			//收割產品的數量設定與產品路徑
			"reap":({ 15, "/obj/farming/rice.c"}),

			//各個步驟的農作訊息
			"msg":
			([
				// 插秧播種時的訊息
				"sow":
					"稻秧整齊地插滿整片田地，一片欣欣向榮的感覺。\n" ,

				// 灌溉時的灌溉次數與訊息
				"irrigate":
				({
					1,"稻田上小小的幼苗已經漸漸長大了。\n",
					2,"只見稻田一片綠意盎然，充滿了生氣。\n",
					3,"稻田上的稻米已經漸漸成熟了。\n",
					5,"只見一整片金黃色的稻田，已經不用再灌溉了，耐心等一陣子便可以收割。\n",
				}),

				// 成熟收割時等待秒數與訊息
				"reap":
				({
					20,
					"這片農田上的稻米已經全部成熟了，終於可以收割了。\n",
					"這片農田經過多日來的灌溉，終於成熟收割了。\n共收割了$NUM袋的稻米。\n"
				}),

				// 荒廢時的秒數與訊息
				"neglect":
				({	
					300,
					"這片農田由於多日來缺乏灌溉，上面的稻苗漸漸枯萎了。\n",
					"這片農田由於多日來缺乏灌溉，已經完全荒廢了。\n",
				}),

				// 天氣影響敘述
				"weather":
				([
					"RAINY":"連日來的雨量使得一些稻苗泡水腐爛了。\n",
					"SNOWY":"大雪連連嚴重使得稻苗們陸續地枯萎了。\n",
					"WINDY":"狂風吹襲使得部份稻苗被強風攔腰折斷。\n",
				]),
			]),
		])
	);
}