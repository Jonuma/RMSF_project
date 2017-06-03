drop table if exists measurements;
drop table if exists users;

create table measurements(
	minutes timestamp, 
	temperature integer,
	light integer,  
	moisture integer,
	idArduino varchar(255),
	primary key(minutes)); 

create table users(
	email varchar(255),
	password varchar(255),
	config integer,
	primary key(email));
	
insert into measurements values('2017-03-18 21:25:00', 23, 110, 370, 'ola');
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372, 'olaaa');

insert into users values('jmbaleiras', '0op01', '2');
insert into users values('cats', '0op01jk', '2');
insert into users values('joaom', '01glo', '2');
