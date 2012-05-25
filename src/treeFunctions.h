#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>
#include "xmlDataTypes.h"


#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H
node * xmlTreeAddChildNode(node * tree, int val);
node * xmlTreeAddSiblingNode(node * tree, int val);
node * xmlTreeSearchElementSiblingsID(node * tree, wchar_t * elementID);
node * xmlTreeSearchSubElementID(node * tree, wchar_t * elementID);
node * xmlTreeHasNode(node * tree, int val);
node * xmlTreeTop(node * tree);
node * xmlTreeUp(node * tree, int n);
node * xmlFindNthParent(node * tree, int n);
void xmlTreePrint(node * tree);
void xmlTreeFree(node * tree);

int xmlTreeSaveGraphviz(node * tree, const char * filename, FILE * file, int depth);
#endif
