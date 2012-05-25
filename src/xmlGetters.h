/*xmlGetters.h*/

#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>
#include "xmlDataTypes.h"
#include "treeFunctions.h"

#ifndef XML_GETTERS_H
#define XML_GETTERS_H
//node * __xmlGetNodeFromDotPath(node * tree, const wchar_t * dot_path, va_list argp);
node * _xmlGetNodeFromDotPath(node * tree, const wchar_t * dot_path, ...);
static int _xmlGetAttrCount(node * tree);
static xmlValue _xmlgetAttrValLong(node * tree, const wchar_t * attrID);
static xmlValue _xmlgetAttrValDouble(node * tree, const wchar_t * attrID);
static xmlValue _xmlgetAttrValString(node * tree, const wchar_t * attrID);

static int _xmlGetDataCount(node * tree);
static xmlValue _xmlGetDataValLong(node * tree, int index);
static xmlValue _xmlGetDataValDouble(node * tree, int index);
static const wchar_t * _xmlGetDataValString(node * tree);
static xmlValue _xmlGetDataValBool(node * tree, int index);

/**
@return all these functions return a pointer to a newly malloced copy of an array containing the number of elements requested or NULL if not available the user is responsible for freeing the returned values.*/
int xmlGetAttrCount(node * tree, const wchar_t * dot_path);
int xmlGetDataCount(node * tree, const wchar_t * dot_path);
bool * xmlGetDataArrayBool(node * tree, const wchar_t * dot_path, int count);
double * xmlGetDataArrayDouble(node * tree, const wchar_t * dot_path, int count);
long * xmlGetDataArrayLong(node * tree, const wchar_t * dot_path, int count);
wchar_t * xmlGetDataArrayString(node * tree, const wchar_t * dot_path, int count);
#endif
