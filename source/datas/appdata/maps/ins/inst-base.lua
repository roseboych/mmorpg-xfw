class 'instbase'

function instbase:__init()
end

function instbase:entercheck( player)
	return -1;
end

_inst_sc ={}

function inst_entercheck( instid, player)
	local inst =_inst_sc[instid];
	
	if inst == nil then return -1 end;
	
	return inst:entercheck( player);
end
