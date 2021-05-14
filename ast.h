#ifndef _AST_H_
#define _AST_H_

struct ast;
extern struct ast *result;

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

struct translate_context {
  int tmpvar_counter;
  int label_counter;
};

struct ast_metatable {
  void (*free_node)(struct ast *);
  void (*traverse_print)(struct ast *, int);
  void (*traverse_translate)(struct ast *, struct translate_context *);
  enum ast_type type;
};

struct ast {
  struct ast_metatable *metatable;
};

static inline void ast_free(struct ast *node) {
  if (node)
    node->metatable->free_node(node);
}

static inline void ast_traverse_print(struct ast *node, int indent) {
  node->metatable->traverse_print(node, indent);
}

static inline void ast_traverse_translate(struct ast *node,
                                          struct translate_context *context) {
  node->metatable->traverse_translate(node, context);
}

#define AST_DECLARE_TYPE_1(type, arg1)                                         \
  struct ast_##type {                                                          \
    struct ast base;                                                           \
    arg1;                                                                      \
  };                                                                           \
  struct ast *ast_new_##type(arg1);

#define AST_DECLARE_TYPE_2(type, arg1, arg2)                                   \
  struct ast_##type {                                                          \
    struct ast base;                                                           \
    arg1;                                                                      \
    arg2;                                                                      \
  };                                                                           \
  struct ast *ast_new_##type(arg1, arg2);

#define AST_DECLARE_TYPE_3(type, arg1, arg2, arg3)                             \
  struct ast_##type {                                                          \
    struct ast base;                                                           \
    arg1;                                                                      \
    arg2;                                                                      \
    arg3;                                                                      \
  };                                                                           \
  struct ast *ast_new_##type(arg1, arg2, arg3);

AST_DECLARE_TYPE_2(root, struct ast *variable_declaration,
                   struct ast *computation_description);
AST_DECLARE_TYPE_2(variable_list, char *name, struct ast *next);
AST_DECLARE_TYPE_2(operator_list, struct ast *op, struct ast *next);

AST_DECLARE_TYPE_2(assign, char *name, struct ast *expr);
AST_DECLARE_TYPE_2(unary_operator, struct ast *expr, int type);
AST_DECLARE_TYPE_3(binary_operator, struct ast *expr1, struct ast *expr2,
                   int type);
AST_DECLARE_TYPE_1(identifier, char *name);
AST_DECLARE_TYPE_1(constant, int value);
AST_DECLARE_TYPE_3(if, struct ast *condition, struct ast *if_true,
                   struct ast *if_false);
AST_DECLARE_TYPE_2(loop, struct ast *condition, struct ast *while_true);

void yyerror(char *s);

#endif
