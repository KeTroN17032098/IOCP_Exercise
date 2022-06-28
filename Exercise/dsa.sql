drop database if exists simpleaccount;

create database simpleaccount;
use simpleaccount;
drop table if exists accounts;
create table accounts(
	uuid int primary key auto_increment,
    id varchar(20) not null unique,
    pw varchar(20) not null
);

drop procedure if exists joined_member;
delimiter //
create procedure joined_member()
begin
	select * from simpleaccount.accounts;
end //
delimiter ;

drop procedure if exists add_member;
delimiter //
create procedure add_member(in ID varchar(20),in PW varchar(20))
begin
	declare asd int;
	select count(*) into asd from simpleaccount.accounts a where a.id=ID;
    if asd>0 then 
		set asd=-1;
	else 
		insert into simpleaccount.accounts values(NULL,ID,PW);
        select uuid into asd from simpleaccount.accounts a where a.id=ID;
    end if;
    select asd;
end //
delimiter ;
use mysql;
create user 'root'@'%' identified by '1234';
grant all privileges on simpleaccount.* to 'root'@'%';