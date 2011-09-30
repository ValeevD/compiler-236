procedure write_num(i : integer);
begin
	write(i, ' ');	
end;

var
	j : integer;
begin
	for j := 1 to 5 do
		write_num (j*j);
end.