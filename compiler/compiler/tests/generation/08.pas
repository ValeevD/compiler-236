var
	ar : array [1..10] of integer;
begin
	ar[3] := 7;
	ar[7] := 3;
	write(ar[7], ar[3]); {37}
end.