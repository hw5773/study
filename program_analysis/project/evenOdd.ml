(*
 * SNU 4541.664A Program Analysis
 * Skeleton for even-odd analyzer
 *)

open Program
open GraphPgm
open EoAbstract

(* Type definitions (Do not modify) *)

module VarMap = Map.Make(Var)
module VarSet = Set.Make(Var)

type num_result =
  | NONE
  | ODD
  | EVEN
  | DONTKNOW

type loc_result = VarSet.t

type result = (num_result * loc_result) VarMap.t

(* Stringfy functions *)

let num_result_to_str = function
  | NONE -> "no number"
  | ODD -> "odd"
  | EVEN -> "even"
  | DONTKNOW -> "don't know"

let loc_result_to_str res =
  "{" ^ (String.concat ", " (VarSet.elements res)) ^ "}"

let result_to_str : result -> string = fun res ->
  VarMap.fold 
    (fun x (num_res, loc_res) accum_str ->
      let num_str = num_result_to_str num_res in
      let loc_str = loc_result_to_str loc_res in
      (Printf.sprintf "%s --> (%s, %s)\n" x num_str loc_str) ^ accum_str
    ) res ""

(* TODO: you have to implement from here *)

let eoAnalyzer : pgm_graph -> result = fun pgm_g ->
	let rec add_loc = fun x eo llist res ->
		let loc = if (VarMap.mem x res) then (snd (VarMap.find x res)) else VarSet.empty in
		let num = 
			match eo with
			| EoInt.BOT -> NONE
			| EoInt.ODD -> ODD
			| EoInt.ZERO -> EVEN
			| EoInt.NONZEROEVEN -> EVEN
			| EoInt.EVEN -> EVEN
			| EoInt.TOP -> DONTKNOW
		in
		match llist with
		| [] -> VarMap.add x (num, loc) res
		| h::t -> add_loc x eo t (VarMap.add x (num, VarSet.add h loc) res)
	in
	let rec update_result = fun xlist m res ->
		match xlist with
		| [] -> res
		| h::t -> 
		let (eo, llist) = Memory.image m h in
			update_result t m (add_loc h eo (LocPowSet.to_list llist) res)
	in
	let rec make_result = fun res tr nlist ->
		match nlist with
		| [] -> res
		| h::t ->
			let i = get_id h in
			let m = get_memory (get_state tr h i) in
			let xlist = List.map fst (Memory.to_list m) in
				make_result (update_result xlist m res) tr t
	in
	let t = execute pgm_g in
	let _ = print_trace t in
	let res = VarMap.empty in
	let nlist = List.rev (all_nodes pgm_g) in
	let res = make_result res t nlist in
	res
