(*
 * Abstract Domain in the problem #2
 * This domain is composed of (mod 1867 space)
 *)

open Program
open GraphPgm
open DomFunctor

exception NoPartialOrder
exception ErrorStateBot
exception ErrorStateTop

module DavInt : sig
	type t = Z of davint
	and davint = private int
	val make : int -> t
	val add : t -> t -> t
	val neg : t -> t
	val compare : t -> t -> int
	end = struct
	type t = Z of davint
	and davint = int

	let compare = fun x y ->
		let Z(e1) = x in 
		let Z(e2) = y in
			if e1 < e2 then -1 
			else if e1 == e2 then 0 
			else 1
	let make = fun x ->
		let t = if x >= 0 then x mod 1867 else (1867 + (x mod 1867)) in
			Z(t)
	let add = fun x y ->
		let Z(e1) = x in
		let Z(e2) = y in
			Z((e1 + e2) mod 1867)

	let neg = fun x ->
		let Z(e) = x in
			Z(1867-e)
end

module Loc = Var
module LocSet = PrimitiveSet (Loc)
module LocPowSet = PowerSetDomain (LocSet)
module DavIntSet = PrimitiveSet (DavInt)
module DavIntPowSet = PowerSetDomain (DavIntSet)
module Val = ProductDomain (DavIntPowSet) (LocPowSet)
module Memory = FunDomainFromSet (LocSet) (Val)

type memory = Memory.t
type d = DavInt.t
type dav = DavIntSet.t
type value = Val.t

module State = 
struct
	type t = node * memory
	let get_node = fun s -> fst s
	let get_memory = fun s -> snd s
	let compare = fun x y ->
		let (n1, _) = x in
		let (n2, _) = y in 
		Pervasives.compare n1 n2
end

module Point  = 
struct
	type t = int
	let compare = Pervasives.compare
end

module StateDomain = FlatDomain(State)
module StateSet = PrimitiveSet(State)
module StatePowSet = PowerSetDomain(StateSet)
module Trace = FunDomain (Point) (StateDomain)

type state = StateDomain.t
type state_set = StatePowSet.t
type trace = Trace.t

let get_node = fun s -> fst s
let get_memory = fun s -> snd s

let rec eval : memory -> exp -> value = fun m e ->
	let rec addSet = fun d dset ->
		let lst = (DavIntPowSet.to_list dset) in
		match lst with
		| [] -> DavIntPowSet.bot
		| h::t -> DavIntPowSet.join (DavIntPowSet.make [(DavInt.add d h)]) (addSet d (DavIntPowSet.make t))
	in
	let rec addSets = fun d1 d2 ->
		let lst = (DavIntPowSet.to_list d1) in
		match lst with
		| [] -> d2
		| h::t -> DavIntPowSet.join (addSet h d2) (addSets (DavIntPowSet.make t) d2)
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
	| ADD(e1, e2) -> Val.make (addSets (fst (eval m e1)) (fst (eval m e2))) (LocPowSet.bot)
	| NEG(e) -> Val.make (negSet (fst (eval m e))) (LocPowSet.bot)
	| VAR(x) -> Memory.image m x
	| DEREF(x) -> join_fetch (LocPowSet.to_list (snd (Memory.image m x)))
	| LOC(x) -> Val.make DavIntPowSet.bot (LocPowSet.make [x])
	| READINT -> Val.make (DavIntPowSet.top) (LocPowSet.bot)

let next : pgm_graph -> state -> state_set = fun pgm_g st ->
	let rec next_state_list = fun nlist m ->
		match nlist with
		| [] -> []
		| h::t -> (h, m)::(next_state_list t m)
	in
	let rec assign_vars = fun m xlist v ->
		match xlist with
		| [] -> Memory.bot
		| h::t -> Memory.join (Memory.weakupdate m h v) (assign_vars m t v)
	in
	let s =
		(match st with
		| StateDomain.BOT -> raise ErrorStateBot
		| StateDomain.ELT(e) -> e
		| StateDomain.TOP -> raise ErrorStateTop)
	in
	let m = get_memory s in
	let n = get_node s in
	let cmd = get_command n in
	let next_list = next_state_list (next_nodes pgm_g n) in
	match cmd with
	| Assign(x, e) ->
		let v = eval m e in
		StatePowSet.make (next_list (assign_vars m [x] v))
	| PtrAssign(x, e) ->
		let v = eval m e in
		StatePowSet.make (next_list (assign_vars m (LocPowSet.to_list (snd (Memory.image m x))) v))
	| Assume(e) ->
		let v, _ = eval m e in
		if DavIntPowSet.mem (DavInt.make 0) v then StatePowSet.bot 
		else if v == DavIntPowSet.bot then StatePowSet.bot
		else StatePowSet.make (next_list m)
	| AssumeNot(e) ->
		let v, _ = eval m e in
		if DavIntPowSet.mem (DavInt.make 0) v then StatePowSet.make (next_list m)
		else StatePowSet.bot
	| Skip -> StatePowSet.make (next_state_list (next_nodes pgm_g n) m)

let get_state = fun t h i ->
    match Trace.image t i with
    | StateDomain.BOT -> (h, Memory.bot)
    | StateDomain.ELT(s) -> s
    | StateDomain.TOP -> raise ErrorStateTop

let execute : pgm_graph -> trace = fun pgm_g ->
    let rec make_init : node list -> trace = fun lst ->
        match lst with
        | [] -> Trace.bot
        | h::t ->
        let i = get_id h in
        Trace.weakupdate (make_init t) i (StateDomain.make (h, Memory.bot))
    in
    let rec next_all = fun nlist tr ->
        match nlist with
        | [] -> StatePowSet.bot
        | h::tl ->
        let i = get_id h in
        let s = Trace.image tr i in
            StatePowSet.join (next pgm_g s) (next_all tl tr)
    in
    let rec partition_join = fun ss ->
        let slist = StatePowSet.to_list ss in
        match slist with
        | [] -> Trace.bot
        | h::t ->
        let i = get_id (fst h) in
        Trace.weakupdate (partition_join (StatePowSet.make t)) i (StateDomain.make h)
    in
    let rec trace_join = fun nlist t1 t2 ->
        match nlist with
        | [] -> Trace.bot
        | h::t ->
        let i = get_id h in
        let s1 = get_state t1 h i in
        let s2 = get_state t2 h i in
        let m1 = get_memory s1 in
        let m2 = get_memory s2 in
        Trace.weakupdate (trace_join t t1 t2) i (StateDomain.make (h, (Memory.join m1 m2)))
    in
    let rec trace_leq = fun nlist t1 t2 ->
        match nlist with
        | [] -> true
        | h::t ->
        let i = get_id h in
        let m1 = get_memory (get_state t1 h i) in
        let m2 = get_memory (get_state t2 h i) in
        (trace_leq t t1 t2) && (Memory.leq m1 m2)
    in
    let rec iteration = fun pgm_g t t0 nlist ->
        let t' = t in
        let t = trace_join nlist t0 (partition_join (next_all nlist t)) in
        if trace_leq nlist t t' then t' else (iteration pgm_g t t0 nlist)
    in
    let nlist = List.rev (all_nodes pgm_g) in
    let t0 = make_init nlist in
        iteration pgm_g t0 t0 nlist
