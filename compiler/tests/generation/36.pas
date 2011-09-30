
function f(x: Real): Real;
begin
  Result := x * x - 5;
end;

var
  a, b, x: Real;
begin
  a := 0;
  b := 5;
  while b - a > 0.00001 do begin
    x := (a + b) / 2;
    Write(x, ' ');
    if f(x) > 0 then b := x else a := x;
  end;
end.
