#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024

void print_prompt()
{
    printf(">> ");
    fflush(stdout);
}

char *read_input()
{
    char *input = malloc(MAX_INPUT);
    if (!input) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        free(input);
        return NULL;
    }

    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;

    return input;
}

void eval_and_print(const char *input)
{
    LexerHandle l = make_lexer(input);
    Token tok;
    while (tok.type != TOKEN_EOF) {
        tok = next_token(l);
        printf("{ type: %s, literal: %s }\n", token_type_to_str(tok.type), tok.literal);
    }
}

int main()
{
    char *input;

    printf("Welcome to the Basic REPL!\n");
    printf("Type 'exit' to quit.\n");

    while (1) {
        print_prompt();
        input = read_input();

        if (input == NULL) {
            printf("\nGoodbye!\n");
            break;
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            printf("Goodbye!\n");
            break;
        }

        eval_and_print(input);
        free(input);
    }

    return 0;
}
