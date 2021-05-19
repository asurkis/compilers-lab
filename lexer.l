%{
#include "ast.h"
#include "parser.tab.h"
%}

%%

[-+*/>=<,;().] { return yytext[0] ; }
":="           { return T_ASSIGN  ; }
"Begin"        { return T_BEGIN   ; }
"End"          { return T_END     ; }
"Var"          { return T_VAR     ; }
"AND"          { return T_AND     ; }
"OR"           { return T_OR      ; }
"XOR"          { return T_XOR     ; }
"IF"           { return T_IF      ; }
"ELSE"         { return T_ELSE    ; }
"WHILE"        { return T_WHILE   ; }
"DO"           { return T_DO      ; }
[a-zA-Z]+ {
  size_t len = strlen(yytext);
  yylval.identifier = malloc(len + 1);
  strcpy(yylval.identifier, yytext);
  return T_IDENTIFIER; }
[0-9]+ {
  yylval.constant = atoi(yytext);
  return T_CONSTANT; }
[ \t\n] {}
\/\*.*\*\/ {}

%%
