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
// operator: assignment_operator | complex_operator | block_operator;
operator: operator_ifelse_child | operator_not_ifelse_child;
operator_ifelse_child: assignment_operator | operator_if_else | operator_loop_ifelse_child | block_operator;
operator_not_ifelse_child: operator_loop_not_ifelse_child | operator_if_noelse;
block_operator: BEGIN operator_list END;
assignment_operator: IDENTIFIER ASSIGN expr ';';
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
// operator_if: operator_if_else | operator_if_noelse;
operator_if_else: IF '(' expr ')' operator_ifelse_child ELSE operator;
operator_if_noelse: IF '(' expr ')' operator;
// operator_loop: operator_loop_ifelse_child | operator_loop_not_ifelse_child;
operator_loop_ifelse_child: WHILE expr DO operator_ifelse_child;
operator_loop_not_ifelse_child: WHILE expr DO operator_not_ifelse_child;
// identifier: LETTER identifier | LETTER;
// constant: DIGIT constant | DIGIT;

%%
