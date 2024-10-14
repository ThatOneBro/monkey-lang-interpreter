#include "ast.h"
#include <assert.h>
#include <memory.h>
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
    free(node);
}

ASTNodeArrayList *make_ast_node_array_list()
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
        list->array = (ASTNode *)realloc(list->array, list->capacity * sizeof(ASTNode));
        memset(&list->array[list->size], 0, list->capacity);
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
        list->array = (ASTNode *)realloc(list->array, list->capacity * sizeof(ASTNode));
        memset(&list->array[list->size], 0, list->capacity);
    }
    return &list->array[list->size++];
}

void cleanup_ast_node_list(ASTNodeArrayList *list)
{
    free(list->array);
    free(list);
}

Program *make_program()
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
        program->array = (ASTNode **)realloc(program->array, program->capacity * sizeof(ASTNode *));
        memset(&program->array[program->size], 0, program->capacity);
    }
    program->array[program->size++] = node;
}

ASTNode *get_nth_statement(Program *program, size_t n)
{
    assert(n >= 0 && n < program->size);
    return program->array[n];
}
