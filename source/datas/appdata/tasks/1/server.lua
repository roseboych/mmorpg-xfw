
class 'task_1' (taskbase)

function task_1:__init()
	taskbase.__init()
end

function task_1:get_task( player)
	if player.level >= 1 and player.level <= 10 then return 0 end;
	
	return -1;
end

function task_1:submit_task( player)
	return 0;
end

_task_sc[1]=task_1();
