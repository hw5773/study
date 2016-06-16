let rec addSets : DavInt.t list -> DavInt.t list -> DavInt.t list -> DavInt.t list = fun l1 l2 r ->
        let rec update_result = fun l r ->
                match l with
                | [] -> r
                | h::t -> if (List.mem h r) then (update_result t r) else (update_result t (List.sort DavInt.compare (h::r)))
        in
        match l1 with
        | [] -> r
        | h::t -> addSets t l2 (update_result (List.map (DavInt.add h) l2) r);;

