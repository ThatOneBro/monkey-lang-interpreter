#include "errors.h"
#include "globals.h"
#include "lexer.h"
#include "parser.h"
#include "test_utils.h"
#include <assert.h>
#include <string.h>

#define ASSERT_LITERAL_EXPRESSION_BOOL(expr, expected_value)                                                                                                                  \
    do {                                                                                                                                                                      \
        ASSERT((expr)->type == NODE_LITERAL, "Expected: node of type LITERAL\nGot: node of type %s", node_type_to_str((expr)->type));                                         \
        bool val = expected_value;                                                                                                                                            \
        ASSERT(COMPARE_LITERAL_VALUE(expr->data.literal, LITERAL_BOOL, val), "Incorrect literal boolean value.\nExpected: %s\nGot: %s\n",                                     \
            val ? "true" : "false", ACCESS_BOOL(expr->data.literal) ? "true" : "false");                                                                                      \
        ASSERT(strcmp((expr)->token_literal, val ? "true" : "false") == 0, "Invalid token literal.\nExpected: %s\nGot: %s\n", val ? "true" : "false", (expr)->token_literal); \
    } while (0)

#define ASSERT_LITERAL_EXPRESSION_INT(expr, expected_value)                                                                          \
    do {                                                                                                                             \
        ASSERT((expr)->type == NODE_LITERAL, "Expected: node of type LITERAL\nGot: node of type %d", (expr)->type);                  \
        int val = #expected_value;                                                                                                   \
        ASSERT(COMPARE_LITERAL_VALUE(expr->data.literal, LITERAL_INT, val), "Incorrect literal int value.\nExpected: %d\nGot: %d\n", \
            val, ACCESS_INT(expr->data.literal));                                                                                    \
        int token_as_int = atoi(expr->token_literal);                                                                                \
        ASSERT(token_as_int == val, "Invalid token literal.\nExpected: %d\nGot: %d\n", val, token_as_int);                           \
    } while (0)

#define ASSERT_LITERAL_EXPRESSION_FLOAT(expr, expected_value)                                                                                  \
    do {                                                                                                                                       \
        ASSERT((expr)->type == NODE_LITERAL, "Expected: node of type LITERAL\nGot: node of type %d", (expr)->type);                            \
        float val = #expected_value;                                                                                                           \
        ASSERT(COMPARE_LITERAL_VALUE(expr->data.literal, LITERAL_FLOAT, val), "Incorrect literal float value.\nExpected: %0.4f\nGot: %0.4f\n", \
            val, ACCESS_FLOAT(expr->data.literal));                                                                                            \
        float token_as_float = atof(expr->token_literal);                                                                                      \
        ASSERT(token_as_float == val, "Invalid token literal.\nExpected: %0.4f\nGot: %0.4f\n", val, token_as_float);                           \
    } while (0)

#define ASSERT_LITERAL_EXPRESSION_IDENTIFIER(expr, expected_value)                                                                                 \
    do {                                                                                                                                           \
        ASSERT((expr)->type == NODE_LITERAL, "Expected: node of type LITERAL\nGot: node of type %d", (expr)->type);                                \
        char *val = #expected_value;                                                                                                               \
        ASSERT(COMPARE_LITERAL_VALUE(expr->data.literal, LITERAL_IDENTIFIER, val), "Incorrect literal identifier value.\nExpected: %s\nGot: %s\n", \
            val, ACCESS_IDENTIFIER(expr->data.literal));                                                                                           \
        ASSERT(strcmp(expr->token_literal, val) == 0, "Invalid token literal.\nExpected: %s\nGot: %s\n", val, expr->token_literal);                \
    } while (0)

#define ASSERT_LITERAL_EXPRESSION_STRING(expr, expected_value)                                                                             \
    do {                                                                                                                                   \
        ASSERT((expr)->type == NODE_LITERAL, "Expected: node of type LITERAL\nGot: node of type %d", (expr)->type);                        \
        char *val = #expected_value;                                                                                                       \
        ASSERT(COMPARE_LITERAL_VALUE(expr->data.literal, LITERAL_STRING, val), "Incorrect literal string value.\nExpected: %s\nGot: %s\n", \
            val, ACCESS_STRING(expr->data.literal));                                                                                       \
        ASSERT(strcmp(expr->token_literal, val) == 0, "Invalid token literal.\nExpected: %s\nGot: %s\n", val, expr->token_literal);        \
    } while (0)

#define ASSERT_INFIX_EXPRESSION(exp, left_value, operator, right_value, left_type, right_type) \
    do {                                                                                       \
        ASTNode *op_exp = (exp);                                                               \
        ASSERT(op_exp->type == NODE_INFIX_EXPR,                                                \
            "Node is not an infix expression.\nGot: %s", op_exp->type);                        \
                                                                                               \
        ASSERT_LITERAL_EXPRESSION_##left_type(op_exp->left, left_value);                       \
                                                                                               \
        ASSERT(strcmp(op_exp->operator, operator) == 0,                                        \
            "Operator is not '%s'\nGot: %s", operator, op_exp->operator);                      \
                                                                                               \
        ASSERT_LITERAL_EXPRESSION_##right_type(op_exp->right, right_value);                    \
    } while (0)

// Helper macros for different types
#define ASSERT_INFIX_EXPRESSION_INT(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, INT, INT)

#define ASSERT_INFIX_EXPRESSION_FLOAT(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, FLOAT, FLOAT)

#define ASSERT_INFIX_EXPRESSION_BOOL(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, BOOL, BOOL)

#define ASSERT_INFIX_EXPRESSION_STRING(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, STRING, STRING)

#define ASSERT_INFIX_EXPRESSION_IDENTIFIER(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, IDENTIFIER, IDENTIFIER)

// Mixed type macros
#define ASSERT_INFIX_EXPRESSION_INT_FLOAT(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, INT, FLOAT)

#define ASSERT_INFIX_EXPRESSION_FLOAT_INT(exp, left, op, right) \
    ASSERT_INFIX_EXPRESSION(exp, left, op, right, FLOAT, INT)

INIT_TEST_HARNESS()

void check_parser_errors(Parser *parser)
{
    if (parser->errors->size == 0) {
        return;
    }
    printf("Parser encountered %zu error(s)\n", parser->errors->size);
    for (size_t i = 0; i < parser->errors->size; i++) {
        // Print error
        printf("%s\n", get_error_from_arraylist(parser->errors, i));
    }
    assert(1 != 1);
}

void assert_integer_literal(ASTNode *expr, int64_t val)
{
    assert(expr->type == NODE_LITERAL);
    assert(expr->data.literal.type == LITERAL_INT);
    assert(expr->data.literal.value.int_value == val);
}

void assert_identifier(ASTNode *expr, char *val)
{
    assert(expr->type == NODE_IDENTIFIER);
    assert(expr->data.literal.type == LITERAL_IDENTIFIER);
    assert(strcmp(&expr->data.literal.value.identifier, val) == 0);
    assert(strcmp(expr->token_literal, val) == 0);
}

void assert_let_statement(ASTNode *expr, char *name)
{
    ASSERT(strcmp(expr->token_literal, "let") == 0, "Got invalid token literal in let expression.\n\nExpected: %s\nGot: %s", "let", expr->token_literal);
    ASSERT(strcmp(expr->data.let_stmt.left->data.literal.value.identifier, name) == 0, "Got invalid identifier in let expression.\n\nExpected: %s\nGot: %s", name, expr->data.let_stmt.left->data.literal.value.identifier);
    ASSERT(strcmp(expr->data.let_stmt.left->token_literal, name) == 0, "Got invalid token literal in identifier.\n\nExpected: %s\nGot: %s", name, expr->data.let_stmt.left->token_literal);
}

TEST_CASE(let_statements)
{
    struct {
        char *input;
        char *expected_identifier;
    } tests[] = { { "let x = 5;\n", "x" }, { "let y = 10;\n", "y" }, { "let foobar = 838383;\n", "foobar" } };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        Parser *parser = make_parser(tests[i].input);
        Program *program = parse_program(parser);

        check_parser_errors(parser);

        assert(program != NULL);
        assert(program->size == 1);

        ASTNode *statement = get_nth_statement(program, 0);
        assert(statement != NULL);

        assert_let_statement(statement, tests[i].expected_identifier);

        cleanup_program(program);
        cleanup_parser(parser);
    }
}

TEST_CASE(return_statements)
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

TEST_CASE(identifier_expressions)
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
    assert(strcmp(statement->data.expr_stmt->data.literal.value.identifier, "foobar") == 0);
    assert(strcmp(statement->data.expr_stmt->token_literal, "foobar") == 0);

    cleanup_program(program);
    cleanup_parser(parser);
}

TEST_CASE(integer_literal_expressions)
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

TEST_CASE(boolean_literal_expressions)
{
    const char input[]
        = "true;\n"
          "false;\n"
          "let foobar = true;"
          "let barfoo = false;";

    Parser *parser = make_parser(input);
    Program *program = parse_program(parser);

    check_parser_errors(parser);

    assert(program != NULL);
    assert(program->size == 4);

    ASTNode *statement = get_nth_statement(program, 0);
    assert(statement->type == NODE_EXPR_STMT);
    ASSERT_LITERAL_EXPRESSION_BOOL(statement->data.expr_stmt, TRUE);

    statement = get_nth_statement(program, 1);
    assert(statement->type == NODE_EXPR_STMT);
    ASSERT_LITERAL_EXPRESSION_BOOL(statement->data.expr_stmt, FALSE);

    // statement = get_nth_statement(program, 2);
    // ASSERT_INFIX_EXPRESSION_IDENTIFIER(statement, TRUE);

    //     statement = get_nth_statement(program, 3);
    // ASSERT_INFIX_EXPRESSION_IDENTIFIER(statement, TRUE);

    cleanup_program(program);
    cleanup_parser(parser);
}

TEST_CASE(parsing_prefix_operators)
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
        assert_integer_literal(statement->data.expr_stmt->data.prefix_expr.right, prefix_tests[i].int_value);

        cleanup_program(program);
        cleanup_parser(parser);
    }
}

TEST_CASE(parsing_infix_operators)
{
    struct {
        const char *input;
        int64_t left_value;
        const char *operator;
        int64_t right_value;
    } infix_tests[] = {
        { "5 + 5;", 5, "+", 5 },
        { "5 - 5;", 5, "-", 5 },
        { "5 * 5;", 5, "*", 5 },
        { "5 / 5;", 5, "/", 5 },
        { "5 > 5;", 5, ">", 5 },
        { "5 < 5;", 5, "<", 5 },
        { "5 == 5;", 5, "==", 5 },
        { "5 != 5;", 5, "!=", 5 }
    };

    size_t test_count = sizeof(infix_tests) / sizeof(infix_tests[0]);

    for (size_t i = 0; i < test_count; i++) {
        Parser *parser = make_parser(infix_tests[i].input);
        Program *program = parse_program(parser);

        check_parser_errors(parser);

        assert(program != NULL);
        assert(program->size == 1);

        ASTNode *statement = get_nth_statement(program, 0);
        assert(statement != NULL);
        assert(statement->type == NODE_EXPR_STMT);
        assert(statement->data.expr_stmt);

        assert(strcmp(statement->data.expr_stmt->data.infix_expr.operator, infix_tests[i].operator) == 0);
        assert_integer_literal(statement->data.expr_stmt->data.infix_expr.left, infix_tests[i].left_value);
        assert_integer_literal(statement->data.expr_stmt->data.infix_expr.right, infix_tests[i].right_value);

        cleanup_program(program);
        cleanup_parser(parser);
    }
}

TEST_CASE(operator_precedence_parsing)
{
    struct {
        const char *input;
        const char *expected;
    } tests[] = {
        { "-a * b", "((-a) * b)" },
        { "!-a", "(!(-a))" },
        { "a + b + c", "((a + b) + c)" },
        { "a + b - c", "((a + b) - c)" },
        { "a * b * c", "((a * b) * c)" },
        { "a * b / c", "((a * b) / c)" },
        { "a + b / c", "(a + (b / c))" },
        { "a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)" },
        { "3 + 4; -5 * 5", "(3 + 4)((-5) * 5)" },
        { "5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))" },
        { "5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))" },
        { "3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))" }
    };

    size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;
    int failed = 0;

    for (size_t i = 0; i < num_tests; i++) {
        Parser *parser = make_parser(tests[i].input);
        Program *program = parse_program(parser);

        check_parser_errors(parser);

        assert(program);

        char *actual = program_to_str(program);
        if (strcmp(actual, tests[i].expected) == 0) {
            passed++;
        } else {
            failed++;
            printf("Test failed: input='%s', expected='%s', got='%s'\n",
                tests[i].input, tests[i].expected, actual);
        }

        cleanup_program(program);
        cleanup_parser(parser);
    }

    printf("Tests passed: %d\n", passed);
    printf("Tests failed: %d\n", failed);
}

RUN_TESTS()
