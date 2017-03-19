drop table if exists measurements;

create table measurements(
	minutes datetime, 
	temperature integer,
	light integer,  --  the foot-candle, the lux, and the lumen are all units used to describe relative brightness or luminosity to the human eye. Radiometric
					--measurements are used to describe the radiant energy of light, and common units
					--include watts, calories, BTU’s, and joules.
	moisture integer,
	primary key(minutes));   

	
insert into measurements values('2017-03-18 21:25:00', 23, 110, 370);
insert into measurements values('2017-03-18 18:50:00', 30, 100, 372);
insert into measurements values('2017-03-19 08:35:00', 19, 80, 370);
	
delimeter $$
create procedure delete_measurement (in minutes datetime, in temperature integer, in light integer, in moisture integer)
--returns datetime; --nao preciso de retornar propriamente nada, nao e?
begin	
	--declare dateday
	--select * into dateday
	select *
	from measurements as m
	where m.minutes = minutes;
	return
	
end$$
delimeter ;

create trigger check_datetime before insert on measurements
for each row
begin
	if exists (select * from measurements as m where time(new.minutes) = time(m.minutes)) then
		insert into measurements values(new.minutes, new.temperature, new.light, new.moisture);
		select delete_measurement(m.minutes, m.temperature, m.light, m.moisture);
	end if;

end 