#include "parser.h"
#include "arrlist_utils.h"
#include "ast.h"
#include "globals.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ERROR_CAPACITY 25

static Token EMPTY_TOKEN = { TOKEN_ILLEGAL, "\0" };

static ParserLookupEntry parser_fns[4] = {
    { .type = TOKEN_IDENT, .prefix_fn = parse_identifier, .infix_fn = NULL },
    { .type = TOKEN_INT, .prefix_fn = parse_integer_literal, .infix_fn = NULL },
    { .type = TOKEN_BANG, .prefix_fn = parse_prefix_expression, .infix_fn = NULL },
    { .type = TOKEN_MINUS, .prefix_fn = parse_prefix_expression, .infix_fn = NULL }
};

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
    case TOKEN_RETURN:
        return parse_return_statement(parser);
        break;
    default:
        return parse_expression_statement(parser);
    }
}

ASTNode *parse_let_statement(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_LET_STMT;
    strcpy(&node->token_literal, "let");

    if (!expect_peek(parser, TOKEN_IDENT)) {
        return NULL;
    }

    // Parse identifier
    ASTNode *identifier_node = make_ast_node(parser->backing_node_list);
    identifier_node->type = NODE_IDENTIFIER;

    strcpy(identifier_node->data.identifier, parser->curr_token.literal);
    strcpy(identifier_node->token_literal, parser->curr_token.literal);

    node->data.let_stmt.left = identifier_node;

    if (!expect_peek(parser, TOKEN_ASSIGN)) {
        return NULL;
    }

    while (!compare_curr_token_type(parser, TOKEN_SEMICOLON)) {
        parse_next_token(parser);
    }

    return node;
}

ASTNode *parse_return_statement(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_RETURN_STMT;
    strcpy(&node->token_literal, "return");

    parse_next_token(parser);

    // TODO: Parse expressions

    while (!compare_curr_token_type(parser, TOKEN_SEMICOLON)) {
        parse_next_token(parser);
    }

    return node;
}

ASTNode *parse_expression_statement(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_EXPR_STMT;

    node->data.expr_stmt = parse_expression(parser, LOWEST);

    if (compare_peek_token_type(parser, TOKEN_SEMICOLON)) {
        parse_next_token(parser);
    }

    return node;
}

ASTNode *parse_expression(Parser *parser, Precedence precedence)
{
    ParserFn prefix_fn = get_prefix_fn(parser->curr_token.type);
    if (prefix_fn == NULL) {
        report_no_prefix_error(parser, parser->curr_token.type);
        return NULL;
    }
    ASTNode *left_expr = prefix_fn(parser);
    return left_expr;
}

ASTNode *parse_identifier(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_IDENTIFIER;
    strcpy(&node->token_literal, parser->curr_token.literal);
    strcpy(&node->data.identifier, parser->curr_token.literal);
    return node;
}

ASTNode *parse_integer_literal(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_LITERAL;
    strcpy(&node->token_literal, parser->curr_token.literal);
    node->data.literal.type = LITERAL_INT;
    node->data.literal.value.int_value = atoi(parser->curr_token.literal);
    return node;
}

ASTNode *parse_prefix_expression(Parser *parser)
{
    ASTNode *node = make_ast_node(parser->backing_node_list);
    node->type = NODE_PREFIX_EXPR;
    strcpy(&node->token_literal, parser->curr_token.literal);
    memcpy(&node->data.prefix_expr.token, &parser->curr_token, sizeof(Token));

    parse_next_token(parser);

    node->data.prefix_expr.right = parse_expression(parser, PREFIX);

    return node;
}

ParserFn get_prefix_fn(TokenType type)
{
    size_t arr_len = sizeof(parser_fns) / sizeof(ParserLookupEntry);
    for (size_t i = 0; i < arr_len; i++) {
        if (parser_fns[i].type == type) {
            return parser_fns[i].prefix_fn;
        }
    }
    return NULL;
}

ParserFn get_infix_fn(TokenType type)
{
    size_t arr_len = sizeof(parser_fns) / sizeof(ParserLookupEntry);
    for (size_t i = 0; i < arr_len; i++) {
        if (parser_fns[i].type == type) {
            return parser_fns[i].infix_fn;
        }
    }
    return NULL;
}

ErrorArrayList *make_error_arraylist(void)
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
        list->array = (char **)realloc_backing_array(list->array, list->size, list->capacity, sizeof(char *));
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
        report_peek_error(parser, tok_type);
        return FALSE;
    }
}

inline void report_peek_error(Parser *parser, TokenType tok_type)
{
    size_t total_len = snprintf(NULL, 0, "Expected next token to be %s, got %s instead", token_type_to_str(tok_type), token_type_to_str(parser->peek_token.type));
    char *error = malloc(total_len + 1); // We add one for sentinel character '\0'
    sprintf(error, "Expected next token to be %s, got %s instead", token_type_to_str(tok_type), token_type_to_str(parser->peek_token.type));
    add_error_to_arraylist(parser->errors, error);
}

inline void report_no_prefix_error(Parser *parser, TokenType tok_type)
{
    size_t total_len = snprintf(NULL, 0, "No prefix parse function for %s found", token_type_to_str(tok_type));
    char *error = malloc(total_len + 1); // We add one for sentinel character '\0'
    sprintf(error, "No prefix parse function for %s found", token_type_to_str(tok_type));
    add_error_to_arraylist(parser->errors, error);
}
