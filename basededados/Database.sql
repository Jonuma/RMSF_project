--nao consegui testar yet!!!!

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
	
	
create trigger check_datetime before insert on measurements
for each row
begin
	if exists (select * from measurements as m where hour(new.minutes) = hour(m.minutes) and minute(new.minutes) = minute(m.minutes)) then
	
		delete from measurements as m
		where hour(new.minutes) = hour(m.minutes) and minute(new.minutes) = minute(m.minutes)
		limit 1; --vi que "limit [int]" significa o numero que esta em "int" e o numero de linhas que ele apaga devido ao "limit"
		
		insert into measurements values(new.minutes, new.temperature, new.light, new.moisture);
	
	end if;

end 