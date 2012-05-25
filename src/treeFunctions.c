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
#include "treeFunctions.h"

void xmlTreeFree(node * tree)
{
	#ifdef DEBUG
	fprintf(stderr, "xmlTreeFree()\n");
	#endif
/*
remove all nodes in a given tree. and free all dynamically allocated memory
can be called on any subtree, or the root node
*/

	void (*recurse)(node *tree) = xmlTreeFree;	//function pointer for recursion

	if(!tree)	return;
	node *p = tree->parent;		//pointer to the parent node
	node *l = tree->left;		//left child node: for less than
	node *r = tree->right;		//right child node for more than or equal

/*find all nodes with two null children by recursion*/
	while(l !=NULL)
	{
		(void)recurse(l);
		l = tree->left;
	}
	while(r !=NULL)
	{
		(void)recurse(r);
		r = tree->right;
	}

/*
do the actual freeing: 
find out by comparing pointers whether this subtree is a left or right child or is root
*/
	if(l == NULL && r == NULL)
	{
		if (p !=NULL)	//ignore root node case
		{
			if (p->left == tree)
			{
				p->left = NULL;		//clear parent's reference to this child
			}
			else if(p->right == tree)
			{
				p->right = NULL;	//clear parent's reference to this child
			}	
		}
	
		free(tree->xmlData.elementID);
		tree->xmlData.elementID = NULL;
		free(tree->xmlData.elementAttr);
		tree->xmlData.elementAttr = NULL;
		free(tree->xmlData.elementData);
		tree->xmlData.elementData = NULL;
		free(tree);
		tree = NULL;
	}
	return;
}


node * xmlTreeAddChildNode(node * tree, int n)
{
	node * newNode = NULL;

	if(!tree)	/* if the tree pointer is NULL we need a new binary tree */
	{
		tree = calloc(sizeof(node), 1);
		if(tree !=NULL)
		{
			tree->parent = NULL;
			tree->left = NULL;
			tree->right = NULL;
			tree->data = 0;
			newNode = tree;
		}
	}
	else if(tree)
	{
		newNode = calloc(sizeof(node), 1);
		
		if(newNode !=NULL)
		{
			newNode->parent = tree;
			//assert(tree->left=NULL);
			if(!(tree->left))
				tree->left = newNode;	//children on the left
			else fprintf(stderr, "treeAddChildNode():ERROR:: NODE %d ALREADY HAS CHILD\n", n);
			
			newNode->data = n;
			newNode->member = L"child";
			newNode->xmlData.elementID = NULL;
			newNode->xmlData.elementAttr = NULL;
			newNode->xmlData.elementData = NULL;
		}
	}
	return newNode;
}


node * xmlTreeAddSiblingNode(node * tree, int n)
{
	node * newNode = NULL;
	if(!tree)	/* if the tree pointer is NULL we need a new binary tree*/
	{
		tree = calloc(sizeof(node), 1);
		if(tree !=NULL)
		{
			tree->parent = NULL;
			tree->left = NULL;
			tree->right = NULL;
			tree->data = 0;
		}
	}
	else if(tree)
	{
		newNode = calloc(sizeof(node), 1);	//generate new node
		
		if(newNode !=NULL)
		{
			
			while(tree->right !=NULL)	//find end of branch
			{
				tree = tree->right;
			}
			newNode->parent = tree;
			if(!(tree->right))
				tree->right = newNode;	//siblings on the right

			newNode->data = n;
			newNode->member = L"sibling";
			newNode->xmlData.elementID = NULL;
			newNode->xmlData.elementAttr = NULL;
			newNode->xmlData.elementData = NULL;
		}
	}
	return newNode;
}


node * xmlTreeSearchElementSiblingsID(node * tree, wchar_t * elementID)
/*
searches a given tree for a tag returning NULL if the tag is not found - otherwise a pointer to the first node found
*/
{
	node * found = NULL;	// to store pointers returned so they can be tested before returning NULL
	node * (*recurse)(node *tree, wchar_t *elementID) = xmlTreeSearchElementSiblingsID;	// recursive definition of this function
	if(!tree)
	{
		return NULL;
	}
	node *l = tree->left;
	node *r = tree->right;

/*see if we've found what we need*/
	if((wcscmp(tree->xmlData.elementID, elementID)) == 0)
	{	
		#ifdef DEBUG
		fprintf(stderr,"xmlTreeSearchElementSiblingsID - elementID:%ls\n", tree->xmlData.elementID);
		#endif
		return tree;
	}
	
	if (r)
	{
		found = recurse(r, elementID);
		if(found)
		{
			#ifdef DEBUG
			fprintf(stderr,"xmlTreeSearchElementSiblingsID - elementID:%ls\n", found->xmlData.elementID);
			#endif
			return found;
		}
	}
	if (l)
	{
		found = recurse(l, elementID);
		if(found)
		{
			#ifdef DEBUG
			fprintf(stderr,"xmlTreeSearchElementSiblingsID() - elementID:%ls\n", found->xmlData.elementID);
			#endif
			return found;
		}
	}
	return NULL;
}


node * xmlTreeSearchSubElementID(node * tree, wchar_t * elementID)
{
	if(tree)
	{
		if(wcscmp(tree->xmlData.elementID, elementID) == 0)
		{	
			#ifdef DEBUG
			fprintf(stderr,"xmlTreeSearchSubElementID() - elementID:%ls\n", tree->xmlData.elementID);
			#endif
			return tree;
		}
		if(tree->left)
			return xmlTreeSearchElementSiblingsID(tree->left, elementID);
	}
	return NULL;	
}


node * xmlTreeHasNode(node * tree, int val)
/*
searches a given tree for val
returns a pointer to the node in which it was found, or NULL on failure
TODO: add the ability to return multiple values in the case that there is more than one instance of a given value
*/
{
	if(!tree)
	{
		return NULL;
	}
	node * found = NULL;									// to store pointers returned so they can be tested before returning NULL
	node * (*recurse)(node * tree, int val) = xmlTreeHasNode;	// recursive definition of this function
	node * l = tree->left;
	node * r = tree->right;

/*see if we've found what we need*/
	if(tree->data == val)
	{
		return tree;
	}
	
/*
* otherwise recursively search lower leaves until it's found or not
* everything will propagate upwards on return. recurse is just a useful alias for this function
*/	if(l)
	{	
		found = recurse(l, val);
		if(found)	
			return found;
	}
	if(r)
	{
		found = recurse(r, val);
	}
	return found;
}


void xmlTreePrint(node * tree)
{
	void (*recurse)(node *tree) = xmlTreePrint;	//recursive definition of this function

	if(!tree)
	{
		return;
	}
	node * l = tree->left;
	node * r = tree->right;

	node * parent = tree->parent;
	fprintf(stderr, "nodeNumber:%d\n", tree->data);
	fprintf(stderr, "elementID:%ls\n", tree->xmlData.elementID);
	fprintf(stderr, "%ls of %ls\n", tree->member, parent !=NULL ? parent->xmlData.elementID:L"none");
	
	fprintf(stderr, "elementAttr:%ls\n", tree->xmlData.elementAttr);
	fprintf(stderr, "elementData:%ls\n\n", tree->xmlData.elementData);

/*	
* recursively print lower leaves until NULL
*/
	if(l)
	{
		recurse(l);
	}
	if (r)
	{
		recurse(r);
	}
}


node * xmlTreeTop(node *tree)
{

	while(tree->parent!=NULL)
	{
		#ifdef DEBUG
		fprintf(stderr, "xmlTreeTop():going up tree\n");
		#endif
		tree = tree->parent;
	}
	return tree;
}


node * xmlTreeUp(node * tree, int count)
{
	if(count < 0)
		count = 0 - count;
	while(tree->parent!=NULL && count != -1)
	{
		#ifdef DEBUG
		fprintf(stderr, "xmlTreeUp():going up tree\n");
		#endif
		tree = tree->parent;
		count--;
	}
	return tree;
}


node * xmlFindNthParent(node * tree, int i)
{
	if(!tree)
		return NULL;
	node * old = NULL;
	i = (i < 0) ? i : 0 - i;
	while(i < 0)
	{
		while(tree->parent!=NULL)
		{
			#ifdef DEBUG
			fprintf(stderr, "xmlFindNthParent():going up tree%d\n",i);
			#endif
			if(tree->parent)
			{	
				old = tree;
				tree = tree->parent;
				if(tree->left == old)
				{	
					break;
				}
			}
			else break;
		}
		i++;
	}
	return tree;
}


int xmlTreeSaveGraphviz(node * tree, const char * filename, FILE * file, int depth)
{
	int (*recurse)(node * tree, const char * filename, FILE * file, int depth) = xmlTreeSaveGraphviz;
	int i = 0;
	if(depth == 0)
	{
		if(filename)
		{
			if(!file)
				file = fopen(filename, "w");
			if(!file)	return -1;
		}
		fprintf(file, "digraph G {\n");
	}
	
	if(!tree) return -1;
	if(!file) return -1;
	node *l = tree->left;
	node *r = tree->right;
	
/*
* 	recursively print lower leaves until NULL
*/
	if(l)
	{
		fprintf(file,"%d -> %d;\n", tree->data, l->data);
		recurse(l, NULL, file, depth+1);
	}
	else
	{
	    fprintf(file, "null%d [style=\"invis\"];\n", tree->data);
    	fprintf(file, "%d -> null%d;\n", tree->data, tree->data);
	}
	if(r)
	{
		fprintf(file,"%d -> %d;\n", tree->data, r->data);
		recurse(r, NULL, file, depth+1);
	}
	else
	{
	    fprintf(file, "null%d [style=\"invis\"];\n", tree->data);
    	fprintf(file, "%d -> null%d;\n", tree->data, tree->data);
	}

	if(depth == 0)
	{
		tree = xmlTreeTop(tree);
		while((tree = xmlTreeHasNode(xmlTreeTop(tree), i))!= NULL)
		{
			fprintf(file, "%d [shape=polygon,sides=4,label=\"%ls:%d\"];\n", i, tree->xmlData.elementID, i);
			i++;
		}
		fprintf(file, "}\n");
		fflush(file);
		fclose(file);
	}
	
	return 0;
}
