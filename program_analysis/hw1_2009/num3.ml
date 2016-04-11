let rec zipperN: int list list -> int list = fun lst ->
	match lst with
	[] -> []
	| lst -> List.append (getFst (pair ([], [], lst))) (zipperN (getSnd (pair ([], [], lst))))
and pair: int list * int list list * int list list -> int list * int list list * int list list = fun (l1, l2, l3) ->
	match l3 with
	[] -> (l1, l2, [])
	| []::tlst -> pair (l1, l2, tlst)
	| (h::t)::tlst -> pair ((List.append l1 [h]), (List.append l2 [t]), tlst)
and getFst p = match p with (f, _, _) -> f
and getSnd p = match p with (_, s, _) -> s;;
