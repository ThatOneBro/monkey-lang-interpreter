#include "lexer.h"
#include "parser.h"
#include "test_utils.h"
#include <assert.h>
#include <string.h>

INIT_TEST_HARNESS()

void assert_let_statement(ASTNode *statement, char *expected_identifier)
{
    assert(strcmp(statement->token_literal, "let") == 0);
    assert(strcmp(statement->data.assignment.identifier->token_literal, expected_identifier) == 0);
    assert(strcmp(statement->data.assignment.identifier->data.identifier, expected_identifier) == 0);
}

TEST_CASE(let_statement)
{
    const char input[]
        = "let x = 5;\n"
          "let y = 10;\n"
          "let foobar = 838383;\n";

    Parser *parser = make_parser(input);
    Program *program = parse_program(parser);

    assert(program != NULL);
    assert(program->size == 3);

    struct {
        char *expected_identifier;
    } tests[] = { { "x" }, { "y" }, { "foobar" } };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        ASTNode *statement = get_nth_statement(program, i);
        assert(statement != NULL);
        assert_let_statement(statement, tests[i].expected_identifier);
    }

    cleanup_program(program);
    cleanup_parser(parser);
}

RUN_TESTS()
