drop table if exists measurements;

create table measurements(
	minutes datetime, 
	temperature integer,
	light integer,  --  the foot-candle, the lux, and the lumen are all units used to describe relative brightness or luminosity to the human eye. Radiometric
					--measurements are used to describe the radiant energy of light, and common units
					--include watts, calories, BTU’s, and joules.
	moisture integer,
	primary key(minutes));   

	
insert into measurements values('2017-03-18 21:25:00', 23, , 370)
	
create trigger check_datetime before insert on measurements
for each row
begin
	if exists (select * from measurements as m where time(new.minutes) = time(m.minutes)) then
		insert into measurements values(new.minutes, new.temperature, new.light, new.moisture)
	end if;

end 