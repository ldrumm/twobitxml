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
#define _GNU_SOURCE //for strcasestr
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <iconv.h>
#include <errno.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>

#include "xmlDataTypes.h"

#ifndef XML_FUNCTIONS_H
#define XML_FUNCTIONS_H
wchar_t * xmlStripHeader(wchar_t * xmlData);
wchar_t * xmlConvertEncoding(char * xmlData, int len);
wchar_t * xmlOpenFile(const char * filePath);
wchar_t * xmlDelComment(wchar_t * xmlData);

wchar_t * xmlGetStringFromRange(range temp);

int xmlCharsToFirstElement(wchar_t * string);
int xmlCharsToNextElement(wchar_t * string);

range xmlFirstElementIDRange(wchar_t * string);
range xmlFirstElementAllRange(wchar_t * string);
range xmlFirstElementAttrRange(wchar_t * string);
range xmlFirstElementDataRange(wchar_t * string);

wchar_t * xmlGetFirstElementIDString(wchar_t * string);
wchar_t * xmlGetFirstElementAllString(wchar_t * string);
wchar_t * xmlGetFirstElementAttrString(wchar_t * string);
wchar_t * xmlGetFirstElementDataString(wchar_t * string);

wchar_t * xmlGetNamedElementAll( wchar_t * string,  wchar_t * elementID);
wchar_t * xmlGetNamedElementData( wchar_t * string,  wchar_t * elementID);
wchar_t * xmlGetNamedElementAttrString(wchar_t * string,  wchar_t * elementID);

int xmlFirstElementAllLen(wchar_t * string);
int xmlCharsToFirstChildElement(wchar_t * string);
// returns the number of characters to the first child element of first element found after the given pointer.
// returns -1 if no child element found.

int xmlSeekNextElement(wchar_t ** fp);
//moves the given pointer to the beginning of the next tag.
//returns number of characters moved, or -1 on error (not found)

int xmlFirstElementHasSiblings(wchar_t * string);
/// given a pointer to the beginning of an xml tag, this function returns 1 if the firstmost tag found has an immediate sibling.
/// a negative number if not.  negative numbers represent the number of closing tags found between the first located element end, and the second element start.  this is how far back up the tree to climb. 
/// unlike most search functions, returns zero on error, 1 if the element has an immediate sibling.
#endif

