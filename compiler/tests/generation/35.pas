
type
  T = record a: Real; b: array [1..10] of Real; end;

procedure P(var s: T);
begin
  s.b[3] := s.a;
end;

var
  q: T;
begin
  q.a := 99;
  P(q);
  Write(q.b[3]);
end.
