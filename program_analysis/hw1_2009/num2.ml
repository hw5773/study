let rec zipper: int list * int list -> int list = fun (l1, l2) ->
        match (l1, l2) with
        | (l1, []) -> l1
        | ([], l2) -> l2
        | (head1::tail1, head2::tail2) -> head1::(head2::(zipper (tail1, tail2)));;
