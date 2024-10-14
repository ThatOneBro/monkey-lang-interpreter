#include "token.h"
#include <assert.h>
#include <stdio.h>

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
