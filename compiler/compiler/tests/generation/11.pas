var
	rec : record
		ar : array [1..2] of integer;
		a : integer;
		b : real;
	end;
begin
	rec.ar[1] := 3;
	rec.ar[2] := 5; 
	rec.a := rec.ar[1] + 5;
	rec.b := rec.a +3.4;
	write(rec.ar[1] + rec.ar[2], rec.a, rec.b);{8811.4}
end.