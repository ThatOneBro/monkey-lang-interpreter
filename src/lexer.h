#ifndef LEXER_H
#define LEXER_H

#include "globals.h"
#include <stddef.h>

#define MAX_IDENTIFIER_SIZE 40
#define MAX_INT_SIZE 20
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
    TOKEN_MINUS,
    TOKEN_BANG,
    TOKEN_ASTERISK,
    TOKEN_SLASH,

    TOKEN_EQ,
    TOKEN_NOT_EQ,
    TOKEN_LT,
    TOKEN_GT,

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
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
} TokenType;

typedef struct Token {
    TokenType type;
    char literal[MAX_TOKEN_LITERAL_SIZE];
} Token;

extern const char *token_type_to_str(TokenType t);

typedef struct Lexer {
    char *input;
    size_t input_len;
    size_t position;
    size_t read_position;
    char curr_char;
} Lexer;

extern Lexer *make_lexer(char *input);
extern void cleanup_lexer(Lexer *lexer);
extern int read_char(Lexer *lexer);
extern void read_identifier(Lexer *lexer, char *out);
extern void skip_whitespace(Lexer *lexer);
extern Token lex_next_token(Lexer *lexer);

#endif // LEXER_H
