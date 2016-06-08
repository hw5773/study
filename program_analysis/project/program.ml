(*
 * SNU 4541.664A Program Analysis
 * Definition of original program
 *)

type var = string 			(* var라는 type을 string으로 정의 *)
module Var =
struct
  type t = string			(* 모듈 Var는 string의 type을 가지고 compare는 compare? *)
  let compare = compare
end
  
type cmd = 
  | ASSIGN of var * exp		(* x := E *)
  | PTRASSIGN of var * exp	(* *x := E *)
  | SEQ of cmd * cmd		(* C; C *)
  | IF of exp * cmd * cmd	(* if E C C *)
  | REPEAT of cmd * exp		(* repeat C E *)
and exp =
  | NUM of int				(* n *)
  | ADD of exp * exp		(* E + E *)
  | NEG of exp				(* ~E *)
  | VAR of var				(* x *)
  | DEREF of var			(* *x *)
  | LOC of var				(* &x *)
  | READINT					(* readInt *)

type program = cmd			(* program 타입은 cmd *)

let rec exp_to_str = function
  | NUM i -> string_of_int i
  | ADD (e1, e2) -> "(" ^ exp_to_str e1 ^ " + " ^ (exp_to_str e2) ^ ")"
  | NEG e -> "-(" ^ exp_to_str e ^ ")"
  | VAR x -> x
  | DEREF x -> "*" ^ x
  | LOC x -> "&" ^ x
  | READINT -> "readInt()"

let rec cmd_to_str = function
  | ASSIGN (x, e) -> x ^ " := " ^ exp_to_str e
  | PTRASSIGN (x, e) -> "*" ^ x ^ " := "^ exp_to_str e
  | SEQ (c1, c2) -> cmd_to_str c1 ^ ";\n" ^ cmd_to_str c2
  | IF (e, c1, c2) -> 
    "if (" ^ exp_to_str e ^ ")" ^ cmd_to_str c1 ^ "\n" ^ cmd_to_str c2
  | REPEAT (c, e) -> 
    "repeat (" ^ cmd_to_str c ^ ") until (" ^ exp_to_str e ^ ")"
