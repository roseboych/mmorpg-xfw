-- reslib application
-- @category	buffer system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- ��˪���ܵĽű�����

class 'b_frostshield' (bufferimplbase)

function b_frostshield:__init()
	bufferimplbase.__init()
end

function b_frostshield:initplayerprop( player)
	player.armor =1000;
	player.force =200;
	player.armor =player.armor*1.5
	player.force =player.force*1.2
end

function b_frostshield:tick( player, buffdata)
end

-- ע�ᵽ_buf_regȫ�ֱ���
_buf_reg["frostshield"] =b_frostshield();
