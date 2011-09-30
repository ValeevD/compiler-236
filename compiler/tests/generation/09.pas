var
	ar : array [1..2] of array[1..2] of integer;
begin
	ar[1,1] := 1;
	ar[1,2] := 2;
	ar[2,1] := 3;
	ar[2,2] := 4;
	write(ar[1,1],ar[1,2],ar[2,1],ar[2,2]);{1234}
end.