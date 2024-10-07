#include "parser.h"
#include "lexer.h"
#include <stdlib.h>

Parser *make_parser(char *input)
{
    Parser *parser = malloc(sizeof(Parser));
    init_lexer(&parser->lexer, input);
    parse_next_token(parser);
    parse_next_token(parser);
}

void cleanup_parser(Parser *parser)
{
    free(parser);
}

void parse_next_token(Parser *parser)
{
    parser->curr_token = parser->peek_token;
    parser->peek_token = lex_next_token(&parser->lexer);
}
