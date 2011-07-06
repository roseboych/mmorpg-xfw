/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PROLIB__PRE__H__
#define __PROLIB__PRE__H__

//当前协议版本号
#define PROTOCOL_VERSION	20090803

//protocol define(游戏逻辑协议从2000开始)
#define APP_PRO_BASE 		2000

enum TPRO_APP
{
	APP_PROTOCOL_BASE =APP_PRO_BASE,

	//宠物系统
	APP_PETSUMMON_REQ,
	APP_PETSUMMON_ACK,
	APP_PETSUMMON_NTF,

	//换装
	APP_EQUIPITEM_REQ,
	APP_EQUIPITEM_ACK,
	APP_EQUIPITEM_NTF,

	// 聊天
	APP_TEAMCHAT_REQ,
	APP_TEAMCHAT_NTF,
	APP_P2PCHAT_REQ,
	APP_P2PCHAT_NTF,

	//buffer系统
	//buffer变化通知
	APP_BUFFCHANGE_NTF,

	//技能系统
	//技能数据初始化
	APP_SKILLINIT_NTF,
	//技能状态变化通知
	APP_SKILLSTATECHG_NTF,
	//学习一个技能
	APP_SKILLSTUDY_REQ,
	APP_SKILLSTUDY_ACK,
	//使用技能
	APP_SKILLUSED_REQ,
	APP_SKILLUSED_ACK,

	//玩家数据存储
	//位置面向更新
	DB_POSROTSAVE_NTF,
	//基本数据更新
	DB_PLAYERBASEINFO_SAVE,

	//player状态
	APP_PLAYERMOVE_REQ,
	APP_PLAYERMOVE_ACK,
	APP_PLAYERMOVE_NTF,

};

//predefine const value
//define nickname max length
#define CHR_NICKNAME_LEN	32
#define CHR_PETNAME_LEN		32

#endif	//__PROLIB__PRE__H__
