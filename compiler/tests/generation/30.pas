function fib (n : integer):integer;
begin
   if (n = 1) or (n = 2) then
	result := 1
   else
	result := fib(n-1) + fib(n-2);
end;

begin
   write(fib(7));
end.