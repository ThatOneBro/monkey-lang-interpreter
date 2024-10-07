#ifndef AST_H
#define AST_H

typedef enum ASTNodeType {
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_LET_STATEMENT,
} ASTNodeType;

typedef struct LLNode {
    struct LLNode *prev_node;
    struct LLNode *next_node;
} LLNode;

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
    LLNode list_node;
} ASTNode;

extern ASTNode *make_ast_node();
void cleanup_ast_node(ASTNode *node);

#endif // AST_H
