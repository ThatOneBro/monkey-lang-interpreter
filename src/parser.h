#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "globals.h"
#include "lexer.h"

typedef enum Precedence {
    _, // IOTA
    LOWEST,
    EQUALS, // ==
    LESSGREATER, // > or <
    SUM, // +
    PRODUCT, // *
    PREFIX, // -X or !X
    CALL, // myFunction(X)
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

typedef ASTNode *(*ParserFn)(Parser *parser);

typedef struct ParserLookupEntry {
    TokenType type;
    void *prefix_fn;
    void *infix_fn;
} ParserLookupEntry;

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

extern ParserFn get_prefix_fn(TokenType type);
extern ParserFn get_infix_fn(TokenType type);

extern ErrorArrayList *make_error_arraylist(void);
extern void add_error_to_arraylist(ErrorArrayList *list, char *error);
extern char *get_error_from_arraylist(ErrorArrayList *list, size_t index);
extern void cleanup_error_arraylist(ErrorArrayList *list);

extern inline bool compare_curr_token_type(Parser *parser, TokenType tok_type);
extern inline bool compare_peek_token_type(Parser *parser, TokenType tok_type);
extern inline bool expect_peek(Parser *parser, TokenType tok_type);
extern inline void *report_peek_error(Parser *parser, TokenType tok_type);

#endif // PARSER_H
