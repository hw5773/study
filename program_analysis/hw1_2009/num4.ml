type nat = ZERO | SUCC of nat

let rec natadd: nat * nat -> nat = fun (n1, n2) ->
        match (n1, n2) with
        (ZERO, ZERO) -> ZERO
        | (n, ZERO) -> n
        | (ZERO, n) -> n
        | (n, SUCC(m)) -> natadd (SUCC(n), m);;

let rec natmul: nat * nat -> nat = fun (n1, n2) ->
        match (n1, n2) with
        (_, ZERO) -> ZERO
        | (ZERO, _) -> ZERO
        | (n, SUCC(ZERO)) -> n
        | (SUCC(ZERO), n) -> n
        | (n, SUCC(m)) -> natadd (n, natmul (n, m));;
