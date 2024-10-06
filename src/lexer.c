#include "lexer.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>

#define MAX_LEXERS 20
#define MAX_TOKENS 200

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
        read_char(handle);
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

int read_char(LexerHandle handle)
{
    int ret = 0;
    Lexer *lexer = get_lexer(handle);

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

Token next_token(LexerHandle handle)
{
    // Get the next token
    Token tok;
    switch (lexers->curr_char) {
    case '=':
        tok.type = TOKEN_ASSIGN;
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
        printf("Unexpected character encountered\n");
        assert(1 != 1);
    }
    read_char(handle);
    return tok;
}
