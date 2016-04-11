let rec iter(n, f) x =
	if n <= 0 then x
	else iter((n-1, f)) (f x);;

let _ = print_int (iter(5, function x -> 2+x) 0);;
let _ = print_endline "";;
