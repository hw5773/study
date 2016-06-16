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
