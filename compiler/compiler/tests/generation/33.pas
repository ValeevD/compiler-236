Const
	N = 5;

Type
	myArr = array [1..N] of integer;

procedure swap (var x, y : integer);
var
	h : integer;
begin
	h := x;
	x := y;
	y := h;
end;

function sort (arr : myArr): myArr;
var
	i,j,h : integer;
begin
	for i := 1 to N do
	   for j := i to N do	
		   if arr[i] > arr[j] then
		   	swap(arr[i], arr[j]);
	result := arr; 	                     
end;

Var
	ar1, ar2 : myArr;
	i : integer;
begin
	for i := 1 to N do
		ar1[i] := (i*i-3) mod 25 + 3*i -4;	
	ar2 := sort(ar1);
	for i := 1 to N do
		write (ar2 [i], ' ');
end.