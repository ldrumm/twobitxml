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
/*xmlGetters.h*/

#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>
#include "xmlDataTypes.h"
#include "treeFunctions.h"
#include "xmlError.h"
#ifndef XML_GETTERS_H
#define XML_GETTERS_H


static xmlValue _xmlgetAttrValLong(node * tree, const wchar_t * attrID);
static xmlValue _xmlgetAttrValDouble(node * tree, const wchar_t * attrID);
static xmlValue _xmlgetAttrValString(node * tree, const wchar_t * attrID);

static int _xmlGetDataCount(node * tree);
static xmlValue _xmlGetDataValLong(node * tree, int index);
static xmlValue _xmlGetDataValDouble(node * tree, int index);
static const wchar_t * _xmlGetDataValString(node * tree);
static xmlValue _xmlGetDataValBool(node * tree, int index);

node * xmlGetNodeFromDotPath(node * tree, const wchar_t * dot_path, ...);
/**
@return all these functions return a pointer to a newly malloced copy of an array containing the number of elements requested or NULL if not available the user is responsible for freeing the returned values.*/
int xmlGetAttrCount(node * tree);
int xmlGetDataCount(node * tree, int datatype);
bool * xmlGetDataArrayBool(node * tree, int count);
double * xmlGetDataArrayDouble(node * tree, int count);
long * xmlGetDataArrayLong(node * tree, int count);
wchar_t * xmlGetDataArrayString(node * tree, int count);
char * xmlGetDataArrayStringUTF8(node * tree, int count);
#endif
