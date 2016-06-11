(*
 * Abstract Domain in the problem #1
 * This domain is composed of (even/odd integer set * location set)
 *)

open Program
open DomFunctor
exception NoPartialOrder

module EoInt : DOMAIN
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

end

module ProgramPoint : DOMAIN
=
struct
  type t = BOT | Z of int | TOP
  let bot = BOT
  let top = TOP

  let leq x y = match x, y with
	| (BOT, _) -> true
	| (_, BOT) -> false
	| (_, TOP) -> true
	| (TOP, _) -> false
	| (_, _) -> raise NoPartialOrder

  let smaller a b = if a < b then a else b
  let bigger a b = if a < b then b else a

  let join x y = match x, y with
	| (BOT, _) -> y
	| (_, BOT) -> x
	| (_, _) -> TOP
end

module Loc = Var
module LocSet = PrimitiveSet (Loc)
module LocPowSet = PowerSetDomain (LocSet)
module Val = ProductDomain (EoInt) (LocPowSet)
module Memory = FunDomainFromSet (LocSet) (Val)
module State = ProductDomain (ProgramPoint) (Memory)
