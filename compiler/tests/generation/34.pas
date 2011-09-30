const
   N = 100;
type
   myArr = array [1..N] of integer;
var
   ar : myArr;
   q : integer;

procedure qsort(var ARR : myArr; l,r : integer);
var
   i,j,x,y : integer;
begin
   i := l;
   j := r;
   x := ARR[(l+r) div 2];		                                    	
   repeat
      while ARR[i] < x do
	i := i + 1;
      while ARR[j] > x do
	j := j - 1;
      if i<=j then
	 begin
	    y := ARR[i];
	    ARR[i] := ARR[j];
	    ARR[j] := y;
	    i := i + 1;
	    j := j - 1;
	 end;
   until i > j;
   if l < j then
	qsort(ARR, l, j);
   if i < r then
	qsort(ARR, i, r);
end;

begin
	for q := 1 to N do
		ar[q] := (q*q*q) mod 23;
	qsort(ar, 1, N);
	for q := 1 to N do
		write(ar[q], ' '); 
end.