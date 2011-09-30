procedure p(var i : integer);
begin
	i := 3*3*3;	
end;

var
	j : integer;
begin
	p(j);
	write(j + 3);
end.