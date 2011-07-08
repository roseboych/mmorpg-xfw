-- reslib application
-- @category	skill system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- 地刺

class 's0001_01000004' (skillimplbase)

function s0001_01000004:__init()
	skillimplbase.__init()
end

function s0001_01000004:studyskill( player)
	return 0;
end

function s0001_01000004:useskill( player, rundata)
	return 0;
end

function s0001_01000004:process( player, rundata, tnow)
	return 1;
end

-- 注册技能到注册表中
-- 人族 法师 [0][1]
_skill_reg[0][1]["01000004"]=s0001_01000004();
