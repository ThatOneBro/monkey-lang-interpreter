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
    NODE_INFIX_EXPR,
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

typedef union LiteralValue {
    int int_value;
    float float_value;
    char *string_value;
    char identifier[MAX_IDENTIFIER_SIZE];
    bool boolean_value;
} LiteralValue;

typedef enum LiteralType {
    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,
    LITERAL_IDENTIFIER,
    LITERAL_BOOL,
} LiteralType;

typedef struct Literal {
    LiteralValue value;
    LiteralType type;
} Literal;

typedef struct PrefixOpExpr {
    Token token;
    struct ASTNode *right;
    char operator[MAX_OPERATOR_SIZE];
} PrefixOpExpr;

typedef struct InfixOpExpr {
    Token token;
    struct ASTNode *left;
    struct ASTNode *right;
    char operator[MAX_OPERATOR_SIZE];
} InfixOpExpr;

typedef struct LetStmt {
    struct ASTNode *left;
    struct ASTNode *right;
} LetStmt;

typedef struct ASTNode {
    union {
        LetStmt let_stmt;
        struct ASTNode *expr_stmt;
        struct ASTNode *return_stmt;
        PrefixOpExpr prefix_expr;
        InfixOpExpr infix_expr;
        Literal literal;
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

#define ACCESS_INT(lit) ((lit).value.int_value)
#define ACCESS_FLOAT(lit) ((lit).value.float_value)
#define ACCESS_STRING(lit) ((lit).value.string_value)
#define ACCESS_IDENTIFIER(lit) ((lit).value.identifier)
#define ACCESS_BOOL(lit) ((lit).value.boolean_value)

#define COMPARE_INT(lit, exp) ((lit).value.int_value == (exp))
#define COMPARE_FLOAT(lit, exp) ((lit).value.float_value == (exp))
#define COMPARE_STRING(lit, exp) (strcmp((lit).value.string_value, (exp)) == 0)
#define COMPARE_IDENTIFIER(lit, exp) (strcmp((lit).value.identifier, (exp)) == 0)
#define COMPARE_BOOL(lit, exp) ((lit).value.boolean_value == (exp))

// Type-safe access macro
#define ACCESS_LITERAL_VALUE(lit, type)                                                         \
    ((type) == LITERAL_INT ? ACCESS_INT(lit) : (type) == LITERAL_FLOAT ? ACCESS_FLOAT(lit)      \
            : (type) == LITERAL_STRING                                 ? ACCESS_STRING(lit)     \
            : (type) == LITERAL_IDENTIFIER                             ? ACCESS_IDENTIFIER(lit) \
            : (type) == LITERAL_BOOL                                   ? ACCESS_BOOL(lit)       \
                                                                       : (void)0)

// Type-safe comparison macro
#define COMPARE_LITERAL_VALUE(lit, type, expected)                                                                                           \
    ((type) == LITERAL_INT ? COMPARE_INT(lit, (int)(expected)) : (type) == LITERAL_FLOAT ? COMPARE_FLOAT(lit, (float)(expected))             \
            : (type) == LITERAL_STRING                                                   ? COMPARE_STRING(lit, (const char *)(expected))     \
            : (type) == LITERAL_IDENTIFIER                                               ? COMPARE_IDENTIFIER(lit, (const char *)(expected)) \
            : (type) == LITERAL_BOOL                                                     ? COMPARE_BOOL(lit, (bool)(expected))               \
                                                                                         : FALSE)

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
extern const char *node_type_to_str(ASTNodeType t);

#endif // AST_H
