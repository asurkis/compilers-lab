%{
#include <parser.tab.h>
%}

%%

"+"|"-"|"*"|"/"|">"|"<"|"=" { return yytext[0]; }
":=" { return ASSIGN; }
"Begin" { return BEGIN; }
"End" { return END; }
"Var" { return VAR; }
"AND" { return AND; }
"OR" { return OR; }
"XOR" { return XOR; }
"IF" { return IF; }
"ELSE" { return ELSE; }
"WHILE" { return WHILE; }
"DO" { return DO; }
[a-zA-Z]+ { yylvalue.identifier = yytext; return IDENTIFIER; }
[0-9]+ { yylvalue.constant = atoi(yytext); return CONSTANT; }
[ \t\n] {}

%%
