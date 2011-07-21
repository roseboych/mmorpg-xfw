class 'task_2' (taskbase)

function task_2:__init()
	taskbase.__init()
end

function task_2:get_task( player)
	if player.level >= 1 and player.level <= 10 then return 0 end;
	
	return -1;
end

function task_2:submit_task( player)
	return 0;
end

_task_sc[2]=task_2();
