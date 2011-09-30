var
	i : integer;
begin
	i :=0;
	repeat
		i := i+1;
		if (i mod 2) = 1 then
			write(i, ' ');
	until i = 50;
end.