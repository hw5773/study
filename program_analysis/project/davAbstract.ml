(*
 * Abstract Domain in the problem #2
 * This domain is composed of (mod 1867 space)
 *)

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

module ProgramPoint : ELEM =
struct
	type t = int
	let compare = Pervasives.compare
end

module Loc = Var
module LocSet = PrimitiveSet (Loc)
module LocPowSet = PowerSetDomain (LocSet)
module DavIntSet = PrimitiveSet (DavInt)
module DavIntPowSet = PowerSetDomain (DavIntSet)
module Val = ProductDomain (DavIntPowSet) (LocPowSet)
module Memory = FunDomainFromSet (LocSet) (Val)
module State = FunDomain (ProgramPoint) (Memory)

type memory = Memory.t
type d = DavInt.t
type dav = DavIntSet.t
type value = Val.t
type state = State.t

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

