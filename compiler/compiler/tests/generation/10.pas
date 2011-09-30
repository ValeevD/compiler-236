var
	rec : record
		a : integer;
		b : real;
	end;
begin
	rec.a := 2;
	rec.b := 3.4;
	write(rec.a, rec.b);{23.4}
end.