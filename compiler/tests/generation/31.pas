function fact (n : integer):integer;
begin
   if (n = 1) then
	result := 1
   else
	result := n*fact(n-1);
end;

begin
   write(fact(6));
end.