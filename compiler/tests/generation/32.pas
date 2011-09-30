function p(i : integer):integer;
begin
	result := i;
	exit;
	result := 100000;
end;

begin
	write(p(5));
end.