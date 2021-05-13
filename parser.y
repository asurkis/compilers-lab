%{
#include "ast.h"
#include "lex.yy.h"
#include <stddef.h>
%}

%union {
  struct ast *ast;
  char *identifier;
  int arithmetic_operator;
  int constant;
}

%token T_BEGIN T_END T_VAR T_ASSIGN T_AND T_OR T_XOR T_IF T_ELSE T_WHILE T_DO
%token <identifier> T_IDENTIFIER
%token <constant> T_CONSTANT

%type <arithmetic_operator> binary_operator_pr1
%type <arithmetic_operator> binary_operator_pr2
%type <arithmetic_operator> binary_operator_pr3
%type <arithmetic_operator> binary_operator_pr4
%type <arithmetic_operator> unary_operator
%type <ast> anywhere_operator
%type <ast> block_operator
%type <ast> expr
%type <ast> if_child_operator
%type <ast> if_child_operator_if
%type <ast> if_child_operator_loop
%type <ast> if_header
%type <ast> loop_header
%type <ast> operand
%type <ast> operator
%type <ast> operator_assign
%type <ast> operator_if
%type <ast> operator_list
%type <ast> operator_loop
%type <ast> program
%type <ast> subexpr_pr1
%type <ast> subexpr_pr2
%type <ast> subexpr_pr3
%type <ast> subexpr_pr4
%type <ast> subexpr_pr5
%type <ast> variable_declaration
%type <ast> variable_list

%%

/*
Вариант 11

<Программа> ::= <Объявление переменных> <Описание вычислений>
<Описание вычислений> ::= Begin < Список операторов > End.
<Объявление переменных> ::= Var <Список переменных>
<Список переменных> ::= <Идент>; | <Идент> , <Список переменных> |
<Идент>; <Список переменных>
<Список операторов> ::= <Оператор> | <Оператор> <Список операторов>
<Оператор>::=<Присваивание> |<Сложный оператор>|<Составной оператор>
<Составной оператор>::= Begin < Список операторов > End
<Присваивание> ::= <Идент> := <Выражение>;
<Выражение> ::= <Ун.оп.><Подвыражение> | <Подвыражение>
<Подвыражение> :: = ( <Выражение> ) | <Операнд> |
< Подвыражение > <Бин.оп.> <Подвыражение>
<Ун.оп.> ::= "-"
<Бин.оп.> ::= "-" | "+" | "*" | "/" | ">" | "<" | "=" | "T_AND" | "T_OR" | "T_XOR"
<Операнд> ::= <Идент> | <Const>
|<Сложный оператор>:: =<Оператор цикла> | <Оператор T_IF>
<Оператор T_IF> ::= T_IF "("< Выражение> ")" Оператор|
 T_IF "(" <Выражение> ")" <Оператор> T_ELSE <Оператор>
<Оператор цикла>:: =T_WHILE <Выражение> T_DO <Оператор>
<Идент> ::= <Буква> <Идент> | <Буква>
<Const> ::= <Цифра> <Const> | <Цифра>
*/

program: variable_declaration block_operator '.' { $$ = ast_new_root($1, $2); };

variable_declaration: T_VAR variable_list { $$ = $2; };
variable_list:
  T_IDENTIFIER ';'               { $$ = ast_new_variable_list( $1, NULL ); } |
  T_IDENTIFIER ',' variable_list { $$ = ast_new_variable_list( $1, $3   ); } |
  T_IDENTIFIER ';' variable_list { $$ = ast_new_variable_list( $1, $3   ); };

block_operator: T_BEGIN operator_list T_END { $$ = $2; };
operator_list:
  operator               { $$ = ast_new_operator_list( $1, NULL ); } |
  operator operator_list { $$ = ast_new_operator_list( $1, $2   ); };

operator: anywhere_operator | operator_if | operator_loop;
anywhere_operator: operator_assign | block_operator;
if_child_operator:
  anywhere_operator |
  if_child_operator_if |
  if_child_operator_loop;
operator_assign: T_IDENTIFIER T_ASSIGN expr ';' { $$ = ast_new_assign($1, $3); };

expr: subexpr_pr1 | unary_operator subexpr_pr1 { $$ = ast_new_unary_operator($1, $2); };
subexpr_pr1: subexpr_pr2 | subexpr_pr2 binary_operator_pr1 subexpr_pr1 { $$ = ast_new_binary_operator($2, $1, $3); };
subexpr_pr2: subexpr_pr3 | subexpr_pr3 binary_operator_pr2 subexpr_pr2 { $$ = ast_new_binary_operator($2, $1, $3); };
subexpr_pr3: subexpr_pr4 | subexpr_pr4 binary_operator_pr3 subexpr_pr3 { $$ = ast_new_binary_operator($2, $1, $3); };
subexpr_pr4: subexpr_pr5 | subexpr_pr5 binary_operator_pr4 subexpr_pr4 { $$ = ast_new_binary_operator($2, $1, $3); };
subexpr_pr5: operand | '(' expr ')' { $$ = $2; };
unary_operator: '-' { $$ = '-'; };
binary_operator_pr1:
  '>' { $$ = '>'; } |
  '<' { $$ = '<'; } |
  '=' { $$ = '='; };
binary_operator_pr2:
  T_AND { $$ = T_AND ; } |
  T_OR  { $$ = T_OR  ; } |
  T_XOR { $$ = T_XOR ; };
binary_operator_pr3:
  '+' { $$ = '+'; } |
  '-' { $$ = '-'; };
binary_operator_pr4:
  '*' { $$ = '*'; } |
  '/' { $$ = '/'; };
operand:
  T_IDENTIFIER { $$ = ast_new_identifier($1); } |
  T_CONSTANT { $$ = ast_new_constant($1); };

/*
В исходной грамматике при выражении T_IF ... T_IF ... T_ELSE ...
непонятно, к какому из двух T_IF относится T_ELSE.
Решение: пусть T_ELSE относится ко второму T_IF
*/
if_header: T_IF '(' expr ')' { $$ = $3; };
operator_if:
  if_header          operator                        { $$ = ast_new_if( $1, $2, NULL ); } |
  if_header if_child_operator T_ELSE          operator { $$ = ast_new_if( $1, $2, $4   ); };
if_child_operator_if:
  if_header if_child_operator T_ELSE if_child_operator { $$ = ast_new_if($1, $2, $4); };

loop_header: T_WHILE expr T_DO { $$ = $2; };
operator_loop:          loop_header          operator { $$ = ast_new_loop($1, $2); };
if_child_operator_loop: loop_header if_child_operator { $$ = ast_new_loop($1, $2); };

%%
