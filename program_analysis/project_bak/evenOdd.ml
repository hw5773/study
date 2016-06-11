(*
 * SNU 4541.664A Program Analysis
 * Skeleton for even-odd analyzer
 *)

open Program
open GraphPgm
open Abstract

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
      (Printf.sprintf "%s --> (%s, %s)" x num_str loc_str) ^ accum_str
    ) res ""

(* TODO: you have to implement from here *)
(*
let print_cmd: node -> unit = fun n ->
	let cmd_str = command_to_str n in
	Printf.sprintf "%s\n" cmd_str

let rec test: G.vertex list -> string = fun vlist ->
	match vlist with
	| [] -> ""
	| h::t -> String.concat "\n" (List.append [(test t)] [(String.concat ") " ((string_of_int (Node.get_id h))::[(command_to_str (Node.get_command h))]))])
*)
(*	| h::t -> String.concat " " ((command_to_str (Node.get_command h))::[(test t)]) *)

type eo = EoInt.t
type v = Val.t
type memory = Memory.t

let int_map: int -> eo = fun x ->
	if x == 0 then ZERO else if x mod 2 == 0 then NONZEROEVEN else ODD

let rec eval: (memory * exp) -> v = fun m e ->
	match e with
	| NUM(n) -> ((int_map n), bot)
	| ADD(e1, e2) -> (fst(eval(m, e1)) + fst(eval(m, e2)), bot)
	| NEG(e) -> (fst(eval(m, e)), bot)
	| VAR(x) -> image ELT(x) m
	| DEREF(x) -> (bot, bot) (* Need to revise *)
	| LOC(x) -> (bot, bot)   (* Need to revise *)
	| READINT -> (top, bot)

let eoAnalyzer: pgm_graph -> result = fun pgm_g ->
	let res = VarMap.empty in let res = VarMap.add "y" (EVEN, VarSet.empty) res in res
