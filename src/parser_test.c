#include "lexer.h"
#include "parser.h"
#include "test_utils.h"
#include <assert.h>
#include <string.h>

INIT_TEST_HARNESS()

void check_parser_errors(Parser *parser)
{
    if (parser->errors->size == 0) {
        return;
    }
    printf("Parser encountered %d error(s)\n", parser->errors->size);
    for (size_t i = 0; i < parser->errors->size; i++) {
        // Print error
        printf("%s\n", get_error_from_arraylist(parser->errors, i));
    }
    assert(1 != 1);
}

void test_integer_literal(ASTNode *expr, int64_t val)
{
    assert(expr->type == NODE_LITERAL);
    assert(expr->data.literal.type == LITERAL_INT);
    assert(expr->data.literal.value.int_value == val);
}

TEST_CASE(let_statement)
{
    const char input[]
        = "let x = 5;\n"
          "let y = 10;\n"
          "let foobar = 838383;\n";

    Parser *parser = make_parser(input);
    Program *program = parse_program(parser);

    check_parser_errors(parser);

    assert(program != NULL);
    assert(program->size == 3);

    struct {
        char *expected_identifier;
    } tests[] = { { "x" }, { "y" }, { "foobar" } };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        ASTNode *statement = get_nth_statement(program, i);
        assert(statement != NULL);

        assert(strcmp(statement->token_literal, "let") == 0);
        assert(strcmp(statement->data.let_stmt.left->token_literal, tests[i].expected_identifier) == 0);
        assert(strcmp(statement->data.let_stmt.left->data.identifier, tests[i].expected_identifier) == 0);
    }

    cleanup_program(program);
    cleanup_parser(parser);
}

TEST_CASE(return_statement)
{
    const char input[]
        = "return 5;\n"
          "return 10;\n"
          "return 993322;\n";

    Parser *parser = make_parser(input);
    Program *program = parse_program(parser);

    check_parser_errors(parser);

    assert(program != NULL);
    assert(program->size == 3);

    for (int i = 0; i < program->size; i++) {
        ASTNode *statement = get_nth_statement(program, i);
        assert(statement != NULL);
        assert(strcmp(statement->token_literal, "return") == 0);
    }

    cleanup_program(program);
    cleanup_parser(parser);
}

TEST_CASE(identifier_expression)
{
    const char input[]
        = "foobar;\n";

    Parser *parser = make_parser(input);
    Program *program = parse_program(parser);

    check_parser_errors(parser);

    assert(program != NULL);
    assert(program->size == 1);

    ASTNode *statement = get_nth_statement(program, 0);
    assert(statement != NULL);
    assert(statement->type == NODE_EXPR_STMT);
    assert(statement->data.expr_stmt);
    assert(statement->data.expr_stmt->type == NODE_IDENTIFIER);
    assert(strcmp(statement->data.expr_stmt->data.identifier, "foobar") == 0);
    assert(strcmp(statement->data.expr_stmt->token_literal, "foobar") == 0);

    cleanup_program(program);
    cleanup_parser(parser);
}

TEST_CASE(integer_literal_expression)
{
    const char input[]
        = "10;\n";

    Parser *parser = make_parser(input);
    Program *program = parse_program(parser);

    check_parser_errors(parser);

    assert(program != NULL);
    assert(program->size == 1);

    ASTNode *statement = get_nth_statement(program, 0);
    assert(statement != NULL);
    assert(statement->type == NODE_EXPR_STMT);
    assert(statement->data.expr_stmt);
    assert(statement->data.expr_stmt->type == NODE_LITERAL);
    assert(statement->data.expr_stmt->data.literal.type == LITERAL_INT);
    assert(statement->data.expr_stmt->data.literal.value.int_value == 10);
    assert(strcmp(statement->data.expr_stmt->token_literal, "10") == 0);

    cleanup_program(program);
    cleanup_parser(parser);
}

TEST_CASE(parsing_prefix_operator)
{
    struct {
        const char *input;
        const char *operator;
        int64_t int_value;
    } prefix_tests[] = {
        { "!5;", "!", 5 },
        { "-15;", "-", 15 }
    };

    size_t test_count = sizeof(prefix_tests) / sizeof(prefix_tests[0]);

    for (size_t i = 0; i < test_count; i++) {
        Parser *parser = make_parser(prefix_tests[i].input);
        Program *program = parse_program(parser);

        check_parser_errors(parser);

        assert(program != NULL);
        assert(program->size == 1);

        ASTNode *statement = get_nth_statement(program, 0);
        assert(statement != NULL);
        assert(statement->type == NODE_EXPR_STMT);
        assert(statement->data.expr_stmt);

        assert(strcmp(statement->data.expr_stmt->data.prefix_expr.operator, prefix_tests[i].operator) == 0);
        test_integer_literal(statement->data.expr_stmt->data.prefix_expr.right, prefix_tests[i].int_value);

        cleanup_program(program);
        cleanup_parser(parser);
    }
}

// TEST_CASE(parsing_infix_operator)
// {
//     struct InfixTest {
//         const char *input;
//         int64_t left_value;
//         const char *operator;
//         int64_t right_value;
//     };

//     struct InfixTest infixTests[] = {
//         { "5 + 5;", 5, "+", 5 },
//         { "5 - 5;", 5, "-", 5 },
//         { "5 * 5;", 5, "*", 5 },
//         { "5 / 5;", 5, "/", 5 },
//         { "5 > 5;", 5, ">", 5 },
//         { "5 < 5;", 5, "<", 5 },
//         { "5 == 5;", 5, "==", 5 },
//         { "5 != 5;", 5, "!=", 5 }
//     };

//     const size_t infixTestsCount = sizeof(infixTests) / sizeof(infixTests[0]);
//     struct {
//         const char *input;
//         OperatorType operator;
//         int64_t int_value;
//     } prefix_tests[] = {
//         { "!5;", "!", 5 },
//         { "-15;", "-", 15 }
//     };

//     size_t test_count = sizeof(prefix_tests) / sizeof(prefix_tests[0]);

//     for (size_t i = 0; i < test_count; i++) {
//         Parser *parser = make_parser(prefix_tests[i].input);
//         Program *program = parse_program(parser);

//         check_parser_errors(parser);

//         assert(program != NULL);
//         assert(program->size == 1);

//         ASTNode *statement = get_nth_statement(program, 0);
//         assert(statement != NULL);
//         assert(statement->type == NODE_EXPR_STMT);
//         assert(statement->data.expr_stmt);

//         assert(statement->data.expr_stmt->data.prefix_expr.operator== prefix_tests[i].operator);
//         test_integer_literal(statement->data.expr_stmt->data.prefix_expr.right, prefix_tests[i].int_value);

//         cleanup_program(program);
//         cleanup_parser(parser);
//     }
// }

RUN_TESTS()
