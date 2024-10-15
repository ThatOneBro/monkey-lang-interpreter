#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "globals.h"
#include "lexer.h"

typedef enum Precedence {
    _, // IOTA
    PREC_LOWEST,
    PREC_EQUALS, // ==
    PREC_LESSGREATER, // > or <
    PREC_SUM, // +
    PREC_PRODUCT, // *
    PREC_PREFIX, // -X or !X
    PREC_CALL, // myFunction(X)
} Precedence;

typedef struct ErrorArrayList {
    char **array;
    size_t size;
    size_t capacity;
} ErrorArrayList;

typedef struct Parser {
    Lexer lexer;
    Token curr_token;
    Token peek_token;
    ASTNodeArrayList *backing_node_list;
    ErrorArrayList *errors;
} Parser;

typedef ASTNode *(*PrefixFn)(Parser *parser);
typedef ASTNode *(*InfixFn)(Parser *parser, ASTNode *left);

typedef struct ParserLookupEntry {
    TokenType type;
    PrefixFn prefix_fn;
    InfixFn infix_fn;
} ParserLookupEntry;

typedef struct PrecedenceEntry {
    TokenType type;
    Precedence precedence;
} PrecedenceEntry;

extern Parser *make_parser(char *input);
extern void cleanup_parser(Parser *parser);
extern void parse_next_token(Parser *parser);

extern Program *parse_program(Parser *parser);
extern ASTNode *parse_statement(Parser *parser);
extern ASTNode *parse_let_statement(Parser *parser);
extern ASTNode *parse_return_statement(Parser *parser);
extern ASTNode *parse_expression_statement(Parser *parser);

extern ASTNode *parse_expression(Parser *parser, Precedence precedence);
extern ASTNode *parse_identifier(Parser *parser);
extern ASTNode *parse_integer_literal(Parser *parser);
extern ASTNode *parse_prefix_expression(Parser *parser);
extern ASTNode *parse_infix_expression(Parser *parser, ASTNode *left);

extern PrefixFn get_prefix_fn(TokenType type);
extern InfixFn get_infix_fn(TokenType type);
extern Precedence get_current_precedence(Parser *parser);
extern Precedence get_peek_precedence(Parser *parser);

extern ErrorArrayList *make_error_arraylist(void);
extern void add_error_to_arraylist(ErrorArrayList *list, char *error);
extern char *get_error_from_arraylist(ErrorArrayList *list, size_t index);
extern void cleanup_error_arraylist(ErrorArrayList *list);

extern inline bool compare_curr_token_type(Parser *parser, TokenType tok_type);
extern inline bool compare_peek_token_type(Parser *parser, TokenType tok_type);
extern inline bool expect_peek(Parser *parser, TokenType tok_type);
extern inline void report_peek_error(Parser *parser, TokenType tok_type);
extern inline void report_no_prefix_error(Parser *parser, TokenType tok_type);

#endif // PARSER_H
