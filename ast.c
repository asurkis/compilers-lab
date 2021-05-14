#include "ast.h"
#include "parser.tab.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define AST_CAST(ast, type) ((type *)((char *)ast - offsetof(type, base)))
#define AST_CAST_SELF(type)                                                    \
  struct ast_##type *self = AST_CAST(node, struct ast_##type);

#define TRAVERSE_PRINT_INDENT                                                  \
  for (i = 0; i < indent; ++i)                                                 \
    fputs("|   ", stdout);

#define TRAVERSE_PRINT_HEADER(type)                                            \
  AST_CAST_SELF(type)                                                          \
  int i;                                                                       \
  TRAVERSE_PRINT_INDENT;

static void ast_traverse_print_root(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(root);
  printf("var (\n");
  ast_traverse_print(self->variable_declaration, indent + 1);

  TRAVERSE_PRINT_INDENT
  printf(") {\n");
  ast_traverse_print(self->computation_description, indent + 1);

  TRAVERSE_PRINT_INDENT
  printf("}\n");
}

static void ast_traverse_print_variable_list(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(variable_list);
  if (self->next) {
    printf("[%s],\n", self->name);
    ast_traverse_print(self->next, indent);
  } else {
    printf("[%s];\n", self->name);
  }
}

static void ast_traverse_print_operator_list(struct ast *node, int indent) {
  AST_CAST_SELF(operator_list);
  ast_traverse_print(self->op, indent);
  if (self->next)
    ast_traverse_print(self->next, indent);
}

static void ast_traverse_print_assign(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(assign);
  printf("[%s] :=\n", self->name);
  ast_traverse_print(self->expr, indent + 1);
}

static char *operator_type_str(int type) {
  switch (type) {
  case '+':
    return "plus";
  case '-':
    return "minus";
  case '*':
    return "multiply";
  case '/':
    return "divide";
  case '>':
    return "greater than";
  case '=':
    return "equal";
  case '<':
    return "less than";
  case T_AND:
    return "logical and";
  case T_OR:
    return "logical or";
  case T_XOR:
    return "logical xor";
  }
  return "!unknown operator!";
}

static void ast_traverse_print_unary_operator(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(unary_operator);
  printf("unary %s\n", operator_type_str(self->type));
  ast_traverse_print(self->expr, indent + 1);
}

static void ast_traverse_print_binary_operator(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(binary_operator);
  printf("binary %s\n", operator_type_str(self->type));
  ast_traverse_print(self->expr1, indent + 1);
  ast_traverse_print(self->expr2, indent + 1);
}

static void ast_traverse_print_identifier(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(identifier);
  printf("identifier [%s]\n", self->name);
}

static void ast_traverse_print_constant(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(constant);
  printf("constant [%d]\n", self->value);
}

static void ast_traverse_print_if(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(if);
  printf("if (\n");
  ast_traverse_print(self->condition, indent + 1);

  TRAVERSE_PRINT_INDENT
  printf(") {\n");
  ast_traverse_print(self->if_true, indent + 1);

  if (self->if_false) {
    TRAVERSE_PRINT_INDENT
    printf("} else {\n");
    ast_traverse_print(self->if_false, indent + 1);
  }

  TRAVERSE_PRINT_INDENT
  printf("}\n");
}

static void ast_traverse_print_loop(struct ast *node, int indent) {
  TRAVERSE_PRINT_HEADER(loop);
  printf("while (\n");
  ast_traverse_print(self->condition, indent + 1);

  TRAVERSE_PRINT_INDENT;
  printf(") {\n");
  ast_traverse_print(self->while_true, indent + 1);

  TRAVERSE_PRINT_INDENT;
  printf("}\n");
}

static void ast_traverse_translate_root(struct ast *node,
                                 struct translate_context *context) {
  AST_CAST_SELF(root);
}

static void ast_traverse_translate_variable_list(struct ast *node,
                                          struct translate_context *context) {
  AST_CAST_SELF(variable_list);
}

static void ast_traverse_translate_operator_list(struct ast *node,
                                          struct translate_context *context) {
  AST_CAST_SELF(operator_list);
}

static void ast_traverse_translate_assign(struct ast *node,
                                   struct translate_context *context) {
  AST_CAST_SELF(assign);
}

static void ast_traverse_translate_unary_operator(struct ast *node,
                                           struct translate_context *context) {
  AST_CAST_SELF(unary_operator);
}

static void ast_traverse_translate_binary_operator(struct ast *node,
                                            struct translate_context *context) {
  AST_CAST_SELF(binary_operator);
}

static void ast_traverse_translate_identifier(struct ast *node,
                                       struct translate_context *context) {
  AST_CAST_SELF(identifier);
}

static void ast_traverse_translate_constant(struct ast *node,
                                     struct translate_context *context) {
  AST_CAST_SELF(constant);
}

static void ast_traverse_translate_if(struct ast *node,
                               struct translate_context *context) {
  AST_CAST_SELF(if);
}

static void ast_traverse_translate_loop(struct ast *node,
                                 struct translate_context *context) {
  AST_CAST_SELF(loop);
}

static void ast_free_root(struct ast *node) {
  AST_CAST_SELF(root);
  ast_free(self->variable_declaration);
  ast_free(self->computation_description);
  free(self);
}

static void ast_free_variable_list(struct ast *node) {
  AST_CAST_SELF(variable_list);
  free(self->name);
  ast_free(self->next);
  free(self);
}

static void ast_free_operator_list(struct ast *node) {
  AST_CAST_SELF(operator_list);
  ast_free(self->op);
  ast_free(self->next);
  free(self);
}

static void ast_free_assign(struct ast *node) {
  AST_CAST_SELF(assign);
  free(self->name);
  ast_free(self->expr);
  free(self);
}

static void ast_free_unary_operator(struct ast *node) {
  AST_CAST_SELF(unary_operator);
  ast_free(self->expr);
  free(self);
}

static void ast_free_binary_operator(struct ast *node) {
  AST_CAST_SELF(binary_operator);
  ast_free(self->expr1);
  ast_free(self->expr2);
  free(self);
}

static void ast_free_identifier(struct ast *node) {
  AST_CAST_SELF(identifier);
  free(self->name);
  free(self);
}

static void ast_free_constant(struct ast *node) {
  AST_CAST_SELF(constant);
  free(self);
}

static void ast_free_if(struct ast *node) {
  AST_CAST_SELF(if);
  ast_free(self->condition);
  ast_free(self->if_true);
  ast_free(self->if_false);
  free(self);
}

static void ast_free_loop(struct ast *node) {
  AST_CAST_SELF(loop);
  ast_free(self->condition);
  ast_free(self->while_true);
  free(self);
}

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
AST_DEFINE_TYPE_2(variable_list, AST_VARIABLE_LIST, char *, name, struct ast *,
                  next);
AST_DEFINE_TYPE_2(operator_list, AST_OPERATOR_LIST, struct ast *, op,
                  struct ast *, next);

AST_DEFINE_TYPE_2(assign, AST_ASSIGN, char *, name, struct ast *, expr);
AST_DEFINE_TYPE_2(unary_operator, AST_UNARY_OPERATOR, struct ast *, expr, int,
                  type);
AST_DEFINE_TYPE_3(binary_operator, AST_BINARY_OPERATOR, struct ast *, expr1,
                  struct ast *, expr2, int, type);
AST_DEFINE_TYPE_1(identifier, AST_IDENTIFIER, char *, name);
AST_DEFINE_TYPE_1(constant, AST_CONSTANT, int, value);
AST_DEFINE_TYPE_3(if, AST_IF, struct ast *, condition, struct ast *, if_true,
                  struct ast *, if_false);
AST_DEFINE_TYPE_2(loop, AST_LOOP, struct ast *, condition, struct ast *,
                  while_true);

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

struct ast *result;

int main() {
  int retcode = yyparse();
  if (retcode)
    return retcode;
  ast_traverse_print(result, 0);
  ast_free(result);
  return 0;
}
