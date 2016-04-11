let rec iter (n, f) x = 
	if n <= 0 then 0
	else iter (n-1, f) f x

