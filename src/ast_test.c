#include "lexer.h"
#include "parser.h"
#include "test_utils.h"
#include <assert.h>
#include <string.h>

INIT_TEST_HARNESS()

TEST_CASE(program_to_str)
{
    ASTNodeArrayList *node_list = make_ast_node_array_list();
    Program *program = make_program();

    ASTNode *let_stmt_node = make_ast_node(node_list);
    let_stmt_node->type = NODE_LET_STMT;
    strcpy(&let_stmt_node->token_literal, "let");

    // Left node
    ASTNode *ident_node = make_ast_node(node_list);
    ident_node->type = NODE_IDENTIFIER;
    strcpy(&ident_node->token_literal, "myVar");
    ident_node->data.literal.type = LITERAL_IDENTIFIER;
    strcpy(&ident_node->data.literal.value.identifier, "myVar");

    let_stmt_node->data.let_stmt.left = ident_node;

    // Right node
    ASTNode *value_node = make_ast_node(node_list);
    value_node->type = NODE_IDENTIFIER;
    strcpy(&value_node->token_literal, "anotherVar");
    ident_node->data.literal.type = LITERAL_IDENTIFIER;
    strcpy(&value_node->data.literal.value.identifier, "anotherVar");

    let_stmt_node->data.let_stmt.right = value_node;

    add_ast_node_to_program(program, let_stmt_node);

    assert(strcmp(program_to_str(program), "let myVar = anotherVar;") == 0);

    cleanup_ast_node_list(node_list);
    cleanup_program(program);
}

RUN_TESTS()
