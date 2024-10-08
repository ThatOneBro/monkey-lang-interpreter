#include "lexer.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *keywords[] = { "fn", "let", "true", "false", "if", "else", "return" };
static const TokenType keyword_token_map[] = { TOKEN_FUNCTION, TOKEN_LET, TOKEN_TRUE, TOKEN_FALSE, TOKEN_IF, TOKEN_ELSE, TOKEN_RETURN };

static const char *TOKEN_TYPE_STR[] = {
    [TOKEN_ILLEGAL] = "ILLEGAL",
    [TOKEN_EOF] = "EOF",

    [TOKEN_IDENT] = "IDENT",
    [TOKEN_INT] = "INT",

    // Operators
    [TOKEN_ASSIGN] = "=",
    [TOKEN_PLUS] = "+",
    [TOKEN_MINUS] = "-",
    [TOKEN_BANG] = "!",
    [TOKEN_ASTERISK] = "*",
    [TOKEN_SLASH] = "/",

    [TOKEN_EQ] = "==",
    [TOKEN_NOT_EQ] = "!=",
    [TOKEN_LT] = "<",
    [TOKEN_GT] = ">",

    // Delimiters
    [TOKEN_COMMA] = ",",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_LPAREN] = "(",
    [TOKEN_RPAREN] = ")",
    [TOKEN_LBRACE] = "{",
    [TOKEN_RBRACE] = "}",

    // Keywords
    [TOKEN_FUNCTION] = "FUNCTION",
    [TOKEN_LET] = "LET",
    [TOKEN_TRUE] = "TRUE",
    [TOKEN_FALSE] = "FALSE",
    [TOKEN_IF] = "IF",
    [TOKEN_ELSE] = "ELSE",
    [TOKEN_RETURN] = "RETURN",
};

const char *token_type_to_str(TokenType t)
{
    assert(t >= 0 && t <= TOKEN_RETURN);
    return TOKEN_TYPE_STR[t];
}

void init_lexer(Lexer *lexer, char *input)
{
    lexer->input = input;
    lexer->input_len = strlen(input) + 1;
    lexer->position = 0;
    lexer->read_position = 0;
    lexer->curr_char = '\0';
    read_char(lexer);
}

Lexer *make_lexer(char *input)
{
    Lexer *lexer = malloc(sizeof(struct Lexer));
    init_lexer(lexer, input);
    return lexer;
}

void cleanup_lexer(Lexer *lexer)
{
    free(lexer);
}

int read_char(Lexer *lexer)
{
    int ret = 0;

    if (lexer->read_position >= lexer->input_len) {
        lexer->curr_char = '\0';
        ret = 1;
    } else {
        lexer->curr_char = lexer->input[lexer->read_position];
    }

    lexer->position = lexer->read_position;
    lexer->read_position++;

    return ret;
}

char peek_char(Lexer *lexer)
{
    if (lexer->read_position >= lexer->input_len) {
        return '\0';
    } else {
        return lexer->input[lexer->read_position];
    }
}

void read_identifier(Lexer *lexer, char *out)
{
    size_t pos = lexer->position;
    while (isalpha(lexer->curr_char)) {
        read_char(lexer);
    }
    assert(lexer->position - pos <= MAX_IDENTIFIER_SIZE);
    memcpy(out, &lexer->input[pos], lexer->position - pos);
    out[lexer->position - pos] = '\0';
}

void read_number(Lexer *lexer, char *out)
{
    size_t pos = lexer->position;
    while (isdigit(lexer->curr_char)) {
        read_char(lexer);
    }
    assert(lexer->position - pos <= MAX_INT_SIZE);
    memcpy(out, &lexer->input[pos], lexer->position - pos);
    out[lexer->position - pos] = '\0';
}

void skip_whitespace(Lexer *lexer)
{
    while (isspace(lexer->curr_char)) {
        read_char(lexer);
    }
}

TokenType lookup_keyword(char *literal)
{
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(literal, keywords[i]) == 0) {
            // Strings are equal
            return keyword_token_map[i];
        }
    }
    return TOKEN_IDENT;
}

Token lex_next_token(Lexer *lexer)
{
    skip_whitespace(lexer);

    // Get the next token from the current char
    Token tok;
    switch (lexer->curr_char) {
    case '=':
        if (peek_char(lexer) == '=') {
            tok.type = TOKEN_EQ;
            strcpy(tok.literal, "==");
            read_char(lexer);
        } else {
            tok.type = TOKEN_ASSIGN;
            // TODO: Check perf difference between `strcpy` and `memcpy`
            strcpy(tok.literal, "=");
        }
        break;
    case '+':
        tok.type = TOKEN_PLUS;
        strcpy(tok.literal, "+");
        break;
    case '-':
        tok.type = TOKEN_MINUS;
        strcpy(tok.literal, "-");
        break;
    case '*':
        tok.type = TOKEN_ASTERISK;
        strcpy(tok.literal, "*");
        break;
    case '/':
        tok.type = TOKEN_SLASH;
        strcpy(tok.literal, "/");
        break;
    case '!':
        if (peek_char(lexer) == '=') {
            tok.type = TOKEN_NOT_EQ;
            strcpy(tok.literal, "!=");
            read_char(lexer);
        } else {
            tok.type = TOKEN_BANG;
            strcpy(tok.literal, "!");
        }
        break;
    case '>':
        tok.type = TOKEN_GT;
        strcpy(tok.literal, ">");
        break;
    case '<':
        tok.type = TOKEN_LT;
        strcpy(tok.literal, "<");
        break;
    case '(':
        tok.type = TOKEN_LPAREN;
        strcpy(tok.literal, "(");
        break;
    case ')':
        tok.type = TOKEN_RPAREN;
        strcpy(tok.literal, ")");
        break;
    case '{':
        tok.type = TOKEN_LBRACE;
        strcpy(tok.literal, "{");
        break;
    case '}':
        tok.type = TOKEN_RBRACE;
        strcpy(tok.literal, "}");
        break;
    case ';':
        tok.type = TOKEN_SEMICOLON;
        strcpy(tok.literal, ";");
        break;
    case ',':
        tok.type = TOKEN_COMMA;
        strcpy(tok.literal, ",");
        break;
    case '\0':
        tok.type = TOKEN_EOF;
        strcpy(tok.literal, "");
        break;
    default:
        if (isalpha(lexer->curr_char)) {
            read_identifier(lexer, tok.literal);
            tok.type = lookup_keyword(tok.literal);
            return tok;
        } else if (isdigit(lexer->curr_char)) {
            read_number(lexer, tok.literal);
            tok.type = TOKEN_INT;
            return tok;
        }
        tok.type = TOKEN_ILLEGAL;
        strcpy(tok.literal, &lexer->curr_char);
    }
    read_char(lexer);
    return tok;
}
