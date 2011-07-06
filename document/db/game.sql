drop procedure if exists addbuffer;

drop procedure if exists addchrinfo;

drop procedure if exists addchritem;

drop procedure if exists callchrpet;

drop procedure if exists chgchritem;

drop procedure if exists chrupdate;

drop procedure if exists delchrdata;

drop procedure if exists delchritem;

drop procedure if exists deletebuffer;

drop procedure if exists generate_testdata;

drop procedure if exists getbuffers;

drop procedure if exists getchravlist;

drop procedure if exists getchrinfo;

drop procedure if exists getchritems;

drop procedure if exists getchrlist;

drop procedure if exists getchrpetdata;

drop procedure if exists itemoperator;

drop procedure if exists levelupdate;

drop procedure if exists skillupdate;

drop table if exists chr_buffer;

drop index ind_nickname on chrdata;

drop index ind_region on chrdata;

drop index ind_chrisdel on chrdata;

drop index ind_chruser on chrdata;

drop table if exists chrdata;

drop table if exists chrdata_base;

drop table if exists chritems;

drop table if exists petdata;

create table chr_buffer
(
   bufferid             int not null,
   chrid                int not null,
   types                smallint comment '0:系统buffer 1:应用buffer',
   createtime           int comment 'yymmddhhssmm',
   scopetime            int comment '相对时间类型的buffer有效',
   primary key (bufferid, chrid)
);

create table chrdata
(
   chrid                int not null auto_increment,
   userid               int not null,
   region               smallint not null comment '所在的大区',
   nickname             varchar(20) not null,
   race                 smallint not null comment '1:人族',
   profession           smallint not null comment '1:战士 2:法师',
   sex                  smallint not null default 0 comment '0:man 1:female',
   moneys               int not null default 0,
   levels               int not null default 0,
   bagslots             smallint not null,
   skill01              int,
   skill02              int,
   petid                int not null default -1 comment '-1:当前没有宠物 >= 0 关联到petdata表',
   isdel                smallint not null default 0 comment '1:yes 0:no',
   primary key (chrid),
   unique key AK_unq_chrnickname (nickname, region)
)
type = InnoDB;

create index ind_chruser on chrdata
(
   userid
);

create index ind_chrisdel on chrdata
(
   isdel
);

create index ind_region on chrdata
(
   region
);

create index ind_nickname on chrdata
(
   nickname
);

create table chrdata_base
(
   chrid                int not null,
   hp                   int not null default 0,
   mp                   int not null default 0,
   power                int,
   agile                int,
   endurance            int,
   intellect            int,
   spirit               int,
   armor                int,
   lastposx             float not null default 2147483647,
   lastposy             float not null default 2147483647,
   lastposz             float not null default 2147483647,
   lastfacing           float not null default -1,
   died                 smallint not null default 0 comment '0:living 1:died',
   exp                  int not null default 0,
   primary key (chrid)
);

create table chritems
(
   itemid               int not null auto_increment,
   chrid                int,
   inavatar             smallint not null default 0 comment '0:在背包中 1:普通装 2:属于时装',
   inpart               smallint not null default 0 comment 'inavatar=1,2 有效 0-16  0:表示存放的格子',
   itemcode             int not null,
   validatetype         smallint not null comment '0:按次计 1:按绝对时间计 2:永久',
   starttime            int comment 'validatetype=0 表示剩余次数 =3保留 =1剩余的在线时间 =2开始时间yyyymmddhh',
   endtime              int comment '=2时有效，表示结束时间  =0保存堆跌次数',
   primary key (itemid)
);

create table petdata
(
   petid                int not null auto_increment,
   chrid                int not null,
   petcode              int not null default 0,
   petname              varchar(20) not null,
   isdel                smallint not null default 0,
   primary key (petid)
);

alter table chr_buffer add constraint FK_chrbuf_id foreign key (chrid)
      references chrdata (chrid) on delete restrict on update restrict;

alter table chrdata_base add constraint FK_chr_dbase_id foreign key (chrid)
      references chrdata (chrid) on delete restrict on update restrict;

alter table chritems add constraint fk_chrtoitem foreign key (chrid)
      references chrdata (chrid) on delete restrict on update restrict;

alter table petdata add constraint fk_chrtopet foreign key (chrid)
      references chrdata (chrid) on delete restrict on update restrict;


delimiter $$

create procedure addbuffer ( cid int, bufid int, t smallint, ct int, st int)
begin
   
    set @cnt =0;
    select @cnt := count(*) from chr_buffer where chrid=cid and bufferid=bufid;
        
    if @cnt > 0 then
        update chr_buffer set createtime=ct,scopetime=st where chrid=cid and bufferid=bufid;
    else
        insert into chr_buffer( chrid, bufferid, types, createtime, scopetime)
            values( cid, bufid, t, ct, st);
    end if;
    
        
    /*数据库错误*/
    if row_count() = 0 then
        select -2;
    end if;
      
    select 0;
end
$$

delimiter ;


delimiter $$

create procedure addchrinfo ( uid int, rid smallint, n varchar(20), rc smallint, pf smallint, sx smallint, hpv int, mpv int,
    expv int, mv int, posx float, posy float, posz float, fc float, powerv int, agilev int, endurancev int,
    intellectv int, spiritv int, armorv int, bags smallint, sk01 int, sk02 int)
begin

    DECLARE EXIT HANDLER FOR SQLSTATE '23000'
    begin
        /*昵称重复*/
        select -1;
    end;
    
    set @cnt=0;
    select count(chrid) into @cnt from chrdata where userid=uid and region=rid and isdel=0;
    if @cnt >= 5 then
        /*最多4个*/
        select -2;
    else
        insert into chrdata(userid,region,nickname,race,profession,sex,moneys,levels,bagslots,skill01,skill02,petid,isdel)
            values( uid, rid, n, rc, pf, sx, 0, 1, bags, sk01, sk02, -1, 0);
        
        if row_count() = 0 then
            select -3;
        else
            set @cid=0;
            select last_insert_id() into @cid;
            
            insert into chrdata_base(chrid, hp, mp, power, agile, endurance, intellect, spirit, armor, 
                lastposx,lastposy,lastposz,lastfacing,died,exp)
                values( @cid, hpv, mpv, powerv, agilev, endurancev, intellectv, spiritv, armorv,posx, posy, posz, fc,0,expv);
                
            if row_count() = 0 then
                select -3;
            end if;
        
            /*result, chrid, moneys, levels, petid*/
            select 0, @cid, 0, 1, -1;
        
        end if;
        
    end if;
end $$

delimiter ;


delimiter $$

create procedure addchritem (cid int, inp smallint, icode int, vtype smallint, stime int, etime int)
begin
    declare exit handler for sqlstate '23000' 
    begin
        /*外键不存在*/
        select -1;
    end;
    
    set @bagnums =0;
    select @bagnums := bagslots from chrdata where chrid=cid;
    
    set @cnt =0;
    select @cnt := count(*) from chritems where chrid=cid and inavatar=0;
    if @cnt >= @bagnums then
        /*背包数量满*/
        select -2;
    else
        insert into chritems(chrid,inavatar,inpart,itemcode,validatetype,starttime,endtime)
            values(cid,0,inp,icode,vtype,stime,etime);
            
        if row_count() = 0 then
            select -3;
        else
            select 0, last_insert_id(), cid, 0, inp, icode, vtype, stime, etime;
        end if;
    end if;       
    
end $$

delimiter ;


delimiter $$

create procedure callchrpet (cid int,pid int,dtype smallint)
begin
    if dtype = 0 then
        update chrdata a, petdata p set a.petid=pid where a.chrid=cid and a.chrid=p.chrid and p.petid=pid and p.isdel=0;
    else
        update chrdata a, petdata p set a.petid=-1 where a.chrid=cid and a.petid=pid and a.chrid=p.chrid and p.petid=pid;
    end if;
    
    if row_count() <> 1 then
        select 1;
    else
        select 0;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure chgchritem ( id int, st int ,et int)
begin
    update chritems set starttime=st, endtime=et where itemid=id;
    
    if row_count() = 0 then
        select -1;
    else
        select 0;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure chrupdate ( cid int, px float, py float, pz float, fc float, 
    h int, m int, e int, mo int, dv smallint)
begin
    update chrdata set 
        moneys =mo where chrid=cid and isdel=0;
    
    if row_count() = 0 then
        select -1;
    else
        update chrdata_base set lastposx=px, lastposy=py, lastposz=pz, lastfacing=fc,
            hp =h, mp =m, exp =e, died=dv where chrid=cid;
            
        if row_count() =0 then
            select -1;
        end if;
        
        select 0;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure delchrdata (cid int)
begin
    update chrdata set isdel=1 where chrid=cid and isdel=0;
    
    if row_count() = 0 then
        select -1;
    else
        select 0;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure delchritem ( id int)
begin
    delete from chritems where itemid=id;
    
    if row_count() = 0 then
        select -1;
    else
        select 0;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure deletebuffer ( cid int, bufid int)
begin
   
    delete from chr_buffer where bufferid=bufid and chrid=cid;
        
    if row_count() = 0 then
        select -1;
    else
        select 0;
    end if;

end
$$

delimiter ;


delimiter $$

create procedure generate_testdata (nums int, ign int)
begin
    set autocommit=0;
    
    set @ii =nums/1000;
    set @ss =1000;
    if @ii <= 0 then
        set @ii =1;
        
        set @ss=nums;
    end if;

    set @jj =0;
    
    while @jj < @ii do
        start transaction;
        
        set @kk =1;
        while @kk <= @ss do
            /*创建帐号*/
            set @vd =@jj*10000+@kk;
            insert into userdb.userinfo( username,password,validatetime,lastregion,types,isdel) 
                values( @vd, @vd, 8888888, 1, 0, 0);
                
            select @iid := last_insert_id();
                
            /*创建角色*/
            if @vd < ign then
                call addchrinfo( @iid, 1, @iid, 0, 0, 0, 1000, 1000, 1000, 999999, 10, 10, 0, 0,
                    20, 10, 20, 10, 20, 10,
                    50, 0, 0);
            end if;
        
            set @kk =@kk+1;
        end while;
    
        commit;
        
        set @jj =@jj+1;
    end while;
    
    set autocommit=1;
    
end $$

delimiter ;


delimiter $$

create procedure getbuffers ( cid int)
begin
   
    select bufferid, types, createtime, scopetime from chr_buffer where chrid=cid;

end
$$

delimiter ;


delimiter $$

create procedure getchravlist (IN uid int, IN rid smallint)
begin
    select i.chrid, i.inavatar, i.itemcode, i.inpart
        from chrdata c, chritems i where c.userid=uid and c.region=rid and c.chrid=i.chrid and i.inavatar <> 0
        order by i.chrid,i.inavatar,i.inpart;

end $$

delimiter ;


delimiter $$

create procedure getchrinfo ( cid int)
begin
    select c.nickname,c.race,c.profession,c.sex,c.moneys,c.levels,c.bagslots,c.skill01,c.skill02,c.petid,
        cb.hp,cb.mp,cb.power,cb.agile,cb.endurance,cb.intellect,cb.spirit,cb.armor,
        cb.lastposx,cb.lastposy,cb.lastposz,cb.lastfacing,cb.died,cb.exp
        from chrdata c, chrdata_base cb where c.chrid=cid and c.chrid=cb.chrid;
end
$$

delimiter ;


delimiter $$

create procedure getchritems (IN cid int)
begin
    select itemid,inavatar,inpart,itemcode,validatetype,starttime,endtime 
        from chritems where chrid=cid order by itemid;
end
$$

delimiter ;


delimiter $$

create procedure getchrlist ( uid int, rid smallint)
begin
    
    select a.chrid, a.nickname, a.race, a.profession, a.sex, a.lastposx, a.lastposy, a.lastposz, a.lastfacing,
        a.hp, a.mp, a.exp, a.levels, a.moneys, ifnull(p.petid,-1), ifnull(p.petcode,-1), ifnull(p.petname,'')
    from
    (select c.chrid, c.nickname, c.race, c.profession, c.sex, cb.lastposx, cb.lastposy, cb.lastposz, cb.lastfacing,
        cb.hp, cb.mp, cb.exp, c.levels, c.moneys, c.petid
        from chrdata c, chrdata_base cb where c.userid=uid and c.region=rid and c.isdel=0 and c.chrid=cb.chrid) a
    left join petdata p
    on a.chrid=p.chrid and a.petid=p.petid and p.isdel=0
    order by a.chrid;
    
end $$

delimiter ;


delimiter $$

create procedure getchrpetdata (cid int)
begin
    select petid,petcode,petname from petdata where chrid=cid and isdel=0;
end $$

delimiter ;


delimiter $$

create procedure itemoperator ( id int, dot smallint, pt smallint)
begin
    declare exit handler for sqlstate '23000'
    begin
        select -1;
    end;
    
    if dot = 0 then
        /*背包到普通装*/
        update chritems set inavatar=1,inpart=pt where itemid=id and inavatar=0;
            
        if row_count() = 0 then
            select -2;
        else
            select 0;
        end if;
    elseif dot = 1 then
        /*普通装到背包*/
        update chritems set inavatar=0,inpart=pt where itemid=id and inavatar=1;
            
        if row_count() = 0 then
            select -2;
        else
            select 0;
        end if;
    elseif dot = 2 then
        /*背包到时装*/
        update chritems set inavatar=2,inpart=pt where itemid=id and inavatar=0;
            
        if row_count() = 0 then
            select -2;
        else
            select 0;
        end if;

    else
        /*时装到背包*/
        update chritems set inavatar=0,inpart=pt where itemid=id and inavatar=2;
            
        if row_count() = 0 then
            select -2;
        else
            select 0;
        end if;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure levelupdate ( cid int, newlv int)
begin
    update chrdata set levels=newlv where chrid=cid and isdel=0;
    
    if row_count() = 0 then
        select -1;
    else
        select 0;
    end if;
    
end $$

delimiter ;


delimiter $$

create procedure skillupdate ( cid int, sk01 int, sk02 int)
begin
    update chrdata set skill01=sk01, skill02=sk02 where chrid=cid and isdel=0;
    
    if row_count() = 0 then
        select -1;
    else
        select 0;
    end if;
    
end $$

delimiter ;

