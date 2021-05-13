#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

struct ast *ast_new(enum ast_type type, union ast_data data) {
  return NULL;
}

void ast_free(struct ast *ast) {}

struct ast *ast_new_root(struct ast *variable_declaration,
                         struct ast *computation_description) {
  return NULL;
}

struct ast *ast_new_variable_list(char const *name, struct ast *next) {
  return NULL;
}

struct ast *ast_new_operator_list(struct ast *operator_, struct ast *next) {
  return NULL;
}

struct ast *ast_new_assign(char const *name, struct ast *expr) {
  return NULL;
}

struct ast *ast_new_unary_operator(int type, struct ast *expr) {
  return NULL;
}

struct ast *ast_new_binary_operator(int type, struct ast *expr1,
                                    struct ast *expr2) {
  return NULL;
}

struct ast *ast_new_identifier(char const *name) {
  return NULL;
}

struct ast *ast_new_constant(int value) {
  return NULL;
}

struct ast *ast_new_if(struct ast *condition, struct ast *if_true,
                       struct ast *if_false) {
  return NULL;
}

struct ast *ast_new_loop(struct ast *condition, struct ast *while_true) {
  return NULL;
}

void ast_free_root(struct ast *node) {}
void ast_free_variable_list(struct ast *node) {}
void ast_free_operator_list(struct ast *node) {}
void ast_free_assign(struct ast *node) {}
void ast_free_unary_operator(struct ast *node) {}
void ast_free_binary_operator(struct ast *node) {}
void ast_free_identifier(struct ast *node) {}
void ast_free_constant(struct ast *node) {}
void ast_free_if(struct ast *node) {}
void ast_free_loop(struct ast *node) {}

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }
