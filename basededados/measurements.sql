/*drop trigger if exists check_datetime;*/
drop table if exists measurements;
drop table if exists users;


create table users(
	idArduino varchar(255),
	email varchar(255),
	password varchar(255),
	configInt integer,
	primary key(idArduino));

create table measurements(
	idArduino varchar(255),
	minutes timestamp,
	temperature integer,
	light integer,  
	moisture integer,
	primary key(minutes),
	foreign key(idArduino) references users(idArduino) ); 
	
/*
insert into measurements values('2017-06-03%21:25:00', 23, 110, 370, 'ola');
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372, 'olaaa');
*/

insert into users values('a0:20:a6:15:42:2e', 'ju@cats.jo', 'olaole', '2');

/*
delimiter $$
create trigger check_datetime before insert on measurements
for each row
begin
	if exists (select * from measurements as m where DAY(m.minutes)+1 = DAY(new.minutes) and hour(m.minutes) = hour(new.minutes)) then
	
		delete from measurements where hour(minutes) = hour(new.minutes);
		insert into measurements values(new.idArduino, new.minutes, new.temperature, new.light, new.moisture);
	
	end if;
end$$
delimiter ;
*/
