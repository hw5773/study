(*
 * Abstract Domain in the problem #1
 * This domain is composed of (even/odd integer set * location set)
 *)
open Program
open GraphPgm
open DomFunctor

exception NoPartialOrder

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
	type t = node * memory
	let get_node = fun s -> fst s
	let get_memory = fun s -> snd s
	let compare = fun x y ->
		let (n1, _) = x in
		let (n2, _) = y in
		Pervasives.compare n1 n2
end

module Point : ELEM = 
struct 
	type t = int
	let compare = Pervasives.compare
end

module StateDomain = FlatDomain(State)
module StateSet = PrimitiveSet (State)
module StatePowSet = PowerSetDomain (StateSet)
module Trace = FunDomain (Point) (StateDomain)

type state = State.t
type state_set = StatePowSet.t

let get_node : state -> node = fun s -> fst s
let get_memory : state -> memory = fun s -> snd s

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
	| ADD(e1, e2) -> Val.make (EoInt.add (fst (eval m e1)) (fst (eval m e2))) (LocPowSet.make []) 
	| NEG(v) -> Val.make (EoInt.neg (fst (eval m v))) LocPowSet.bot
	| VAR(x) -> Memory.image m x
	| DEREF(x) -> join_fetch (LocPowSet.to_list (snd (Memory.image m x))) 
	| LOC(x) -> Val.make EoInt.BOT (LocPowSet.make [x])
	| READINT -> Val.make EoInt.TOP LocPowSet.bot

let next : pgm_graph -> state -> state_set = fun pgm_g s ->
	let rec next_state_list = fun nlist m ->
		match nlist with
		| [] -> []
		| h::t -> (h, m)::(next_state_list t m)
	in
	let m = get_memory s in
	let n = get_node s in
	let cmd = get_command n in
	match cmd with
	| Assign(v, e) -> StatePowSet.make (next_state_list (next_nodes pgm_g n) (Memory.update m v (eval m e)))
(*
	| PtrAssign(v, e) ->
	| Assume (e) ->
	| AssumeNot (e) ->
*)
	| Skip -> StatePowSet.make (next_state_list (next_nodes pgm_g n) (Memory.update m x (eval m e)))
