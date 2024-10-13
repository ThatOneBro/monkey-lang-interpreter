#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum ASTNodeType {
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_LET_STATEMENT,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        // struct {
        //     struct ASTNode *left;
        //     struct ASTNode *right;
        //     char op;
        // } binary_op;
        double number;
        char *identifier;
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

extern ASTNode *make_ast_node();
extern void cleanup_ast_node(ASTNode *node);

extern ASTNodeArrayList *make_ast_node_array_list();
extern void add_ast_node_to_list(ASTNodeArrayList *list, ASTNode *node);
extern ASTNode *get_ast_node_from_list(ASTNodeArrayList *list, size_t index);
extern void set_ast_node_in_list(ASTNodeArrayList *list, size_t index, ASTNode *node);
extern void free_ast_node_list(ASTNodeArrayList *list);

#endif // AST_H
