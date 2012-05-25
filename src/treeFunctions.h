/*
Copyright (c) 2012 Luke Drummond

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/
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
