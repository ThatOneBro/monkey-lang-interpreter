#include "lexer.h"
#include <assert.h>
#include <ctype.h>
#include <memory.h>
#include <stdio.h>

#define MAX_LEXERS 20
#define MAX_TOKENS 200
#define MAX_IDENTIFIER_SIZE 40
#define MAX_INT_SIZE 20

static const char *keywords[] = { "fn", "let" };
static const TokenType keyword_token_map[] = { TOKEN_FUNCTION, TOKEN_LET };

static Lexer lexers[MAX_LEXERS];

const char *token_type_to_str(TokenType t)
{
    assert(t >= 0 && t <= TOKEN_LET);
    return TOKEN_TYPE_STR[t];
}

LexerHandle make_lexer(char *input, size_t input_len)
{
    for (size_t handle = 0; handle < MAX_LEXERS; handle++) {
        if (lexers[handle].input != NULL) {
            continue;
        }
        // We found an empty slot
        lexers[handle].input = input;
        lexers[handle].input_len = input_len;
        lexers[handle].position = 0;
        lexers[handle].read_position = 0;
        lexers[handle].curr_char = '\0';
        read_char(&lexers[handle]);
        return handle;
    }
    // We searched the whole list, return null
    return -1;
}

Lexer *get_lexer(LexerHandle handle)
{
    assert(handle >= 0 && handle < MAX_LEXERS);
    assert(lexers[handle].input != NULL);
    return &lexers[handle];
}

void cleanup_lexer(LexerHandle handle)
{
    assert(lexers[handle].input != NULL);
    memset(&lexers[handle], 0, sizeof(Lexer));
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

void read_identifier(Lexer *lexer, char *out)
{
    size_t pos = lexer->position;
    while (isalpha(lexer->curr_char)) {
        read_char(lexer);
    }
    assert(lexer->position - pos <= MAX_IDENTIFIER_SIZE);
    memcpy(out, &lexer->input[pos], lexer->position - pos);
}

void read_number(Lexer *lexer, char *out)
{
    size_t pos = lexer->position;
    while (isdigit(lexer->curr_char)) {
        read_char(lexer);
    }
    assert(lexer->position - pos <= MAX_INT_SIZE);
    memcpy(out, &lexer->input[pos], lexer->position - pos);
}

void skip_whitespace(Lexer *lexer)
{
    while (lexer->curr_char == ' ' || lexer->curr_char == '\t' || lexer->curr_char == '\n' || lexer->curr_char == '\r') {
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

Token next_token(LexerHandle handle)
{
    Lexer *lexer = get_lexer(handle);
    skip_whitespace(lexer);

    // Get the next token from the current char
    Token tok;
    switch (lexer->curr_char) {
    case '=':
        tok.type = TOKEN_ASSIGN;
        // TODO: Check perf difference between `strcpy` and `memcpy`
        strcpy(tok.literal, "=");
        break;
    case '+':
        tok.type = TOKEN_PLUS;
        strcpy(tok.literal, "+");
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
    case ',':
        tok.type = TOKEN_COMMA;
        strcpy(tok.literal, ",");
        break;
    case ';':
        tok.type = TOKEN_SEMICOLON;
        strcpy(tok.literal, ";");
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
