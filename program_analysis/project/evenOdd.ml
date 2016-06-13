(*
 * SNU 4541.664A Program Analysis
 * Skeleton for even-odd analyzer
 *)

open Program
open GraphPgm
open EoAbstract
open EoFunction

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
	let start_node = get_start_node pgm_g in
	let _ = print_list pgm_g [start_node] in
	let _ = print_string "Hello\n" in
	let res = VarMap.empty in
	let res = VarMap.add "x" (EVEN, VarSet.empty) res in
	let res = VarMap.add "y" (NONE, VarSet.add "p" VarSet.empty) res in
	res
