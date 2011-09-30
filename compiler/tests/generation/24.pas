const
	N = 10;
var
	i : integer;
	ar : array [1..N] of integer;
begin
	for i := 1 to N do
		ar[i] := i;
	for i := N downto 1 do
		write (ar[i], ' ');
end.