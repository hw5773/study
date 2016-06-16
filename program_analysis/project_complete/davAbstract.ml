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
	type t = Z of int
	val make : int -> t
	val add : t -> t -> t
	val neg : t -> t
	val compare : t -> t -> int
	end = struct
	type t = Z of int

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
	type t = int * memory
	let get_id = fun s -> fst s
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
