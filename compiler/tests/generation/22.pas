var
	i, j : integer;
begin
	for i := 1 to 50 do
		begin
			j := 100;
			while j <> i do
				j := j - 1;			
			if (j mod 2) = 0 then
				write (j, ' ');
		end;
end.