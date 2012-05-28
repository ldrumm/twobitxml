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
@file
*/

#include "xmlGetDOM.h"
#include "xmlFunctions.h"
#include "treeFunctions.h"
#include "xmlDataTypes.h"


/**@brief Constructs a complete DOM binary tree from the given string.
	This is the high level tree-building function that builds a complete Document Object Model and extracts element IDs, attributes, and data for processing by the real user friendly functions found in xmlGetters.c
	
@param [in] xmlText The wide character string of raw xml data.
@return A complete DOM tree on success, NULL on error.
*/
node * xmlGetDOM(wchar_t * xmlText)
{
	if(!xmlText)
		return NULL;
	int i = 0;
	int count = 0;
	node * xmlTree = xmlTreeAddChildNode(NULL, 0);
	if(!xmlTree)
		return NULL;
	i = xmlCharsToFirstElement(xmlText);
	if(i == -1)
		return NULL;
	xmlText += i;
	
	while(1)
	{
		xmlTree->xmlData.elementID = xmlGetFirstElementIDString(xmlText);
		xmlTree->xmlData.elementAttr = xmlGetFirstElementAttrString(xmlText);
		xmlTree->xmlData.elementData = xmlGetFirstElementDataString(xmlText);
		count++;
		if(xmlCharsToFirstChildElement(xmlText)!= -1)
		{
			xmlTree = xmlTreeAddChildNode(xmlTree, count);
			goto cont;
		}
		i = xmlFirstElementHasSiblings(xmlText);
		if(i == 0)	//error
			break;	
		if(i < 0)
		{
			xmlTree = xmlFindNthParent(xmlTree, i);
			xmlTree = xmlTreeAddSiblingNode(xmlTree, count);
			goto cont;
		}
		else if(i == 1)	//there is an immediate sibling
		{
			xmlTree = xmlTreeAddSiblingNode(xmlTree, count);		
		}
	cont:	
		if(xmlSeekNextElement(&xmlText) == -1)	break;
	}
	return xmlTreeTop(xmlTree);
}


/**
@brief Wrapper to xmlGetDOM allowing to build a DOM tree straight from a relative or complete system path.
@param [in] path System path to a xml text file to open.
@return A complete DOM tree on success, NULL on error.
*/	
node * xmlGetDOMFromPath(const char * path)
{
	
	wchar_t * xmlText = NULL;
	node * tree;
	if(!path) 
		return NULL;
	xmlText = xmlOpenFile(path);
	if(!xmlText) 
		return NULL;
	tree = xmlGetDOM(xmlText);
	free(xmlText);
	return tree;
}

