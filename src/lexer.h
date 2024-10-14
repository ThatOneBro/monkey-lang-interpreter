#ifndef LEXER_H
#define LEXER_H

#include "globals.h"
#include "token.h"
#include <stddef.h>

typedef struct Lexer {
    char *input;
    size_t input_len;
    size_t position;
    size_t read_position;
    char curr_char;
} Lexer;

extern void init_lexer(Lexer *lexer, char *input);
extern Lexer *make_lexer(char *input);
extern void cleanup_lexer(Lexer *lexer);
extern int read_char(Lexer *lexer);
extern void read_identifier(Lexer *lexer, char *out);
extern void skip_whitespace(Lexer *lexer);
extern Token lex_next_token(Lexer *lexer);

#endif // LEXER_H
