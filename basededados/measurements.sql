drop table if exists measurements;

create table measurements(
	minutes datetime, 
	temperature integer,
	light integer,  
	moisture integer,
	primary key(minutes));   	
	
	
insert into measurements values('2017-03-18 21:25:00', 23, 110, 370);
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372);
insert into measurements values('2017-03-19 08:35:00', 19, 80, 370);
	