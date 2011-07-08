-- css application
-- @category	player script
-- @package
-- @author	Created by Lu Jianfeng ( lujf1978@163.com), 2010
-- ��ɫ���Ի������ñ�

-- �����ְҵ������������
-- ����force������agile������endurance������intellect������spirit������armor
-- �������� maxexp, maxhp, maxmp
-- ���ո���������з�������
-- 

-- ȫ�����ñ�
cfg_character_props ={}
--race=0 profession=0 level 1~5
cfg_character_props[0] ={}
cfg_character_props[0][0] ={}
cfg_character_props[0][0][1] ={ force=10, agile=15, armor=100, endurance=120, intellect=60, spirit=50, maxexp=100, maxhp=1000, maxmp=1000}
cfg_character_props[0][0][2] ={ force=20, agile=25, armor=200, endurance=240, intellect=70, spirit=60, maxexp=300, maxhp=2000, maxmp=2000}
cfg_character_props[0][0][3] ={ force=40, agile=45, armor=300, endurance=380, intellect=80, spirit=70, maxexp=600, maxhp=3000, maxmp=3000}
cfg_character_props[0][0][4] ={ force=70, agile=75, armor=500, endurance=450, intellect=90, spirit=90, maxexp=1000, maxhp=4000, maxmp=4000}
cfg_character_props[0][0][5] ={ force=100, agile=105, armor=700, endurance=600, intellect=100, spirit=120, maxexp=1500, maxhp=5000, maxmp=5000}

--race=0 profession=1 level 1~5
cfg_character_props[0][1] ={}
cfg_character_props[0][1][1] ={ force=10, agile=15, armor=100, endurance=120, intellect=60, spirit=50, maxexp=100, maxhp=1000, maxmp=1000}
cfg_character_props[0][1][2] ={ force=20, agile=25, armor=200, endurance=240, intellect=70, spirit=60, maxexp=300, maxhp=2000, maxmp=2000}
cfg_character_props[0][1][3] ={ force=40, agile=45, armor=300, endurance=380, intellect=80, spirit=70, maxexp=600, maxhp=3000, maxmp=3000}
cfg_character_props[0][1][4] ={ force=70, agile=75, armor=500, endurance=450, intellect=90, spirit=90, maxexp=1000, maxhp=4000, maxmp=4000}
cfg_character_props[0][1][5] ={ force=100, agile=105, armor=700, endurance=600, intellect=100, spirit=120, maxexp=1500, maxhp=5000, maxmp=5000}








-- ���Գ�ʼ��
-- @param player ��ǰ�����player
function init_character_property( player)
	if player == nil then return end;
	
	local race =player.race;
	local profession =player.profession;
	local level =player.level;
	
	local props =cfg_character_props[race][profession][level];
	
	if props == nil then return end;
	
	player.maxhp =props.maxhp;
	player.maxmp =props.maxmp;
	player.maxexp =props.maxexp;
	player.force =props.force;
	player.agile =props.agile;
	player.endurance =props.endurance;
	player.intellect =props.intellect;
	player.spirit =props.spirit;
	player.armor =props.armor;
end

-- ����property���ݣ���ֹ�г������ֵ���ֵ����
-- @param player ��ǰ�����player
function max_character_property( player)
	if player == nil then return end;
	
end
