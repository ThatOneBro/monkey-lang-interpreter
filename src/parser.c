#include "parser.h"
#include "ast.h"
#include "globals.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ERROR_CAPACITY 25

static Token EMPTY_TOKEN = { TOKEN_ILLEGAL, "\0" };

Parser *make_parser(char *input)
{
    Parser *parser = (Parser *)malloc(sizeof(struct Parser));
    if (parser == NULL) {
        return NULL;
    }
    init_lexer(&parser->lexer, input);
    parser->backing_node_list = make_ast_node_array_list();
    parser->errors = make_error_arraylist();
    memcpy(&parser->curr_token, &EMPTY_TOKEN, sizeof(Token));
    memcpy(&parser->peek_token, &EMPTY_TOKEN, sizeof(Token));
    parse_next_token(parser);
    parse_next_token(parser);
    return parser;
}

void cleanup_parser(Parser *parser)
{
    cleanup_ast_node_list(parser->backing_node_list);
    cleanup_error_arraylist(parser->errors);
    free(parser);
}

void parse_next_token(Parser *parser)
{
    memcpy(&parser->curr_token, &parser->peek_token, sizeof(Token));
    Token next_tok = lex_next_token(&parser->lexer);
    memcpy(&parser->peek_token, &next_tok, sizeof(Token));
}

Program *parse_program(Parser *parser)
{
    Program *program = make_program();
    if (program == NULL) {
        return NULL;
    }
    while (parser->curr_token.type != TOKEN_EOF) {
        ASTNode *node = parse_statement(parser);
        if (node != NULL) {
            add_ast_node_to_program(program, node);
        }
        parse_next_token(parser);
    }
    return program;
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
    strcpy(&node->token_literal, "let");

    if (!expect_peek(parser, TOKEN_IDENT)) {
        return NULL;
    }

    // Parse identifier
    ASTNode *identifier_node = make_ast_node(parser->backing_node_list);
    identifier_node->type = NODE_IDENTIFIER;

    strcpy(identifier_node->data.identifier, parser->curr_token.literal);
    strcpy(identifier_node->token_literal, parser->curr_token.literal);

    node->data.assignment.identifier = identifier_node;

    if (!expect_peek(parser, TOKEN_ASSIGN)) {
        return NULL;
    }

    while (!compare_curr_token_type(parser, TOKEN_SEMICOLON)) {
        parse_next_token(parser);
    }

    return node;
}

ErrorArrayList *make_error_arraylist()
{
    ErrorArrayList *list = malloc(sizeof(ErrorArrayList));
    list->array = calloc(INITIAL_ERROR_CAPACITY, sizeof(char *));
    list->size = 0;
    list->capacity = INITIAL_ERROR_CAPACITY;
    return list;
}

void add_error_to_arraylist(ErrorArrayList *list, char *error)
{
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = (char **)realloc(list->array, list->capacity * sizeof(char *));
        memset(&list->array[list->size], 0, list->capacity);
    }
    list->array[list->size++] = error;
}

char *get_error_from_arraylist(ErrorArrayList *list, size_t index)
{
    assert(index >= 0 && index < list->size);
    return list->array[index];
}

void cleanup_error_arraylist(ErrorArrayList *list)
{
    for (size_t i = 0; i < list->size; i++) {
        free(list->array[i]);
    }
    free(list->array);
    free(list);
}

inline bool compare_curr_token_type(Parser *parser, TokenType tok_type)
{
    return parser->curr_token.type == tok_type;
}

inline bool compare_peek_token_type(Parser *parser, TokenType tok_type)
{
    return parser->peek_token.type == tok_type;
}

inline bool expect_peek(Parser *parser, TokenType tok_type)
{
    if (compare_peek_token_type(parser, tok_type)) {
        parse_next_token(parser);
        return TRUE;
    } else {
        add_peek_error(parser, tok_type);
        return FALSE;
    }
}

inline void *report_peek_error(Parser *parser, TokenType tok_type)
{
    size_t needed = snprintf(NULL, 0, "Expected next token to be %s, got %s instead", token_type_to_str(tok_type), token_type_to_str(parser->peek_token.type)) + 1;
    char *error = malloc(needed);
    sprintf(error, "Expected next token to be %s, got %s instead", token_type_to_str(tok_type), token_type_to_str(parser->peek_token.type));
    add_error_to_arraylist(parser->errors, error);
}
