-- reslib application
-- @category	skill system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- skill�ű��Ļ��ඨ��

class 'skillimplbase'
	
function skillimplbase:__init()
end

-- ѧϰ����,��������ж���ĳЩ����ֵ�Ŀ۳�
-- @param player	��ǰ���
-- @return 0:�ɹ� <0:��ͨ��ʧ������
function skillimplbase:studyskill( player)
	return -1;
end

-- ʹ�ü���
-- @param player	��ǰ���
-- @param rundata	�������ݰ�
function skillimplbase:useskill( player, rundata)
	return 0;
end

-- ��˲�����ܵ�tick��ں���
-- @param player	��ǰ���
-- @param rundata	����ʱ����
-- @param tnow		��ǰʱ��
-- @return	0:����ִ�� 1:skill����
function skillimplbase:process( player, rundata, tnow)
	return 1;
end

-- �����������淶
-- sxxxx_xxxxxxxx
-- ����ʵ����:s��ĸ+2λʮ�����Ʊ�ʾ������+2λʮ�����Ʊ�ʾ��ְҵ+��ĸ'_'�ָ�+8λ16���Ʊ�ʾ�ļ��ܱ��

-- ����ע������֧�ֵļ���ʵ����
_skill_reg ={}

-- ����
_skill_reg[0] ={}
------ սʿ
_skill_reg[0][0] ={}
------ ��ʦ
_skill_reg[0][1] ={}

------------------------------------------------------------�����˶�skillimplbase��Ĺ��������ķ�װ---------------------------------------------
-- ��studyskill�ķ�װ
-- @param skid		����id
-- @param player	
-- @return 0:�ɹ� -1:����δע��
function skill_study( skid, player)
	local sk =_skill_reg[player.race][player.profession][skid];
	
	if sk == nil then return 1 end;
	
	return sk:studyskill( player);
end

--��useskill�ķ�װ
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
