class 'inst_1' (instbase)

function inst_1:__init()
	instbase.__init()
end

function inst_1:entercheck( player)
	if player.level >= 0 and player.level <=10 then return 0 end;
	
	return -1;
end

_inst_sc[1]=inst_1();
