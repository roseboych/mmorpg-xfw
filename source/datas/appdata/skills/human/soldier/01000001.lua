-- reslib application
-- @category	skill system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- 近身攻击

class 's0000_01000001' (skillimplbase)

function s0000_01000001:__init()
	skillimplbase.__init()
end

function s0000_01000001:studyskill( player)
	return 0;
end

function s0000_01000001:useskill( player, rundata)
	return 0;
end

-- 注册技能到注册表中
-- 战士_skill_reg[0][0]
_skill_reg[0][0]["01000001"]=s0000_01000001();
