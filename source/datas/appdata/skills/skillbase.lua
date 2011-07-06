-- reslib application
-- @category	skill system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- skill脚本的基类定义

class 'skillimplbase'
	
function skillimplbase:__init()
end

-- 学习技能,完成条件判定和某些属性值的扣除
-- @param player	当前玩家
-- @return 0:成功 <0:不通的失败条件
function skillimplbase:studyskill( player)
	return -1;
end

-- 使用技能
-- @param player	当前玩家
-- @param rundata	技能数据包
function skillimplbase:useskill( player, rundata)
	return 0;
end

-- 非瞬发技能的tick入口函数
-- @param player	当前玩家
-- @param rundata	运行时数据
-- @param tnow		当前时间
-- @return	0:正常执行 1:skill结束
function skillimplbase:process( player, rundata, tnow)
	return 1;
end

-- 技能类命名规范
-- sxxxx_xxxxxxxx
-- 所有实现类:s字母+2位十六进制表示的种族+2位十六进制表示的职业+字母'_'分割+8位16进制表示的技能编号

-- 用来注册所有支持的技能实现类
_skill_reg ={}

-- 人族
_skill_reg[0] ={}
------ 战士
_skill_reg[0][0] ={}
------ 法师
_skill_reg[0][1] ={}

------------------------------------------------------------定义了对skillimplbase类的公共函数的封装---------------------------------------------
-- 对studyskill的封装
-- @param skid		技能id
-- @param player	
-- @return 0:成功 -1:技能未注册
function skill_study( skid, player)
	local sk =_skill_reg[player.race][player.profession][skid];
	
	if sk == nil then return 1 end;
	
	return sk:studyskill( player);
end

--对useskill的封装
-- @param skid
-- @param player
-- @param rundata
function skill_use( skid, player, rundata)
	local sk =_skill_reg[player.race][player.profession][skid];
	
	if sk == nil then return -1 end;
	
	return sk:useskill( player, rundata);
end

function skill_process( skid, player, rundata, tnow)
	local sk =_skill_reg[player.race][player.profession][skid];
	
	if sk == nil then return -1 end;
	
	return sk:process( player, rundata, tnow);
end
