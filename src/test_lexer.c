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
        TokenType expected_type;
        char *expected_literal;
    } tests[] = { { TOKEN_ASSIGN, "=" }, { TOKEN_PLUS, "+" }, { TOKEN_LPAREN, "(" },
        { TOKEN_RPAREN, ")" }, { TOKEN_LBRACE, "{" }, { TOKEN_RBRACE, "}" },
        { TOKEN_COMMA, "," }, { TOKEN_SEMICOLON, ";" }, { TOKEN_EOF, "" } };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        Token tok = next_token(l);

        assert(tok.type == tests[i].expected_type);
        assert(strcmp(tok.literal, tests[i].expected_literal) == 0);

        printf("Test %d passed\n", i + 1);
    }

    cleanup_lexer(l);
}

// TEST_CASE(simple_assignment)
// {
//     const char *input = "let x = 5;";
//     Token expected[] = { { TOKEN_LET, "let" }, { TOKEN_IDENT, "x" },
//         { TOKEN_ASSIGN, "=" }, { TOKEN_INT, "5" },
//         { TOKEN_SEMICOLON, ";" }, { TOKEN_EOF, "EOF" } };

//     LexerHandle l = make_lexer(input, sizeof(input));

//     size_t expected_len = sizeof(expected) / sizeof(struct Token);
//     for (int i = 0; i < expected_len; i++) {
//         Token tok = next_token(l);
//         assert_token(&expected[i], &tok);
//     }

//     cleanup_lexer(l);
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

//     LexerHandle l = make_lexer(input, sizeof(input));

//     size_t expected_len = sizeof(expected) / sizeof(struct Token);
//     for (int i = 0; i < expected_len; i++) {
//         Token tok = next_token(l);
//         assert_token(&expected[i], &tok);
//     }

//     cleanup_lexer(l);
// }

int main()
{
    RUN_TEST(next_token_works);
    // RUN_TEST(simple_assignment);
    // RUN_TEST(function_declaration);

    printf("All tests passed!\n");
    return 0;
}