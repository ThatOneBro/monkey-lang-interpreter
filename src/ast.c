#include "ast.h"
#include "arrlist_utils.h"
#include "str_utils.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 50

ASTNode *make_ast_node(ASTNodeArrayList *list)
{
    ASTNode *ast_node;
    if (list == NULL) {
        ast_node = (ASTNode *)malloc(sizeof(ASTNode));
    } else {
        ast_node = alloc_node_in_list(list);
    }
    return ast_node;
}

void cleanup_ast_node(ASTNode *node)
{
    if (node == NULL) {
        return;
    }
    free(node);
}

ASTNodeArrayList *make_ast_node_array_list(void)
{
    ASTNodeArrayList *list = (ASTNodeArrayList *)malloc(sizeof(ASTNodeArrayList));
    list->array = (ASTNode *)calloc(INITIAL_CAPACITY, sizeof(ASTNode));
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

void add_ast_node_to_list(ASTNodeArrayList *list, ASTNode *node)
{
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = (ASTNode *)realloc_backing_array(list->array, list->size, list->capacity, sizeof(ASTNode));
    }
    list->array[list->size++] = *node;
}

ASTNode *get_ast_node_from_list(ASTNodeArrayList *list, size_t index)
{
    if (index < 0 || index >= list->size) {
        printf("Index out of bounds\n");
        return NULL;
    }
    return &list->array[index];
}

void set_ast_node_in_list(ASTNodeArrayList *list, size_t index, ASTNode *node)
{
    if (index < 0 || index >= list->size) {
        printf("Index out of bounds\n");
        return;
    }
    list->array[index] = *node;
}

ASTNode *alloc_node_in_list(ASTNodeArrayList *list)
{
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = (ASTNode *)realloc_backing_array(list->array, list->size, list->capacity, sizeof(ASTNode));
    }
    return &list->array[list->size++];
}

void cleanup_ast_node_list(ASTNodeArrayList *list)
{
    free(list->array);
    free(list);
}

Program *make_program(void)
{
    Program *program = (Program *)malloc(sizeof(Program));
    program->array = (ASTNode **)calloc(INITIAL_CAPACITY, sizeof(ASTNode *));
    program->size = 0;
    program->capacity = INITIAL_CAPACITY;
    return program;
}

void cleanup_program(Program *program)
{
    free(program->array);
    free(program);
}

void add_ast_node_to_program(Program *program, ASTNode *node)
{
    if (program->size == program->capacity) {
        program->capacity *= 2;
        program->array = (ASTNode **)realloc_backing_array(program->array, program->size, program->capacity, sizeof(ASTNode *));
    }
    program->array[program->size++] = node;
}

ASTNode *get_nth_statement(Program *program, size_t n)
{
    assert(n >= 0 && n < program->size);
    return program->array[n];
}

// char *expr_to_str(ASTNode *node)
// {
//     switch (node->type) {
//     case NODE_BINARY_EXPR:
//         break;
//     }
// }

char *node_to_str(ASTNode *node)
{
    if (node == NULL) {
        return NULL;
    }
    String *string = make_string();
    switch (node->type) {
    case NODE_LET_STMT:
        copy_str_into_string(string, "let ");
        copy_str_into_string(string, node->data.let_stmt.left->data.identifier);
        copy_str_into_string(string, " = ");

        if (node->data.let_stmt.right) {
            char *value_str = node_to_str(node->data.let_stmt.right);
            copy_str_into_string(string, value_str);
            free(value_str);
        }

        copy_str_into_string(string, ";");
        break;
    case NODE_RETURN_STMT:
        copy_str_into_string(string, "return ");

        if (node->data.return_stmt->data.expr_stmt) {
            char *value_str = node_to_str(node->data.return_stmt->data.expr_stmt);
            copy_str_into_string(string, value_str);
            free(value_str);
        }

        copy_str_into_string(string, ";");
        break;
    case NODE_EXPR_STMT:
        if (node->data.expr_stmt == NULL) {
            return NULL;
        }
        return node_to_str(node->data.expr_stmt);
    case NODE_IDENTIFIER:
        assert(node->data.identifier);
        assert(node->token_literal);
        return strdup(node->token_literal);
    case NODE_PREFIX_EXPR:
        assert(node->data.prefix_expr.operator);
        assert(node->data.prefix_expr.right);

        copy_str_into_string(string, "(");
        copy_str_into_string(string, node->data.prefix_expr.operator);
        char *value_str = node_to_str(node->data.prefix_expr.right);
        copy_str_into_string(string, value_str);
        copy_str_into_string(string, ")");

        free(value_str);
        break;
    case NODE_INFIX_EXPR:
        assert(node->data.infix_expr.left);
        assert(node->data.infix_expr.operator);
        assert(node->data.infix_expr.right);

        copy_str_into_string(string, "(");

        char *value_str_left = node_to_str(node->data.infix_expr.left);
        copy_str_into_string(string, value_str_left);

        copy_str_into_string(string, " ");
        copy_str_into_string(string, node->data.infix_expr.operator);
        copy_str_into_string(string, " ");

        char *value_str_right = node_to_str(node->data.infix_expr.right);
        copy_str_into_string(string, value_str_right);

        copy_str_into_string(string, ")");

        free(value_str_left);
        free(value_str_right);
        break;
    case NODE_LITERAL:
        copy_str_into_string(string, node->token_literal);
        break;
    default:
        printf("Node type: %d\n", node->type);
        assert(1 != 1);
    }

    char *str = get_str_from_string(string);
    cleanup_string(string);
    return str;
}

char *program_to_str(Program *program)
{
    char **strs = malloc(program->size * sizeof(char *));
    for (size_t i = 0; i < program->size; i++) {
        strs[i] = node_to_str(program->array[i]);
    }
    char *result = concat_cstrs(strs, program->size);
    for (size_t i = 0; i < program->size; i++) {
        free(strs[i]);
    }
    free(strs);
    return result;
}
