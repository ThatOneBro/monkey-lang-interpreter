#include "lexer.h"
#include "test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

INIT_TEST_HARNESS()

// Helper function to compare tokens
static void assert_token(Token *expected, Token *actual)
{
    assert(expected->type == actual->type);
    assert(strcmp(expected->literal, actual->literal) == 0);
}

// Test function
TEST_CASE(lex_next_token_basic)
{
    const char input[] = "=+(){},;";
    Lexer *l = make_lexer(input);

    struct {
        TokenType expected_type;
        char *expected_literal;
    } tests[] = { { TOKEN_ASSIGN, "=" }, { TOKEN_PLUS, "+" }, { TOKEN_LPAREN, "(" },
        { TOKEN_RPAREN, ")" }, { TOKEN_LBRACE, "{" }, { TOKEN_RBRACE, "}" },
        { TOKEN_COMMA, "," }, { TOKEN_SEMICOLON, ";" }, { TOKEN_EOF, "" } };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        Token tok = lex_next_token(l);

        printf("Test %d - expected token type: %s, got: %s\n", i + 1, token_type_to_str(tests[i].expected_type), token_type_to_str(tok.type));
        printf("Test %d - expected token literal: %s, got: %s\n", i + 1, tests[i].expected_literal, tok.literal);

        assert(tok.type == tests[i].expected_type);
        assert(strcmp(tok.literal, tests[i].expected_literal) == 0);

        printf("Test %d passed\n", i + 1);
    }

    cleanup_lexer(l);
}

TEST_CASE(lex_next_token_advanced)
{
    const char input[] = "let five = 5;\n"
                         "let ten = 10;\n"
                         "\n"
                         "let add = fn(x, y) {\n"
                         "    x + y;\n"
                         "};\n"
                         "let result = add(five, ten);\n"
                         "!-/*5;\n"
                         "5 < 10 > 5;\n"
                         "\n"
                         "if (5 < 10) {\n"
                         "    return true;\n"
                         "} else {\n"
                         "    return false;\n"
                         "}\n"
                         "\n"
                         "10 == 10;\n"
                         "10 != 9;\n";

    Lexer *l = make_lexer(input);

    struct {
        TokenType expected_type;
        char *expected_literal;
    } tests[] = {
        { TOKEN_LET, "let" },
        { TOKEN_IDENT, "five" },
        { TOKEN_ASSIGN, "=" },
        { TOKEN_INT, "5" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_LET, "let" },
        { TOKEN_IDENT, "ten" },
        { TOKEN_ASSIGN, "=" },
        { TOKEN_INT, "10" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_LET, "let" },
        { TOKEN_IDENT, "add" },
        { TOKEN_ASSIGN, "=" },
        { TOKEN_FUNCTION, "fn" },
        { TOKEN_LPAREN, "(" },
        { TOKEN_IDENT, "x" },
        { TOKEN_COMMA, "," },
        { TOKEN_IDENT, "y" },
        { TOKEN_RPAREN, ")" },
        { TOKEN_LBRACE, "{" },
        { TOKEN_IDENT, "x" },
        { TOKEN_PLUS, "+" },
        { TOKEN_IDENT, "y" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_RBRACE, "}" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_LET, "let" },
        { TOKEN_IDENT, "result" },
        { TOKEN_ASSIGN, "=" },
        { TOKEN_IDENT, "add" },
        { TOKEN_LPAREN, "(" },
        { TOKEN_IDENT, "five" },
        { TOKEN_COMMA, "," },
        { TOKEN_IDENT, "ten" },
        { TOKEN_RPAREN, ")" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_BANG, "!" },
        { TOKEN_MINUS, "-" },
        { TOKEN_SLASH, "/" },
        { TOKEN_ASTERISK, "*" },
        { TOKEN_INT, "5" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_INT, "5" },
        { TOKEN_LT, "<" },
        { TOKEN_INT, "10" },
        { TOKEN_GT, ">" },
        { TOKEN_INT, "5" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_IF, "if" },
        { TOKEN_LPAREN, "(" },
        { TOKEN_INT, "5" },
        { TOKEN_LT, "<" },
        { TOKEN_INT, "10" },
        { TOKEN_RPAREN, ")" },
        { TOKEN_LBRACE, "{" },
        { TOKEN_RETURN, "return" },
        { TOKEN_TRUE, "true" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_RBRACE, "}" },
        { TOKEN_ELSE, "else" },
        { TOKEN_LBRACE, "{" },
        { TOKEN_RETURN, "return" },
        { TOKEN_FALSE, "false" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_RBRACE, "}" },
        { TOKEN_INT, "10" },
        { TOKEN_EQ, "==" },
        { TOKEN_INT, "10" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_INT, "10" },
        { TOKEN_NOT_EQ, "!=" },
        { TOKEN_INT, "9" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_EOF, "" }
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        Token tok = lex_next_token(l);

        printf("Test %d - expected token type: %s, got: %s\n", i + 1, token_type_to_str(tests[i].expected_type), token_type_to_str(tok.type));
        printf("Test %d - expected token literal: %s, got: %s\n", i + 1, tests[i].expected_literal, tok.literal);

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

RUN_TESTS()
