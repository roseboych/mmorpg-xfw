/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DBACTION_DEF__H__
#define __DBACTION_DEF__H__

//存储过程定义
//获取玩家角色列表
#define DBPROC_CHRGETLIST	"{call getchrlist(:uid<int,in>,:rid<short,in>)}"
//获取角色avatar列表
#define DBPROC_CHRGETAVLIST	"{call getchravlist(:uid<int,in>,:rid<short,in>)}"
//创建玩家角色
#define DBPROC_CHRADDINFO	"{call addchrinfo(:uid<int,in>,:rid<short,in>,:n<char[20],in>,:rc<short,in>,:fp<short,in>,:sx<short,in>,:hpv<int,in>,:mpv<int,in>,:expv<int,in>,:mv<int,in>,:posx<float,in>,:posy<float,in>,:posz<float,in>,:fc<float,in>,:power<int,in>,:agile<int,in>,:endurance<int,in>,:intellect<int,in>,:spirit<int,in>,:armor<int,in>,:bags<short,in>,:sk01<int,in>,:sk02<int,in>)}"
//删除玩家角色
#define DBPROC_CHRDEL		"{call delchrdata(:cid<int,in>)}"
//获取角色信息
#define DBPROC_CHRGETINFO	"{call getchrinfo(:cid<int,in>)}"
//更新玩家基本信息
#define DBPROC_CHRUPDATE	"{call chrupdate(:cid<int,in>,:moneysv<int,in>,:levelsv<int,in>,:bagslotsv<short,in>,:skill01v<int,in>,:skill02v<int,in>,:hpv<int,in>,:mpv<int,in>,:posx<float,in>,:posy<float,in>,:posz<float,in>,:face<float,in>,:diedv<short,in>,:expv<int,in>)}"
//获取玩家装备信息
#define DBPROC_ITEMGETCHR	"{call getchritems(:cid<int,in>)}"
//增加一个装备
#define DBPROC_ITEMADD		"{call addchritem(:cid<int,in>,:inp<short,in>,:icode<int,in>,:vtype<short,in>,:stime<int,in>,:etime<int,in>)}"
//道具装备操作
#define DBPROC_ITEMOPER		"{call itemoperator(:id<int,in>,:dot<short,in>,:pt<short,in>)}"
//删除一个装备
#define DBPROC_ITEMDEL		"{call delchritem(:id<int,in>)}"
//更新装备信息
#define DBPROC_ITEMUPDATE	"{call chgchritem(:id<int,in>,:st<int,in>,:et<int,in>)}"
//获取玩家宠物列表
#define DBPROC_PETGETLIST	"{call getchrpetdata(:id<int,in>)}"
//召唤或者收起宠物
#define DBPROC_CALLPET		"{call callchrpet(:cid<int,in>,:pid<int,in>,:dtype<short,in>)}"
//获取玩家buffer列表
#define DBPROC_BUFFERLIST	"{call getbuffers(:id<int,in>)}"

//定义的action类型
#define DBACTION_CHRGETLIST		1
#define DBACTION_CHRADDINFO		2
#define DBACTION_CHRDEL			3
#define DBACTION_CHRGETINFO		4
#define DBACTION_ITEMADD		5
#define DBACTION_ITEMOPER		6
#define DBACTION_ITEMDEL		7
#define DBACTION_ITEMUPDATE		8
#define DBACTION_CHRUPDATEINFO	9

#endif	//__DBACTION_DEF__H__
