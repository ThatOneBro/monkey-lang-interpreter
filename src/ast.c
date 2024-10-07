#include "ast.h"
#include <stdlib.h>

ASTNode *make_ast_node()
{
    ASTNode *ast_node = malloc(sizeof(ASTNode));
    return ast_node;
}

void cleanup_ast_node(ASTNode *node)
{
    free(node);
}
