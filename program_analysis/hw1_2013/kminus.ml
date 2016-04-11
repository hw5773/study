module type KMINUS =
sig
	exception Error of string
	type id = string
	type exp = NUM of int
			| VAR of id
			| ADD of exp * exp
			| MINUS of exp
	type cmd = SKIP
			| SEQ of cmd * cmd
			| IF of exp * cmd * cmd
			| ASSIGN of id * exp
			| WHILE of exp * cmd
end

module type SEM =
sig
	type id = string
	type exp = VAR of id
			| NUM of int
			| LAM of id * exp
			| APP of exp * exp
			| IF of exp * exp * exp
			| FIX of exp
			| NOTEQZ of exp
			| ADD of exp * exp
			| MINUS of exp
			| UPDATE of exp * id * exp
end
