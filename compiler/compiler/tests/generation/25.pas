const
	N = 10;
var
	i, j, h : integer;
	ar : array [1..N] of integer;
begin
	for i := 1 to N do
		ar[i] := (i*i*2-3) * 4  + i div 3;
	for i := 1 to N do
		for j := i to N do
				if ar[i] > ar[j] then
					begin
						h := ar[i];
						ar[i] := ar[j];
						ar[j] := h;
					end;
	for i := 1 to N do
		write (ar[i], ' ');
end.