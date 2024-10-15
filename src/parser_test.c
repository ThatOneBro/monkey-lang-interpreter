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

TEST_CASE(let_statements)
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
    assert(strcmp(statement->data.expr_stmt->data.identifier, "foobar") == 0);
}

RUN_TESTS()
