#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "globals.h"
#include "lexer.h"

typedef struct Parser {
    Lexer lexer;
    Token curr_token;
    Token peek_token;
    ASTNodeArrayList *backing_node_list;
} Parser;

extern Parser *make_parser(char *input);
extern void cleanup_parser(Parser *parser);
extern void parse_next_token(Parser *parser);
extern Program *parse_program(Parser *parser);
extern ASTNode *parse_statement(Parser *parser);
extern ASTNode *parse_let_statement(Parser *parser);
extern bool compare_curr_token_type(Parser *parser, TokenType tok_type);
extern bool compare_peek_token_type(Parser *parser, TokenType tok_type);
extern bool expect_peek(Parser *parser, TokenType tok_type);

#endif // PARSER_H
