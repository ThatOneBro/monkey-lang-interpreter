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

typedef size_t LexerHandle;

extern LexerHandle make_lexer(char *input);
extern Lexer *get_lexer(LexerHandle handle);
extern void cleanup_lexer(LexerHandle handle);
extern int read_char(Lexer *lexer);
extern void read_identifier(Lexer *lexer, char *out);
extern void skip_whitespace(Lexer *lexer);
extern Token next_token(LexerHandle handle);

#endif // LEXER_H
