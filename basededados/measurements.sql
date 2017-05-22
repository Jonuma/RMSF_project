drop table if exists measurements;
drop table if exists users;

create table measurements(
	minutes timestamp, 
	temperature integer,
	light integer,  
	moisture integer,
	primary key(minutes)); 

create table users(
	username varchar(255),
	password varchar(255),
	primary key(username));
	
	
insert into measurements values('2017-03-18 21:25:00', 23, 110, 370);
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372);
insert into measurements values('2017-03-19 08:35:00', 19, 80, 370);

insert into users values('jmbaleiras', '0op01');
insert into users values('cats', '0op01jk');
insert into users values('joaom', '01glo');
