-- reslib application
-- @category	buffer system script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- buffer脚本的基类定义

class 'bufferimplbase'

-- 初始化构造函数
function bufferimplbase:__init()
end

-- player属性初始化时调用的函数
-- @param player 当前的player
function bufferimplbase:initplayerprop( player)
end

-- buffer的tick函数入口
-- @param player 当前的player
-- @param buffdata 当前要处理的buffer数据包
function bufferimplbase:tick( player, buffdata)
end

-- 保存了所有的buffer实现
_buf_reg ={}

------------------------------------------------------------定义了对bufferimplbase类的公共函数的封装---------------------------------------------

-- 封装了对buffer类的调用
-- @param iid	buffer实例名称,和.lu文件名一致
-- @param player 当前处理的player
function buff_initplayerprop( iid, player)
	local buff_ins =_buf_reg[iid];
	
	if buff_ins == nil then return end;
	
	buff_ins:initplayerprop( player);
end

-- 封装了对buffer类的调用
-- @param iid	buffer实例名称,和.lu文件名一致
-- @param player 当前处理的player
-- @param buffdata 当前要处理的buffer数据包
function buff_tick( iid, player, buffdata)
	local buff_ins =_buf_reg[iid];
	
	if buff_ins == nil then return end;
	
	buff_ins:tick( player, buffdata);
end
