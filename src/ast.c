#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10

ASTNode *make_ast_node()
{
    ASTNode *ast_node = malloc(sizeof(ASTNode));
    return ast_node;
}

void cleanup_ast_node(ASTNode *node)
{
    free(node);
}

ASTNodeArrayList *make_ast_node_array_list()
{
    ASTNodeArrayList *list = (ASTNodeArrayList *)malloc(sizeof(ASTNodeArrayList));
    list->array = (ASTNode *)malloc(INITIAL_CAPACITY * sizeof(ASTNode));
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

void add_ast_node_to_list(ASTNodeArrayList *list, ASTNode *node)
{
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = (ASTNode *)realloc(list->array, list->capacity * sizeof(ASTNode));
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

void free_ast_node_list(ASTNodeArrayList *list)
{
    free(list->array);
    free(list);
}
