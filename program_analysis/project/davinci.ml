(*
 * SNU 4541.664A Program Analysis
 * Skeleton code for davinci-code analyzer
 *)

open Program
open GraphPgm
open DavAbstract
open DavFunction

(* Type definitions (Do not modify) *)

type result = 
  | YES 
  | NO 
  | DONT_KNOW

(* Stringfy functions *)
let result_to_str : result -> string = fun a -> match a with
  | YES -> "YES, this is davinci code"
  | NO  -> "NO, this is NOT davinci code"
  | DONT_KNOW -> "I don't know"

(* TODO: you have to implement from here *)
module VarMap = Map.Make(Var)
module VarSet = Set.Make(Var)

let daVinciAnalyzer : pgm_graph -> result = fun pgm_g ->
	let max = (List.length (all_nodes pgm_g)) + 1 in
	let rec check_list = fun xlist m dav ->
		match xlist with
		| [] -> dav
		| h::t -> 
		let (d, l) = Memory.image m h in
		if d = DavIntPowSet.top
		then check_list t m (VarMap.add h DONT_KNOW dav)
		else if (DavIntPowSet.mem (DavInt.make 415) d) && ((List.length (DavIntPowSet.to_list d)) == 1) 
			then check_list t m (VarMap.add h YES dav)
		else if (DavIntPowSet.mem (DavInt.make 415) d) && ((List.length (DavIntPowSet.to_list d)) != 1)
			then check_list t m (VarMap.add h DONT_KNOW dav)
		else check_list t m (VarMap.add h NO dav)
	in
	let rec check_davinci = fun i max t dav ->
		let m = get_memory (get_state t i) in
		let xlist = List.map fst (Memory.to_list m) in
		if i == max
		then check_list xlist m dav
		else
			check_davinci (i+1) max t (check_list xlist m dav)
	in		
    let rec make_result = fun dav ->
		let dlist = List.map snd (VarMap.bindings dav) in
		if List.mem YES dlist
		then YES
		else if List.mem DONT_KNOW dlist
		then DONT_KNOW
		else NO
    in
    let t = execute pgm_g in
	let _ = print_trace pgm_g t in
    let dav = VarMap.empty in
	let dav = check_davinci 1 max t dav in 
    let res = make_result dav in
	res
