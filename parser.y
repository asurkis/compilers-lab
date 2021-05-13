%{

%}

%union {
  char *identifier;
  int constant;
}

%token BEGIN END VAR ASSIGN AND OR XOR IF ELSE WHILE DO
%token <identifier> IDENTIFIER
%token <constant> CONSTANT

%%

/*
Вариант 11

<Программа> ::= <Объявление переменных> <Описание вычислений>
<Описание вычислений> ::= Begin < Список операторов > End.
<Объявление переменных> ::= Var <Список переменных>
<Список переменных> ::= <Идент>; | <Идент> , <Список переменных> |
<Идент> ; <Список переменных>
<Список операторов> ::= <Оператор> | <Оператор> <Список операторов>
<Оператор>::=<Присваивание> |<Сложный оператор>|<Составной оператор>
<Составной оператор>::= Begin < Список операторов > End
<Присваивание> ::= <Идент> := <Выражение> ;
<Выражение> ::= <Ун.оп.><Подвыражение> | <Подвыражение>
<Подвыражение> :: = ( <Выражение> ) | <Операнд> |
< Подвыражение > <Бин.оп.> <Подвыражение>
<Ун.оп.> ::= "-"
<Бин.оп.> ::= "-" | "+" | "*" | "/" | ">" | "<" | "=" | "AND" | "OR" | "XOR"
<Операнд> ::= <Идент> | <Const>
|<Сложный оператор>:: =<Оператор цикла> | <Оператор IF>
<Оператор IF> ::= IF "("< Выражение> ")" Оператор|
 IF "(" <Выражение> ")" <Оператор> ELSE <Оператор>
<Оператор цикла>:: =WHILE <Выражение> DO <Оператор>
<Идент> ::= <Буква> <Идент> | <Буква>
<Const> ::= <Цифра> <Const> | <Цифра>
*/

program: variable_declaration block_operator '.';
// computation_description: BEGIN operator_list END '.';
variable_declaration: VAR variable_list;
variable_list: IDENTIFIER ';'
  | IDENTIFIER ',' variable_list
  | IDENTIFIER ';' variable_list
  ;
operator_list: operator
  | operator operator_list
  ;
operator: anywhere_operator | operator_if | operator_loop;
anywhere_operator: operator_assign | block_operator;
if_child_operator: anywhere_operator | if_child_operator_if | if_child_operator_loop;
block_operator: BEGIN operator_list END;
operator_assign: IDENTIFIER ASSIGN expr ';';
expr: unary_operator subexpr_pr0
  | subexpr_pr0;
// subexpr: '(' expr ')'
//   | operand
//   | subexpr binary_operator subexpr
//   ;
subexpr_pr0: subexpr_pr1 | subexpr_pr1 binary_operator_pr0 subexpr_pr0;
subexpr_pr1: subexpr_pr2 | subexpr_pr2 binary_operator_pr1 subexpr_pr1;
subexpr_pr2: subexpr_pr3 | subexpr_pr3 binary_operator_pr2 subexpr_pr2;
subexpr_pr3: subexpr_pr4 | subexpr_pr4 binary_operator_pr3 subexpr_pr3;
subexpr_pr4: '(' expr ')' | operand;
unary_operator: '-';
// binary_operator: '-' | '+' | '*' | '/' | '>' | '<' | '=' | AND | OR | XOR;
binary_operator_pr0: '>' | '<' | '=';
binary_operator_pr1: AND | OR | XOR;
binary_operator_pr2: '+' | '-';
binary_operator_pr3: '*' | '/';
operand: IDENTIFIER | CONSTANT;
// complex_operator: operator_loop | operator_if;

/*
В исходной грамматике при выражении IF ... IF ... ELSE ...
непонятно, к какому из двух IF относится ELSE.
Решение: пусть ELSE относится ко второму IF
*/
// operator_if: IF '(' expr ')' operator
//   | IF '(' expr ')' operator ELSE operator
//   ;
if_header: IF '(' expr ')';
operator_if: if_header operator
  | if_header if_child_operator ELSE operator;
if_child_operator_if: if_header if_child_operator ELSE if_child_operator;

loop_header: WHILE expr DO;
operator_loop: loop_header operator;
if_child_operator_loop: loop_header if_child_operator;

%%
