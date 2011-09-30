var
	i : integer;
begin
	i := 0;
	while i <> 50 do
	begin
		i := i + 1;
		if (i mod 2) = 0 then
			write(i, ' ');
	end;
end.