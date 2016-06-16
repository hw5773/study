(*
 * Abstract Domain in the problem #1
 * This domain is composed of (even/odd integer set * location set)
 *)

open Program
open GraphPgm
open DomFunctor

exception NoPartialOrder
exception ErrorStateBot
exception ErrorStateTop

module EoInt
=
struct
  type t = BOT | ZERO | NONZEROEVEN | EVEN | ODD | TOP
  let bot = BOT
  let top = TOP

  let leq x y = match x, y with
    | (BOT, _) -> true
    | (_, BOT) -> false
    | (_, TOP) -> true
    | (TOP, _) -> false
	| (ZERO, ZERO) -> true
	| (NONZEROEVEN, NONZEROEVEN) -> true
	| (ODD, ODD) -> true
	| (EVEN, EVEN) -> true
	| (ZERO, EVEN) -> true
	| (NONZEROEVEN, EVEN) -> true
	| (EVEN, ZERO) -> false
	| (EVEN, NONZEROEVEN) -> false
	| (_, _) -> raise NoPartialOrder

  let smaller a b = if leq a b then a else b
  let bigger a b = if leq a b then b else a

  let join x y = match x, y with
	| (BOT, _) -> y
	| (_, BOT) -> x
	| (ZERO, NONZEROEVEN) -> EVEN
	| (NONZEROEVEN, ZERO) -> EVEN
	| (ODD, ODD) -> ODD
	| (ODD, _) -> TOP
	| (_, ODD) -> TOP
	| (_, _) -> bigger x y 

  let add = fun v1 v2 ->
	match v1, v2 with
	| (BOT, _) -> BOT
	| (_, BOT) -> BOT
	| (TOP, _) -> TOP
	| (_, TOP) -> TOP
	| (ODD, ODD) -> EVEN
	| (_, ODD) -> ODD
	| (ODD, _) -> ODD
	| (ZERO, ZERO) -> ZERO
	| (ZERO, NONZEROEVEN) -> NONZEROEVEN
	| (NONZEROEVEN, ZERO) -> NONZEROEVEN
	| (_, _) -> EVEN

  let neg = fun v -> v
end

module Loc = Var
module LocSet = PrimitiveSet (Loc)
module LocPowSet = PowerSetDomain (LocSet)
module Val = ProductDomain (EoInt) (LocPowSet)
module Memory = FunDomainFromSet (LocSet) (Val)

type memory = Memory.t
type value = Val.t
type eo = EoInt.t

module State =
struct
	type t = int * memory
	let get_id = fun s -> fst s
	let get_memory = fun s -> snd s
	let compare = fun x y ->
		let (n1, _) = x in
		let (n2, _) = y in
		Pervasives.compare n1 n2
end

module Point = 
struct 
	type t = int
	let compare = Pervasives.compare
end

module StateDomain = FlatDomain(State)
module StateSet = PrimitiveSet (State)
module StatePowSet = PowerSetDomain (StateSet)
module Trace = FunDomain (Point) (StateDomain)

type state = StateDomain.t
type state_set = StatePowSet.t
type trace = Trace.t

let get_node = fun pgm_g s ->
	List.nth (List.rev (all_nodes pgm_g)) ((fst s) - 1)

let get_memory = fun s -> snd s

let rec eval : memory -> exp -> value = fun m e ->
	let rec join_fetch : LocSet.elt list -> value = fun lst ->
		match lst with
		| [] -> Val.make EoInt.bot LocPowSet.bot
		| h::t ->
			let eo1, l1 = Memory.image m h in
			let eo2, l2 = join_fetch t in
			Val.make (EoInt.join eo1 eo2) (LocPowSet.join l1 l2)
	in
 	match e with
	| NUM(i) -> if (i mod 2 == 1) then (Val.make EoInt.ODD LocPowSet.bot)
				else if (i == 0) then (Val.make EoInt.ZERO LocPowSet.bot)
				else (Val.make EoInt.NONZEROEVEN LocPowSet.bot)
	| ADD(e1, e2) -> 
		let eo1, l1 = eval m e1 in
		let eo2, l2 = eval m e2 in 
		Val.make (EoInt.add eo1 eo2) (LocPowSet.join l1 l2) 
	| NEG(v) -> 
		let eo, l = eval m v in
		Val.make (EoInt.neg eo) l
	| VAR(x) -> Memory.image m x
	| DEREF(x) -> join_fetch (LocPowSet.to_list (snd (Memory.image m x))) 
	| LOC(x) -> Val.make EoInt.BOT (LocPowSet.make [x])
	| READINT -> Val.make EoInt.TOP LocPowSet.bot

let rec print_list = fun lst ->
	match lst with
	| [] -> print_string "\n"
	| h::t -> print_string h; print_string "\n";print_list t

let rec print_mem = fun mlist ->
	match mlist with
	| [] -> print_string ""
	| h::t -> 
	let (x, (eo, l)) = h in
	let loc = String.concat " " (LocPowSet.to_list l) in
	match eo with
	| EoInt.BOT -> print_string ("var: " ^ x ^ "  val: nan locs: " ^ loc ^ "\n"); print_mem t
	| EoInt.ODD -> print_string ("var: " ^ x ^ "  val: odd locs: " ^ loc ^ "\n"); print_mem t
	| EoInt.ZERO ->  print_string ("var: " ^ x ^ "  val: zero locs: " ^ loc ^ "\n"); print_mem t
	| EoInt.NONZEROEVEN -> print_string ("var: " ^ x ^ "  val: nonzero locs: " ^ loc ^ "\n"); print_mem t
	| EoInt.EVEN -> print_string ("var: " ^ x ^ "  val: even locs: " ^ loc ^ "\n"); print_mem t
	| EoInt.TOP -> print_string ("var: " ^ x ^ "  val: top locs: " ^ loc ^ "\n"); print_mem t

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
		print_mem (Memory.to_list m);print_trace pgm_g (Trace.make tl)
		| StateDomain.TOP -> raise ErrorStateTop

let rec print_state_set = fun s ->
	let slist = (StatePowSet.to_list s) in
	match slist with
	| [] -> print_string ""
	| h::t ->
	let (n, m) = h in
		print_mem (Memory.to_list m); print_state_set (StatePowSet.make t)

let rec print_val = fun v ->
	let eo, l = v in
	let loc = String.concat " " (LocPowSet.to_list l) in
	match eo with
	| EoInt.BOT -> print_string ("val: nan locs: " ^ loc ^ "\n")
	| EoInt.ODD -> print_string ("val: odd locs: " ^ loc ^ "\n")
	| EoInt.ZERO ->  print_string ("val: zero locs: " ^ loc ^ "\n")
	| EoInt.NONZEROEVEN -> print_string ("val: nonzero locs: " ^ loc ^ "\n")
	| EoInt.EVEN -> print_string ("val: even locs: " ^ loc ^ "\n")
	| EoInt.TOP -> print_string ("val: top locs: " ^ loc ^ "\n")

let next : pgm_graph -> int -> state -> state_set = fun pgm_g max st ->
	let rec next_state_list = fun nlist m ->
		match nlist with
		| [] -> []
		| h::t -> (h, m)::(next_state_list t m)
	in
	let rec assign_vars = fun m xlist v ->
		match xlist with
		| [] -> m
		| h::t -> 
			Memory.join (Memory.weakupdate m h v) (assign_vars m t v)
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
		| _ -> next_state_list lst in
	match cmd with
	| Assign(x, e) -> 
		let v = eval m e in
		StatePowSet.make (next_list (Memory.update m x v))
	| PtrAssign(x, e) ->
		let v = eval m e in
		StatePowSet.make (next_list (assign_vars m (LocPowSet.to_list (snd (Memory.image m x))) v))
	| Assume (e) ->
		let v, _ = eval m e in
		(match v with 
		| EoInt.BOT -> StatePowSet.bot
		| EoInt.ZERO -> StatePowSet.bot
		| EoInt.TOP -> StatePowSet.make (next_list m)
		| _ -> StatePowSet.make (next_list m))
	| AssumeNot (e) ->
		let v, _ = eval m e in
		(match v with
		| EoInt.ZERO -> StatePowSet.make (next_list m)
		| EoInt.NONZEROEVEN -> StatePowSet.make (next_list m)
		| EoInt.TOP -> StatePowSet.make (next_list m)
		| _ -> StatePowSet.bot)
	| Skip -> StatePowSet.make (next_list m)

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
		let i = (List.length (all_nodes pgm_g)) + 1 in
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
	let rec partition_join = fun ss ->
		let slist = StatePowSet.to_list ss in
		match slist with
		| [] -> Trace.bot
		| h::t ->
		let i = fst h in
		Trace.weakupdate (partition_join (StatePowSet.make t)) i (StateDomain.make h) 
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
		let t = trace_join 1 max t0 (partition_join (next_all nlist t)) Trace.bot in
		if trace_leq 1 max t t' then t' else (iteration pgm_g t t0 nlist)
	in
	let nlist = (all_nodes pgm_g) in
	let t0 = make_init nlist in
		iteration pgm_g t0 t0 nlist
