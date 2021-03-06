type heap = EMPTY | NODE of rank * value * heap * heap
and rank = int
and value = int

exception EmptyHeap
let rank 		= function EMPTY -> 0
				| NODE(r, _, _, _) -> r
let insert 		= function (x, h) -> merge(h, NODE(0, x, EMPTY, EMPTY))
let findMin		= function EMPTY -> 0
				| NODE(_, x, _, _) -> x
let deleteMin 	= function EMPTY -> raise EmptyHeap
				| NODE(_, x, lh, rh) -> merge(lh, rh)

let shake 		= function (x, lh, rh) ->
				if (rank lh) >= (rank rh)
				then NODE(rank rh + 1, x, lh, rh)
				else NODE(rank lh + 1, x, rh, lh)

let merge		= function (h, n) ->
				match ((r1, x1, lh1, rh1), (r2, x2, lh2, rh2)) with
				(EMPTY, n) -> n
				|  
