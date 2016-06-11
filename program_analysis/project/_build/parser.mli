type token =
  | INT of (int)
  | ID of (string)
  | ASSIGN
  | STAR
  | SEMICOLON
  | IF
  | REPEAT
  | PLUS
  | MINUS
  | AMPER
  | EOF
  | READINT
  | LPAREN
  | RPAREN

val main :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Program.cmd
