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
	

