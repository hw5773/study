/* quiz */
a := 1;
b := 2;
x := readInt;
if (x) (p := &a) (p := 10);
if (x) (p := &b) (p := p + 1);
c := *p /* c must point to {b}, not {a, b}. Why? */

/* c is even */
