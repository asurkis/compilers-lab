#include "ast.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define AST_CAST(ast, type) ((type *)((char *)ast - offsetof(type, base)))
#define AST_CAST_SELF(type)                                                    \
  struct ast_##type *self = AST_CAST(node, struct ast_##type);

#define TRAVERSE_PRINT_INDENT                                                  \
  for (i = 0; i < 4 * indent; ++i)                                             \
    putchar(' ');

#define TRAVERSE_PRINT_HEADER(type)                                            \
  AST_CAST_SELF(type)                                                          \
  int i;                                                                       \
  TRAVERSE_PRINT_INDENT;

void ast_traverse_print_root(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(root);
  printf("var\n");
  ast_traverse_print(self->variable_declaration, indent + 1);
  ast_traverse_print(self->computation_description, indent + 1);
  printf(".");
}

void ast_traverse_print_variable_list(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(variable_list);
  if (self->next) {
    printf("%s,\n", self->name);
    ast_traverse_print(self->next, indent);
  } else {
    printf("%s;\n", self->name);
  }
}

void ast_traverse_print_operator_list(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(operator_list);
}

void ast_traverse_print_assign(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(assign);
}

void ast_traverse_print_unary_operator(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(unary_operator);
}

void ast_traverse_print_binary_operator(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(binary_operator);
}

void ast_traverse_print_identifier(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(identifier);
}

void ast_traverse_print_constant(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(constant);
}

void ast_traverse_print_if(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(if);
}

void ast_traverse_print_loop(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(loop);
}

void ast_traverse_translate_root(struct ast *node,
                                 struct translate_context *context) {
  AST_CAST_SELF(root);
}

void ast_traverse_translate_variable_list(struct ast *node,
                                          struct translate_context *context) {
  AST_CAST_SELF(variable_list);
}

void ast_traverse_translate_operator_list(struct ast *node,
                                          struct translate_context *context) {
  AST_CAST_SELF(operator_list);
}

void ast_traverse_translate_assign(struct ast *node,
                                   struct translate_context *context) {
  AST_CAST_SELF(assign);
}

void ast_traverse_translate_unary_operator(struct ast *node,
                                           struct translate_context *context) {
  AST_CAST_SELF(unary_operator);
}

void ast_traverse_translate_binary_operator(struct ast *node,
                                            struct translate_context *context) {
  AST_CAST_SELF(binary_operator);
}

void ast_traverse_translate_identifier(struct ast *node,
                                       struct translate_context *context) {
  AST_CAST_SELF(identifier);
}

void ast_traverse_translate_constant(struct ast *node,
                                     struct translate_context *context) {
  AST_CAST_SELF(constant);
}

void ast_traverse_translate_if(struct ast *node,
                               struct translate_context *context) {
  AST_CAST_SELF(if);
}

void ast_traverse_translate_loop(struct ast *node,
                                 struct translate_context *context) {
  AST_CAST_SELF(loop);
}

void ast_free_root(struct ast *node) { AST_CAST_SELF(root); }
void ast_free_variable_list(struct ast *node) { AST_CAST_SELF(variable_list); }
void ast_free_operator_list(struct ast *node) { AST_CAST_SELF(operator_list); }
void ast_free_assign(struct ast *node) { AST_CAST_SELF(assign); }
void ast_free_unary_operator(struct ast *node) {
  AST_CAST_SELF(unary_operator);
}
void ast_free_binary_operator(struct ast *node) {
  AST_CAST_SELF(binary_operator);
}
void ast_free_identifier(struct ast *node) { AST_CAST_SELF(identifier); }
void ast_free_constant(struct ast *node) { AST_CAST_SELF(constant); }
void ast_free_if(struct ast *node) { AST_CAST_SELF(if); }
void ast_free_loop(struct ast *node) { AST_CAST_SELF(loop); }

#define AST_DEFINE_TYPE_1(type, type_enum, type1, arg1)                        \
  static struct ast_metatable ast_metatable_##type = {                         \
      &ast_free_##type,                                                        \
      &ast_traverse_print_##type,                                              \
      &ast_traverse_translate_##type,                                          \
      type_enum,                                                               \
  };                                                                           \
  struct ast *ast_new_##type(type1 arg1) {                                     \
    struct ast_##type *new_ = malloc(sizeof(struct ast_##type));               \
    if (!new_)                                                                 \
      return NULL;                                                             \
    new_->base.metatable = &ast_metatable_##type;                              \
    new_->arg1 = arg1;                                                         \
    return &new_->base;                                                        \
  }

#define AST_DEFINE_TYPE_2(type, type_enum, type1, arg1, type2, arg2)           \
  static struct ast_metatable ast_metatable_##type = {                         \
      &ast_free_##type,                                                        \
      &ast_traverse_print_##type,                                              \
      &ast_traverse_translate_##type,                                          \
      type_enum,                                                               \
  };                                                                           \
  struct ast *ast_new_##type(type1 arg1, type2 arg2) {                         \
    struct ast_##type *new_ = malloc(sizeof(struct ast_##type));               \
    if (!new_)                                                                 \
      return NULL;                                                             \
    new_->base.metatable = &ast_metatable_##type;                              \
    new_->arg1 = arg1;                                                         \
    new_->arg2 = arg2;                                                         \
    return &new_->base;                                                        \
  }

#define AST_DEFINE_TYPE_3(type, type_enum, type1, arg1, type2, arg2, type3,    \
                          arg3)                                                \
  static struct ast_metatable ast_metatable_##type = {                         \
      &ast_free_##type,                                                        \
      &ast_traverse_print_##type,                                              \
      &ast_traverse_translate_##type,                                          \
      type_enum,                                                               \
  };                                                                           \
  struct ast *ast_new_##type(type1 arg1, type2 arg2, type3 arg3) {             \
    struct ast_##type *new_ = malloc(sizeof(struct ast_##type));               \
    if (!new_)                                                                 \
      return NULL;                                                             \
    new_->base.metatable = &ast_metatable_##type;                              \
    new_->arg1 = arg1;                                                         \
    new_->arg2 = arg2;                                                         \
    new_->arg3 = arg3;                                                         \
    return &new_->base;                                                        \
  }

AST_DEFINE_TYPE_2(root, AST_ROOT, struct ast *, variable_declaration,
                  struct ast *, computation_description);
AST_DEFINE_TYPE_2(variable_list, AST_VARIABLE_LIST, char const *, name,
                  struct ast *, next);
AST_DEFINE_TYPE_2(operator_list, AST_OPERATOR_LIST, struct ast *, op,
                  struct ast *, next);

AST_DEFINE_TYPE_2(assign, AST_ASSIGN, char const *, name, struct ast *, expr);
AST_DEFINE_TYPE_2(unary_operator, AST_UNARY_OPERATOR, struct ast *, expr, int,
                  type);
AST_DEFINE_TYPE_3(binary_operator, AST_BINARY_OPERATOR, struct ast *, expr1,
                  struct ast *, expr2, int, type);
AST_DEFINE_TYPE_1(identifier, AST_IDENTIFIER, char const *, name);
AST_DEFINE_TYPE_1(constant, AST_CONSTANT, int, value);
AST_DEFINE_TYPE_3(if, AST_IF, struct ast *, condition, struct ast *, if_true,
                  struct ast *, if_false);
AST_DEFINE_TYPE_2(loop, AST_LOOP, struct ast *, condition, struct ast *,
                  while_true);

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }
