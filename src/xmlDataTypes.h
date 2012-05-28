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
@file xmlDataTypes.h
@author Luke Drummond
@date 28/05/2012 
*/


#include <stdbool.h>
#include <wchar.h>

#ifndef XML_DATATYPES_H
#define XML_DATATYPES_H

/** @brief Substructure of basic node type containing extracted strings of attributes, data, and tag names.
*/
typedef struct
{
	wchar_t * elementID;	//!< the basic name or identifier tag without the angled brackets i.e. 'dimensions'
	wchar_t * elementAttr;	//!< the attributes from the opening tag i.e. 'units="meters"' 
	wchar_t * elementData;	//!< the data between the '10 2 4.5'
}xmlTreeData;

/** @brief The basic node type for building an xml DOM binary tree.
This is the concrete type handled by the all tree utility and databinding functions explicitly.*/ 
typedef struct node
{	///@private
	int data;				//!<  Unique integer reference generated internally.
	///@private
	wchar_t * member;		//!< DEPRECATED
	struct node * left;		//!< Child node. 
	struct node * right;	//!< Sibling node.
	struct node * parent;	//!< Parent node. All trees have a parent excepting the document root.
	xmlTreeData xmlData;	//!< Structure containing all textual data for the relevant xml tag.
}node;

/** @brief Structure containing string beginning and end pointers used by many of the primitive xml string processing functions.
@private
*/
typedef struct
{
	wchar_t * from;
	wchar_t * to;
}range;


/** @brief Union of single primitive datatypes used internally by databinding functions.
@private
*/
typedef union
{
	double doubleVal;
	long longVal;
	bool boolVal;
	const wchar_t * stringVal;
}xmlValueUnion;

/** @brief Struct of primitive datatypes used internally by databinding functions, and possible error flags
@private*/
typedef struct {
	xmlValueUnion value;
	int errNum;
}xmlValue;
#endif


