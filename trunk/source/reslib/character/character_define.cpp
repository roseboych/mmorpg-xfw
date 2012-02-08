/**
* character module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
* 定义了和角色相关的常量
*/
#include "character_define.h"

#include <prolib/gen/pro_enum_def.h>

//玩家游戏状态定义
const NS_STL::string g_chrstate_desc[CHRSTATE_MAX] =
{
	"nofightsitdown",
	"nofightstand",
	"nofightwalk",
	"nofightrun",
	"nofightride",
	"nofightfly",
	"fightstand",
	"fightwalk",
	"fightrun",
	"fightride",
	"fightfly",
	"navigatefly",
	"deathstand",
	"deathwalk",
	"deathrun"
};

//玩家种族定义
const NS_STL::string g_chr_race_desc[CHR_RACE_MAX] =
{
	"human"
};

//职业定义
const NS_STL::string g_profession_human_desc[PROFESSION_HUMAN_MAX] =
{
	"soldier",
	"master"
};

