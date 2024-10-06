#include "lexer.h" // Assume this contains the Token definition and read_char() function
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TEST_CASE(name) static void test_##name()
#define RUN_TEST(name)                     \
    printf("Running test_%s...\n", #name); \
    test_##name();                         \
    printf("test_%s passed.\n\n", #name)

// Helper function to compare tokens
static void assert_token(Token *expected, Token *actual)
{
    assert(expected->type == actual->type);
    assert(strcmp(expected->literal, actual->literal) == 0);
}

// Test function
TEST_CASE(next_token_works)
{
    char *input = "=+(){},;";
    LexerHandle l = make_lexer(input, sizeof(input));

    struct {
        TokenType expectedType;
        char *expectedLiteral;
    } tests[] = { { TOKEN_ASSIGN, "=" }, { TOKEN_PLUS, "+" }, { TOKEN_LPAREN, "(" },
        { TOKEN_RPAREN, ")" }, { TOKEN_LBRACE, "{" }, { TOKEN_RBRACE, "}" },
        { TOKEN_COMMA, "," }, { TOKEN_SEMICOLON, ";" }, { TOKEN_EOF, "" } };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        Token tok = next_token(l);

        assert(tok.type == tests[i].expectedType);
        assert(strcmp(tok.literal, tests[i].expectedLiteral) == 0);

        printf("Test %d passed\n", i + 1);
    }

    //   free(l);
    cleanup_lexer(l);
    //   printf("All tests passed successfully!\n");
}

// TEST_CASE(simple_assignment)
// {
//     const char *input = "let x = 5;";
//     Token expected[] = { { TOKEN_LET, "let" }, { TOKEN_IDENT, "x" },
//         { TOKEN_ASSIGN, "=" }, { TOKEN_INT, "5" },
//         { TOKEN_SEMICOLON, ";" }, { TOKEN_EOF, "EOF" } };

//     Token *tokens = lex(input);

//     for (int i = 0; i < 6; i++) {
//         assert_token(&expected[i], &tokens[i]);
//     }

//     // Don't forget to free the tokens if they were dynamically allocated
//     // free(tokens);
// }

// TEST_CASE(function_declaration)
// {
//     const char *input = "fn add(x, y) { x + y }";
//     Token expected[] = {
//         { TOKEN_FUNCTION, "fn" }, { TOKEN_IDENT, "add" }, { TOKEN_LPAREN, "(" },
//         { TOKEN_IDENT, "x" }, { TOKEN_COMMA, "," }, { TOKEN_IDENT, "y" },
//         { TOKEN_RPAREN, ")" }, { TOKEN_LBRACE, "{" }, { TOKEN_IDENT, "x" },
//         { TOKEN_PLUS, "+" }, { TOKEN_IDENT, "y" }, { TOKEN_RBRACE, "}" },
//         { TOKEN_EOF, "EOF" }
//     };

//     Token *tokens = lex(input);

//     for (int i = 0; i < 13; i++) {
//         assert_token(&expected[i], &tokens[i]);
//     }

//     // free(tokens);
// }

int main()
{
    RUN_TEST(next_token_works);
    // RUN_TEST(simple_assignment);
    // RUN_TEST(function_declaration);

    printf("All tests passed!\n");
    return 0;
}