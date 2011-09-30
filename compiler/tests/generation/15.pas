var
	i : real;
begin
	i := -3.4;
	
	if i = -3.4 then
		begin
			i := 1+2+3+3.4+i;
			i := i - 6 + 1;
			write(i);
		end
	else
		write('fasle');
 
end.