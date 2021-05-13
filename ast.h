#ifndef _AST_H_
#define _AST_H_

struct ast;

enum ast_type {
  AST_ROOT,
  AST_VARIABLE_LIST,
  AST_OPERATOR_LIST,
  AST_ASSIGN,
  AST_UNARY_OPERATOR,
  AST_BINARY_OPERATOR,
  AST_IDENTIFIER,
  AST_CONSTANT,
  AST_IF,
  AST_LOOP
};

union ast_data {
  struct {
    struct ast *vardecl;
    struct ast *compdesc;
  } root;
};

struct ast {
  union ast_data data;
  enum ast_type type;
};

struct ast *ast_new(enum ast_type type, union ast_data data);
void ast_free(struct ast *ast);

struct ast *ast_new_root            (struct ast *variable_declaration, struct ast *computation_description);
struct ast *ast_new_variable_list   (char const *name, struct ast *next);
struct ast *ast_new_operator_list   (struct ast *operator_, struct ast *next);
struct ast *ast_new_assign          (char const *name, struct ast *expr);
struct ast *ast_new_unary_operator  (int type, struct ast *expr);
struct ast *ast_new_binary_operator (int type, struct ast *expr1, struct ast *expr2);
struct ast *ast_new_identifier      (char const *name);
struct ast *ast_new_constant        (int value);
struct ast *ast_new_if              (struct ast *condition, struct ast *if_true, struct ast *if_false);
struct ast *ast_new_loop            (struct ast *condition, struct ast *while_true);

void ast_free_root            (struct ast *node);
void ast_free_variable_list   (struct ast *node);
void ast_free_operator_list   (struct ast *node);
void ast_free_assign          (struct ast *node);
void ast_free_unary_operator  (struct ast *node);
void ast_free_binary_operator (struct ast *node);
void ast_free_identifier      (struct ast *node);
void ast_free_constant        (struct ast *node);
void ast_free_if              (struct ast *node);
void ast_free_loop            (struct ast *node);

void yyerror(char *s);

#endif
