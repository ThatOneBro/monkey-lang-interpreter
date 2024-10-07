#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct Parser {
    Lexer lexer;
    Token curr_token;
    Token peek_token;
} Parser;

extern Parser *make_parser(char *input);
extern void cleanup_parser(Parser *parser);
extern void parse_next_token(Parser *parser);

#endif // PARSER_H
