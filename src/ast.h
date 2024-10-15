#ifndef AST_H
#define AST_H

#include "globals.h"
#include "token.h"
#include <stddef.h>

#define MAX_OPERATOR_SIZE 4

typedef enum ASTNodeType {
    NODE_LET_STMT,
    NODE_RETURN_STMT,
    NODE_EXPR_STMT,
    NODE_PREFIX_EXPR,
    NODE_LITERAL,
    NODE_IDENTIFIER
} ASTNodeType;

typedef enum OperatorType {
    OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_ASSIGN,
    OP_NEGATE
} OperatorType;

typedef union Value {
    int int_value;
    float float_value;
    char *string_value;
    bool boolean_value;
} Value;

typedef enum LiteralType {
    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,
} LiteralType;

typedef struct Literal {
    Value value;
    LiteralType type;
} Literal;

typedef struct PrefixExpr {
    Token token;
    struct ASTNode *right;
    char operator[MAX_OPERATOR_SIZE];
} PrefixExpr;

typedef struct LetStmt {
    struct ASTNode *left;
    struct ASTNode *right;
} LetStmt;

typedef struct ASTNode {
    union {
        LetStmt let_stmt;
        struct ASTNode *expr_stmt;
        struct ASTNode *return_stmt;
        PrefixExpr prefix_expr;
        Literal literal;
        char identifier[MAX_IDENTIFIER_SIZE];
    } data;
    ASTNodeType type;
    char token_literal[MAX_TOKEN_LITERAL_SIZE];
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

extern ASTNodeArrayList *make_ast_node_array_list(void);
extern void add_ast_node_to_list(ASTNodeArrayList *list, ASTNode *node);
extern ASTNode *get_ast_node_from_list(ASTNodeArrayList *list, size_t index);
extern void set_ast_node_in_list(ASTNodeArrayList *list, size_t index, ASTNode *node);
extern ASTNode *alloc_node_in_list(ASTNodeArrayList *list);
extern void cleanup_ast_node_list(ASTNodeArrayList *list);

extern Program *make_program(void);
extern void cleanup_program(Program *program);
extern void add_ast_node_to_program(Program *program, ASTNode *node);
extern ASTNode *get_nth_statement(Program *program, size_t n);

extern char *node_to_str(ASTNode *node);
extern char *program_to_str(Program *program);

#endif // AST_H
