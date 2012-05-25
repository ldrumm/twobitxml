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
#include "xmlFunctions.h"
#include "treeFunctions.h"
#include "xmlDataTypes.h"

node * xmlGetDOM(wchar_t * xmlData)
{
	if(!xmlData)
		return NULL;
	int i = 0;
	int count = 0;
	node * xmlTree = xmlTreeAddChildNode(NULL, 0);
	if(!xmlTree)
		return NULL;
	i = xmlCharsToFirstElement(xmlData);
	if(i == -1)
		return NULL;
	xmlData += i;
	
	while(1)
	{
		xmlTree->xmlData.elementID = xmlGetFirstElementIDString(xmlData);
		xmlTree->xmlData.elementAttr = xmlGetFirstElementAttrString(xmlData);
		xmlTree->xmlData.elementData = xmlGetFirstElementDataString(xmlData);
		count++;
		if(xmlCharsToFirstChildElement(xmlData)!= -1)
		{
			xmlTree = xmlTreeAddChildNode(xmlTree, count);
			goto cont;
		}
		i = xmlFirstElementHasSiblings(xmlData);
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
		if(xmlSeekNextElement(&xmlData) == -1)	break;
	}
	return xmlTreeTop(xmlTree);
}
	

