#ifndef _AST_H_
#define _AST_H_

struct ast;

enum ast_type {
  ASTT_ROOT,
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
} ast_t;

struct ast *ast_new(enum ast_type type, union ast_data data);
struct ast *ast_new_root(struct ast *vardecl, struct ast *compdesc);
struct ast *ast_new_compdesc(struct ast *oplist);
void ast_free(struct ast *ast);

#endif
