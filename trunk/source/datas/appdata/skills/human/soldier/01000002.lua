-- reslib application
-- @category	skill system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- 冰霜护盾

class 's0000_01000002' (skillimplbase)

function s0000_01000002:__init()
	skillimplbase.__init()
end

function s0000_01000002:studyskill( player)
	return 0;
end

function s0000_01000002:useskill( player, rundata)
	return 0;
end

-- 注册技能到注册表中
-- 人族 战士 _skill_reg[0][0]
_skill_reg[0][0]["01000002"]=s0000_01000002();
