-- reslib application
-- @category	buffer system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- buffer�ű��Ļ��ඨ��

class 'bufferimplbase'

-- ��ʼ�����캯��
function bufferimplbase:__init()
end

-- player���Գ�ʼ��ʱ���õĺ���
-- @param player ��ǰ��player
function bufferimplbase:initplayerprop( player)
end

-- buffer��tick�������
-- @param player ��ǰ��player
-- @param buffdata ��ǰҪ�����buffer���ݰ�
function bufferimplbase:tick( player, buffdata)
end

-- ���������е�bufferʵ��
_buf_reg ={}

------------------------------------------------------------�����˶�bufferimplbase��Ĺ��������ķ�װ---------------------------------------------

-- ��װ�˶�buffer��ĵ���
-- @param iid	bufferʵ������,��.lu�ļ���һ��
-- @param player ��ǰ�����player
function buff_initplayerprop( iid, player)
	local buff_ins =_buf_reg[iid];
	
	if buff_ins == nil then return end;
	
	buff_ins:initplayerprop( player);
end

-- ��װ�˶�buffer��ĵ���
-- @param iid	bufferʵ������,��.lu�ļ���һ��
-- @param player ��ǰ�����player
-- @param buffdata ��ǰҪ�����buffer���ݰ�
function buff_tick( iid, player, buffdata)
	local buff_ins =_buf_reg[iid];
	
	if buff_ins == nil then return end;
	
	buff_ins:tick( player, buffdata);
end
