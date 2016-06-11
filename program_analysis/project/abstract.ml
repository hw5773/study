(*
 * Abstract Domain in the problem #1
 * This domain is composed of (even/odd integer set * location set)
 *)

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

module ProgramPoint : ELEM =
struct
	type t = int
	let compare = Pervasives.compare
end

module Loc = Var
module LocSet = PrimitiveSet (Loc)
module LocPowSet = PowerSetDomain (LocSet)
module Val = ProductDomain (EoInt) (LocPowSet)
module Memory = FunDomainFromSet (Loc) (Val)
module State = FunDomain (ProgramPoint) (Memory)

type memory = Memory.t
type value = Val.t
type eo = EoInt.t

let rec eval : memory -> exp -> value = fun m e ->
  match e with
	| NUM(i) -> if (i mod 2 == 1) then (Val.make EoInt.ODD (LocPowSet.make []))
				else if (i == 0) then (Val.make EoInt.ZERO (LocPowSet.make []))
				else (Val.make EoInt.NONZEROEVEN (LocPowSet.make []))
	| ADD(e1, e2) -> Val.make (EoInt.add (fst (eval m e1)) (fst (eval m e2))) (LocPowSet.make [])
	| NEG(e) -> Val.make (EoInt.neg (fst (eval m e))) (LocPowSet.make [])
	| VAR(x) -> Memory.image m x
