
class 'taskbase'

function taskbase:__init()
end

function taskbase:get_task( player)
	return 0;
end

function taskbase:submit_task( player)
	return 0;
end

_task_sc ={}

function task_get( tid, player)
	local ts =_task_sc[tid];
	
	if ts == nil then return -1 end;
	
	return ts:get_task( player);
end

function task_submit( tid, player)
	local ts =_task_sc[tid];
	
	if ts == nil then return -1 end;
	
	return ts:submit_task( player);
end
