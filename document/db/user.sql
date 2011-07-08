drop procedure if exists getuserinfo;

drop index uinfo_isdel_ind on userinfo;

drop index uinfo_uname_ind on userinfo;

drop table if exists userinfo;

create table userinfo
(
   userid               int not null auto_increment,
   username             varchar(20) not null,
   password             varchar(33) not null,
   validatetime         int,
   lastregion           smallint default 0,
   lastlogout           int not null default 0,
   types                smallint not null default 0,
   isdel                smallint not null default 0,
   deltime              int not null default 0,
   primary key (userid)
)
type = InnoDB;

create index uinfo_uname_ind on userinfo
(
   username
);

create index uinfo_isdel_ind on userinfo
(
   isdel
);


delimiter $$

create procedure getuserinfo ( in uname varchar(20))
begin
    select userid,password,validatetime,lastregion,lastlogout,types,isdel from userinfo where username=uname;
    
end $$

delimiter ;

