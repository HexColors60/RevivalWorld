/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : file.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-07-05
 * Note   : included by Master Object
 * Update :
 *  o 2002-00-00

 -----------------------------------------
 */

/* 判斷一個指定物件所屬的區域 */ //不知道能做啥 by Clode
string domain_file(string str) {}

/* 判斷一個指定物件的作者為誰 */
/* 需定義 PACKAGE_MUDLIB_STATS  */
string author_file(string str) {} //也是不知道能做啥..難道用來弄chown? :p by Clode

/* 為新創造的物件指定一個隱私字串 */
/* 需定義 PRIVS */
string privs_file(string filename) {
    return filename;
}
