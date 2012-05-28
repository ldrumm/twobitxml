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
/**
@file treeFunctions.c
@brief contains functions for all the tree related utility functions
@author Luke Drummond
@date Monday 28/05/2012 
*/
#include "treeFunctions.h"



/**\brief Frees all nodes in a given tree and nullifies all references. 
	Can be called on any node of a given subtree, or the root node.
	
@param [in] tree node pointer to the tree or subtree to unlink and free.
*/
void xmlTreeFree(node * tree)
{

	#ifdef DEBUG
	fprintf(stderr, "xmlTreeFree()\n");
	#endif

	void (*recurse)(node *tree) = xmlTreeFree;	//function pointer for recursion

	if(!tree)
		return;
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

/**\brief Adds a new blank child to the given node.
This function will succeed only if the given node has no child. Unlike xmlTreeAddSiblingNode, this function will not traverse the tree to find the first empty child reference.

@param [in] tree The node pointer to which to directly add the child.
@param [in] n Integer reference for counting nodes.
@return The new node, or NULL on failure.
*/
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


/**\brief Adds a new sibling to the given node.
This function can be called with reference to any node, whether siblings exist or not. If the given node already has a sibling, the sibling tree is traversed and the node is added to the end of the branch.

@param [in] tree The node pointer to which to add the sibling.
@param [in] n Integer reference for counting nodes
*/
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


/**\brief Searches given node for first sibling with the called tag(elementID).

@param [in] tree The node pointer to which to directly add the child.
@param [in] elementID The element ID (xml tag) to find.
@return NULL if the tag is not found - otherwise a pointer to the first node found.
*/
node * xmlTreeSearchElementSiblingsID(node * tree, wchar_t * elementID)

{
	node * found = NULL;	// to store pointers returned so they can be tested before returning NULL
	node * (*recurse)(node *tree, wchar_t *elementID) = xmlTreeSearchElementSiblingsID;	// recursive definition of this function
	if(!tree)
		return NULL;

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


/**\brief Searches given node for first children with the given elementID (xml tag).
Searches only 1 level below the given node. i.e. checks given node, then all siblings of first child.

@param [in] tree The node from which to search.
@param [in] elementID String of element ID (xml tag) to find.
@return A pointer to the first node found. NULL if not found.
*/	
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


/**\brief Searches all nodes on a given tree for the requested reference number.

@param [in] tree the node pointer to which to directly add the child.
@param [in] n the integer reference to find
@return a pointer to the node if reference is found, or NULL on failure
*/
node * xmlTreeHasNode(node * tree, int n)

{
	if(!tree)
		return NULL;

	node * found = NULL;									// to store pointers returned so they can be tested before returning NULL
	node * (*recurse)(node * tree, int n) = xmlTreeHasNode;	// recursive definition of this function
	node * l = tree->left;
	node * r = tree->right;

/*see if we've found what we need*/
	if(tree->data == n)
	{
		return tree;
	}
	
/*
* otherwise recursively search lower leaves until it's found or not
* everything will propagate upwards on return. recurse is just a useful alias for this function
*/	if(l)
	{	
		found = recurse(l, n);
		if(found)	
			return found;
	}
	if(r)
	{
		found = recurse(r, n);
	}
	return found;
}


void xmlTreePrint(node * tree)
{
	void (*recurse)(node *tree) = xmlTreePrint;	//recursive definition of this function

	if(!tree)
		return;

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


/**\brief finds the root node of a given tree

@return The root node of the tree. NULL on failure.
*/
node * xmlTreeTop(node *tree)
{
	if(!tree) 
		return NULL;
	while(tree->parent!=NULL)
	{
		#ifdef DEBUG
		fprintf(stderr, "xmlTreeTop():going up tree\n");
		#endif
		tree = tree->parent;
	}
	return tree;
}


/**\brief finds the node n edges closer to the root.

@param [in] tree The node pointer from which to traverse upwards.
@param [in] n The number of edges of any class to traverse towards the root
@return The node n edges closer to root, or root node - Whichever comes first.
*/
node * xmlTreeUp(node * tree, int n)
{
	if(!tree)
		return NULL;
	if(n < 0)
		n = 0 - n;
	while(tree->parent!=NULL && n != -1)
	{
		#ifdef DEBUG
		fprintf(stderr, "xmlTreeUp():going up tree\n");
		#endif
		tree = tree->parent;
		n--;
	}
	return tree;
}


/**\brief finds the nth parent node of a given node.
	This function traverses up the tree and finds the Nth parent-child relationship between given node and root.
	
@param [in] tree The node pointer from which to traverse upwards.	
@param [in] n The number of parent-child relationships to traverse upwards through.
@return The nth parent node of the tree, or root - Whichever comes first.
*/
node * xmlFindNthParent(node * tree, int n)
{
	if(!tree)
		return NULL;
	node * old = NULL;
	n = (n < 0) ? n : 0 - n;
	while(n < 0)
	{
		while(tree->parent!=NULL)
		{
			#ifdef DEBUG
			fprintf(stderr, "xmlFindNthParent():going up tree%d\n",n);
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
		n++;
	}
	return tree;
}


/**\brief Saves a textfile of an xml DOM in graphviz dot format.
	Outputs a directed graph ("digraph") of the given tree suitable for further processing with GNU dot, neato or similar.  Child nodes are displayed on the left, siblings on right. Tree nodes are labelled with the elementID.

@param [in] tree The node pointer from which to save directed graph
@param [in] filename The full or relative path to which to save the output file.  May be null if file is non NULL.
@param [out] file File pointer to which to save the Graphviz file. Optionally may be NULL if filepath is non-NULL.
@param [in] depth  Integer reference for internal use.  Users should call this function from the macro.
@return 0 on success. -1 on error.
*/
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
