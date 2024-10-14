#ifndef AST_H
#define AST_H

#include "token.h"
#include <stddef.h>

typedef enum ASTNodeType {
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_LET_STATEMENT,
    NODE_RETURN_STATEMENT,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char token_literal[MAX_TOKEN_LITERAL_SIZE];
    union {
        // struct {
        //     struct ASTNode *left;
        //     struct ASTNode *right;
        //     char op;
        // } binary_op;
        double number;
        char identifier[MAX_IDENTIFIER_SIZE];
        struct ASTNode *return_value;
        struct {
            struct ASTNode *identifier;
            struct ASTNode *value;
        } assignment;
        // struct {
        //     char *identifier;
        //     struct ASTNode **arguments;
        //     int arg_count;
        // } function_call;
    } data;
} ASTNode;

typedef struct ASTNodeArrayList {
    ASTNode *array;
    size_t size;
    size_t capacity;
} ASTNodeArrayList;

typedef struct Program {
    ASTNode **array;
    size_t size;
    size_t capacity;
} Program;

extern ASTNode *make_ast_node(ASTNodeArrayList *list);
extern void cleanup_ast_node(ASTNode *node);

extern ASTNodeArrayList *make_ast_node_array_list();
extern void add_ast_node_to_list(ASTNodeArrayList *list, ASTNode *node);
extern ASTNode *get_ast_node_from_list(ASTNodeArrayList *list, size_t index);
extern void set_ast_node_in_list(ASTNodeArrayList *list, size_t index, ASTNode *node);
extern ASTNode *alloc_node_in_list(ASTNodeArrayList *list);
extern void cleanup_ast_node_list(ASTNodeArrayList *list);

extern Program *make_program();
extern void cleanup_program(Program *program);
extern void add_ast_node_to_program(Program *program, ASTNode *node);
extern ASTNode *get_nth_statement(Program *program, size_t n);

#endif // AST_H
