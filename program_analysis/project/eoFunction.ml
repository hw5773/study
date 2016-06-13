open GraphPgm

let rec print_list : pgm_graph -> node list -> unit = fun pgm_g lst ->
	match lst with
	| [] -> ()
	| h::t -> print_string (command_to_str (get_command h)); print_string "\n"; print_list pgm_g (List.append t (next_nodes pgm_g h))
