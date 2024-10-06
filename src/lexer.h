#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include <stddef.h>
#define MAX_TOKEN_LITERAL_SIZE 20

typedef enum TokenType {
    TOKEN_ILLEGAL,
    TOKEN_EOF,

    // Identifiers + literals
    TOKEN_IDENT,
    TOKEN_INT,

    // Operators
    TOKEN_ASSIGN,
    TOKEN_PLUS,

    // Delimiters
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    // Keywords
    TOKEN_FUNCTION,
    TOKEN_LET,
} TokenType;

static const char *TOKEN_TYPE_STR[] = {
    [TOKEN_ILLEGAL] = "ILLEGAL",
    [TOKEN_EOF] = "EOF",

    [TOKEN_IDENT] = "IDENT",
    [TOKEN_INT] = "INT",

    [TOKEN_ASSIGN] = "=",
    [TOKEN_PLUS] = "+",

    [TOKEN_COMMA] = ",",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_LPAREN] = "(",
    [TOKEN_RPAREN] = ")",
    [TOKEN_LBRACE] = "{",
    [TOKEN_RBRACE] = "}",

    [TOKEN_FUNCTION] = "FUNCTION",
    [TOKEN_LET] = "LET",
};

typedef struct Token {
    TokenType type;
    char literal[MAX_TOKEN_LITERAL_SIZE];
} Token;

static const char *token_type_to_str(TokenType t);

typedef struct Lexer {
    char *input;
    size_t input_len;
    size_t position;
    size_t read_position;
    char curr_char;
} Lexer;

typedef size_t LexerHandle;

extern LexerHandle make_lexer(char *input, size_t input_len);
extern Lexer *get_lexer(LexerHandle handle);
extern void cleanup_lexer(LexerHandle handle);
extern int read_char(LexerHandle handle);
extern Token next_token(LexerHandle handle);

#endif // LEXER_H
