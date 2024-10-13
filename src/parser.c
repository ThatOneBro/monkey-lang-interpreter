#include "parser.h"
#include "ast.h"
#include "globals.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>

Parser *make_parser(char *input)
{
    Parser *parser = malloc(sizeof(Parser));
    init_lexer(&parser->lexer, input);
    parser->backing_node_list = make_ast_node_array_list();
    parse_next_token(parser);
    parse_next_token(parser);
}

void cleanup_parser(Parser *parser)
{
    cleanup_ast_node_list(parser->backing_node_list);
    free(parser);
}

void parse_next_token(Parser *parser)
{
    parser->curr_token = parser->peek_token;
    parser->peek_token = lex_next_token(&parser->lexer);
}

Program *parse_program(Parser *parser)
{
    Program *program = make_program();
    while (parser->curr_token.type != TOKEN_EOF) {
        ASTNode *node = parse_statement(parser);
        if (node != NULL) {
            add_ast_node_to_program(program, node);
        }
        parse_next_token(parser);
    }
}

ASTNode *parse_statement(Parser *parser)
{
    switch (parser->curr_token.type) {
    case TOKEN_LET:
        return parse_let_statement(parser);
        break;
    default:
        return NULL;
    }
}

ASTNode *parse_let_statement(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_LET_STATEMENT;

    if (!expect_peek(parser, TOKEN_IDENT)) {
        return NULL;
    }

    // Parse identifier
    ASTNode *identifier_node = make_ast_node(parser->backing_node_list);
    identifier_node->type = NODE_IDENTIFIER;
    strcpy(identifier_node->data.identifier, parser->curr_token.literal);
    node->data.assignment.identifier = identifier_node;

    if (!expect_peek(parser, TOKEN_ASSIGN)) {
        return NULL;
    }

    while (!compare_curr_token_type(parser, TOKEN_SEMICOLON)) {
        parse_next_token(parser);
    }

    return node;
}

bool compare_curr_token_type(Parser *parser, TokenType tok_type)
{
    return parser->curr_token.type == tok_type;
}

bool compare_peek_token_type(Parser *parser, TokenType tok_type)
{
    return parser->peek_token.type == tok_type;
}

bool expect_peek(Parser *parser, TokenType tok_type)
{
    if (compare_peek_token_type(parser, tok_type)) {
        parse_next_token(parser);
        return TRUE;
    }

    return FALSE;
}
