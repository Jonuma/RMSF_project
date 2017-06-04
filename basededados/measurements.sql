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
	configInt integer,
	primary key(email));
	
insert into measurements values('2017-03-18 21:25:00', 23, 110, 370, 'ola');
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372, 'olaaa');

insert into users values('jmbaleiras', '0op01', '2');
insert into users values('cats', '0op01jk', '2');
insert into users values('joaom', '01glo', '2');

create trigger check_datetime before insert on measurements
for each row
begin
	if exists (select * from measurements as m where day(new.minutes) = day(m.minutes)+1 and hour(new.minutes) = hour(m.minutes)) then
	
		delete from measurements as m
		where day(new.minutes) = day(m.minutes)+1 and hour(new.minutes) = hour(m.minutes)
		limit 1; --vi que "limit [int]" significa o numero que esta em "int" e o numero de linhas que ele apaga devido ao "limit"
		
		insert into measurements values(new.idArduino, new.minutes, new.temperature, new.light, new.moisture);
	
	end if;

end 