drop table if exists measurements;
drop table if exists users;

create table users(
	email varchar(255),
	password varchar(255),
	Primary key(email));

create table measurements(
	minutes timestamp, 
	email varchar(255),
	temperature varchar(255),
	light varchar(255),  
	moisture varchar(255),
	primary key(minutes, email),
	foreign key(email) references users(email) );
	
/*
insert into users values('ju@gm.com', '0op01');
insert into users values('cats@sp.com', '0op01jk');
insert into users values('jo@ts.pt', '01glo');	
	
	
insert into measurements values('2017-03-18 21:25:00', 23, 110, 370);
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372);
insert into measurements values('2017-03-19 08:35:00', 19, 80, 370);
*/
