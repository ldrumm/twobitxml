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
/*treeDataTypes.h*/
#include <stdbool.h>
#include <wchar.h>

#ifndef XML_DATATYPES_H
#define XML_DATATYPES_H
typedef struct
{
	wchar_t * elementID;	// the basic name or identifier tag without the angled brackets i.e. COLLADA
	wchar_t * elementAttr;	// the attributes from the opening tag i.e. <tag units="meters" name='Luke "Drumgay" Drummond"> 
	wchar_t * elementData;	// the data between the <tag>10 2 0 4 7</tag>
}xmlTreeData;

/* the basic treeNode type
this is the concrete type 
handled by the treeFunctions explicitly*/ 
typedef struct node
{
	int data;
	wchar_t * member;
	struct node * left;
	struct node * right;
	struct node * parent;
	xmlTreeData xmlData;	// structure containing refs to elementID, elementAttr
}node;

typedef struct
{
	wchar_t * from;
	wchar_t * to;
}range;
	
typedef union
{
	double doubleVal;
	long longVal;
	bool boolVal;
	const wchar_t * stringVal;
}xmlValueUnion;

typedef struct {
	xmlValueUnion value;
	int errNum;
}xmlValue;
#endif


