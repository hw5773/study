open Program
open GraphPgm
open DavAbstract
open DomFunctor


let rec range i j = if i > j then [] else (DavInt.make i)::(range (i+1) j)

let rec print_mem = fun mlist ->
	let rec make_int_lst = fun dlist ->
		match dlist with
		| [] -> ()
		| h::t ->
		let DavInt.Z(d) = h in
		print_string (string_of_int d); print_string " "; (make_int_lst t)
	in
	match mlist with
	| [] -> print_string ""
	| h::t ->
	let (x, (d, l)) = h in
	let loc = String.concat " " (LocPowSet.to_list l) in
	let dlist = 
		(if d = DavIntPowSet.top
		then []
		else (DavIntPowSet.to_list d)) in
	print_string ("var: " ^ x ^ " val: ");
	make_int_lst dlist; print_string (" loc: "^loc^"\n"); (print_mem t)

let rec print_trace = fun pgm_g t ->
	let tlist = (Trace.to_list t) in
	match tlist with
	| [] -> print_string ""
	| h::tl ->
	let (num, st) = h in
		let _ = print_string ("<" ^ string_of_int(num) ^ "> \n") in
		match st with
		| StateDomain.BOT -> raise ErrorStateBot
		| StateDomain.ELT((i, m)) ->
		(if i < ((List.length (all_nodes pgm_g)) + 1)
		then print_string (command_to_str (get_command (List.nth (List.rev (all_nodes pgm_g)) (i-1))))
		else print_string "last");
		print_string "\n";
		print_mem (Memory.to_list m); print_trace pgm_g (Trace.make tl)
		| StateDomain.TOP -> raise ErrorStateTop

let get_node = fun pgm_g s ->
	List.nth (List.rev (all_nodes pgm_g)) ((fst s) - 1)

let get_memory = fun s -> snd s

let rec eval : memory -> exp -> value = fun m e ->
	let rec addSets : d list -> d list -> d list -> d list = fun l1 l2 r ->
		let rec update_result = fun l r ->
			match l with
			| [] -> r
			| h::t -> 
				if (List.mem h r) 
				then (update_result t r) 
				else (update_result t (List.sort DavInt.compare (h::r)))
		in
			match l1 with
			| [] -> r
			| h::t -> addSets t l2 (update_result (List.map (DavInt.add h) l2) r)
	in
	let rec negSet = fun d ->
		let lst = (DavIntPowSet.to_list d) in
		match lst with
		| [] -> DavIntPowSet.bot
		| h::t -> DavIntPowSet.union (DavIntPowSet.make [DavInt.neg h]) (negSet (DavIntPowSet.make t))
	in
	let rec join_fetch = fun lst ->
		match lst with
		| [] -> Val.make DavIntPowSet.bot LocPowSet.bot
		| h::t ->
			let d1, l1 = Memory.image m h in
			let d2, l2 = join_fetch t in
			Val.make (DavIntPowSet.join d1 d2) (LocPowSet.join l1 l2)
	in
 	match e with
	| NUM(i) -> Val.make (DavIntPowSet.make [DavInt.make i]) (LocPowSet.bot)
	| ADD(e1, e2) -> 
		let d1, l1 = eval m e1 in 
		let d2, l2 = eval m e2 in
		if ((d1 = DavIntPowSet.top) || (d2 = DavIntPowSet.top))
			then Val.make DavIntPowSet.top (LocPowSet.join l1 l2)
		else if ((d1 = DavIntPowSet.bot) || (d2 = DavIntPowSet.bot))
			then Val.make DavIntPowSet.bot (LocPowSet.join l1 l2)
		else Val.make (DavIntPowSet.make (addSets (DavIntPowSet.to_list d1) (DavIntPowSet.to_list d2) [])) (LocPowSet.join l1 l2)
	| NEG(e) -> 
		let d, l = eval m e in 
		Val.make (negSet d) l
	| VAR(x) -> Memory.image m x
	| DEREF(x) -> join_fetch (LocPowSet.to_list (snd (Memory.image m x)))
	| LOC(x) -> Val.make DavIntPowSet.bot (LocPowSet.make [x])
	| READINT -> Val.make DavIntPowSet.top (LocPowSet.bot)

let rec print_state_set = fun s ->
	let slist = StatePowSet.to_list s in
	match slist with
	| [] -> print_string ""
	| h::t ->
	let (i, m) = h in
		print_int i; print_string "\n"; print_mem (Memory.to_list m); print_state_set (StatePowSet.make t)

let next : pgm_graph -> int -> state -> state_set = fun pgm_g max st ->
	let rec next_state_list = fun nlist m ->
		match nlist with
		| [] -> []
		| h::t -> (h, m)::(next_state_list t m)
	in
	let rec assign_vars = fun m xlist v ->
		match xlist with
		| [] -> m
		| h::t -> Memory.join (Memory.weakupdate m h v) (assign_vars m t v)
	in
	let s =
		(match st with
		| StateDomain.BOT -> raise ErrorStateBot
		| StateDomain.ELT(e) -> e
		| StateDomain.TOP -> raise ErrorStateTop)
	in
	let m = get_memory s in
	let n = get_node pgm_g s in
	let cmd = get_command n in
	let next_list = 
		let lst = List.map get_id (next_nodes pgm_g n) in
			match lst with
			| [] -> next_state_list [max]
			| _ -> next_state_list lst 
		in
	match cmd with
	| Assign(x, e) ->
		let v = eval m e in
			StatePowSet.make (next_list (Memory.update m x v))
	| PtrAssign(x, e) ->
		let v = eval m e in
			StatePowSet.make (next_list (assign_vars m (LocPowSet.to_list (snd (Memory.image m x))) v))
	| Assume(e) ->
		let v, _ = eval m e in
		if DavIntPowSet.make [(DavInt.make 0)] = v then StatePowSet.bot
		else if v = DavIntPowSet.bot then StatePowSet.bot
		else StatePowSet.make (next_list m)
	| AssumeNot(e) ->
		let v, _ = eval m e in
		if DavIntPowSet.mem (DavInt.make 0) v then 
			StatePowSet.make (next_list m)
		else if DavIntPowSet.top = v then
			StatePowSet.make (next_list m)
		else StatePowSet.bot
	| Skip -> 
		StatePowSet.make (next_list m)

let get_state = fun t i ->
    match Trace.image t i with
    | StateDomain.BOT -> (i, Memory.bot)
    | StateDomain.ELT(s) -> s
    | StateDomain.TOP -> raise ErrorStateTop

let execute : pgm_graph -> trace = fun pgm_g ->
	let max = (List.length (all_nodes pgm_g) + 1) in
    let rec make_init : node list -> trace = fun lst ->
        match lst with
        | [] ->
		let i = max in
		Trace.weakupdate Trace.bot i (StateDomain.make (i, Memory.bot))
        | h::t ->
        let i = get_id h in
        Trace.weakupdate (make_init t) i (StateDomain.make (i, Memory.bot))
    in
    let rec next_all = fun nlist tr ->
        match nlist with
        | [] -> StatePowSet.bot
        | h::tl ->
        let i = get_id h in
        let s = Trace.image tr i in
            StatePowSet.join (next pgm_g max s) (next_all tl tr)
    in
    let rec partition_join = fun ss tr ->
        let slist = StatePowSet.to_list ss in
        match slist with
        | [] -> tr
        | h::t ->
		let i = fst h in
		partition_join (StatePowSet.make t) (Trace.weakupdate tr i (StateDomain.make h))
    in
    let rec trace_join = fun i max t1 t2 t ->
        let m1 = get_memory (get_state t1 i) in
        let m2 = get_memory (get_state t2 i) in
		let t' = Trace.weakupdate t i (StateDomain.make (i, (Memory.join m1 m2))) in
		if i == max
		then t'
		else trace_join (i+1) max t1 t2 t'
    in
    let rec trace_leq = fun i max t1 t2 ->
        let m1 = get_memory (get_state t1 i) in
        let m2 = get_memory (get_state t2 i) in
		let b = Memory.leq m1 m2 in
		if i == max
		then b
        else (trace_leq (i+1) max t1 t2) && b
    in
    let rec iteration = fun pgm_g t t0 nlist ->
        let t' = t in
        let t = trace_join 1 max t0 (partition_join (next_all nlist t) Trace.bot) Trace.bot in
        if trace_leq 1 max t t' then t' else (iteration pgm_g t t0 nlist)
    in
    let nlist = (all_nodes pgm_g) in
    let t0 = make_init nlist in
        iteration pgm_g t0 t0 nlist
