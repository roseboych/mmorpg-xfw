-- reslib application
-- @category	skill system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- ������

class 's0001_01000001' (skillimplbase)

function s0001_01000001:__init()
	skillimplbase.__init()
end

function s0001_01000001:studyskill( player)
	return 0;
end

function s0001_01000001:useskill( player, rundata)
	return 0;
end

-- ע�Ἴ�ܵ�ע�����
-- ���� ��ʦ [0][1]
_skill_reg[0][1]["01000001"]=s0001_01000001();
