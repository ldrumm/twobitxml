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
#include "xmlGetters.h"

node * xmlGetNodeFromDotPath(node * tree, const wchar_t * dot_path, ...)
{
//	void * arg;
//	size_t len;
	if(!dot_path)
		return NULL;
	va_list argp;
	int wildCardNumber = 0;
	long tempi;
	wchar_t * tempcp;
	wchar_t text [wcslen(dot_path)+1];
	text[wcslen(dot_path)] = '\0';
	wchar_t * path = &text[0];
	wchar_t * tempptr = NULL;
	wchar_t * tok = NULL;
	wchar_t * p = NULL;
	path = wcsncpy(text, dot_path, wcslen(dot_path));
	
	// tokenizestring and store tokens as list of strings
	// for each token, find wildcard/escape, and increment. call va_arg, with corresponding datatype.
	// find corresponding node
	// continue

	va_start(argp, dot_path);
	while(1)
	{
		tok = wcstok(path, L".", &tempptr);
		if(!tok)
			break;

		path = NULL;
	
		for(p = tok; *p != '\0'; p++) 
		{
			if(*p != '<') 
				continue;
			*p = '\0';
			p++;
			switch(*p)
			{
				case 'd':
				{ 
					tempi = va_arg(argp, long);
					tree = xmlTreeSearchSubElementID(tree, tok);
					if(!tree)
						return NULL;
					
					while(tempi--)
					{
						
						tree = xmlTreeSearchElementSiblingsID(tree->right, tok);
						if(!tree)
							return NULL;	
					}	
				}
				case 'n': 
				{
					tempcp = va_arg(argp, wchar_t *);
					//TODO
				}
				case 'v': 
				{
					tempcp = va_arg(argp, wchar_t *);
					//TODO
				}
				default: break;
			}
		}
		tree = xmlTreeSearchSubElementID(tree, tok);
	}
	va_end(argp);
	return tree;
}


node * __xmlGetNodeFromDotPath(node * tree, const wchar_t * dot_path, ...)
{
	
	va_list argp;
	va_start(argp, dot_path);
	//tree = xmlGetNodeFromDotPath(tree, dot_path, argp);
	va_end(argp);
	return tree;
}
/*
<d = 	int representing Nth matching node on branch:
<n =	wchar_t * representing attribute name
<v =	wchar_t * representing attribute value
<a = 	wchar_t * representing name="value" pair
*/
/* 
tree = getNodeFromDotPath("movie.credits.cast.extra[%d]", 3) will return the 3rd 'extra' listed as a child of movie->credits->cast
i.e.
	movie
	_|__
	x	credits
		_|___________________
	   x	cast			crew etc.
			_|__________________________________________________
		   x	lead	extra(0)	extra(2) other(0) extra (3)	director (0)
														*
string representing attribute id
string representing attribute value
*/


static int _xmlGetAttrCount(node * tree)
{
	return -1;
		
}


static xmlValue _xmlgetAttrValLong(node * tree, const wchar_t * attrID)
{
	xmlValue a;
	a.errNum = -1;
	return a;
}


static xmlValue _xmlgetAttrValDouble(node * tree, const wchar_t * attrID)
{
	xmlValue a;
	a.errNum = -1;
	return a;
}


static xmlValue _xmlgetAttrValString(node * tree, const wchar_t * attrID)
{
	xmlValue a;
	a.errNum = -1;
	return a;
}


static int _xmlGetDataCount(node * tree)
{
	return -1;
}


static xmlValue _xmlGetDataValLong(node * tree, int index)
{
	xmlValue ret;
	ret.value.longVal = 0;
	ret.errNum = -1;
	if(!tree)
		return ret;	
	if(!tree->xmlData.elementData)
		return ret;

	wchar_t data[wcslen(tree->xmlData.elementData) + 1];
	wchar_t * text = data;
	wchar_t * tok = NULL;
	wchar_t * tempptr = NULL;
	wcsncpy(data, tree->xmlData.elementData, wcslen(tree->xmlData.elementData));

	int i = 0;
	while(i <= index)
	{
		tok = wcstok(text, L" ", &tempptr);
		if(!tok)
			break;
		text = NULL;
		i++;
	}
	ret.errNum = 0;
	ret.value.longVal = wcstol(tok, NULL, 10);
	return ret;
}


static xmlValue _xmlGetDataValDouble(node * tree, int index)
{
	
	
	xmlValue ret;
	ret.value.doubleVal = 0.0;
	ret.errNum = -1;
	
	if(!tree)
		return ret;	
	if(!tree->xmlData.elementData)
		return ret;
		
	int i = 0;
	wchar_t data[wcslen(tree->xmlData.elementData) + 1];
	wchar_t * text = data;
	wchar_t * tok = NULL;
	wchar_t * tempptr = NULL;
	
	wcsncpy(data, tree->xmlData.elementData, wcslen(tree->xmlData.elementData));

	
	while(i <= index)
	{
		i++;
		tok = wcstok(text, L" ", &tempptr);
		if(!tok)
			break;
		text = NULL;	
	}
//	if(i != index)
//	{	
//		FIXME
//		printf("couldn't get requested index, not enough values in array%d\n", i);
//		return ret;	// if an out of bounds index was requested, return NaN
//	}
	ret.value.doubleVal = wcstod(tok, NULL);
	ret.errNum = 0;
	return ret;
}


static const wchar_t * _xmlGetDataValString(node * tree)
{
 if(tree)
 	if(tree->xmlData.elementData)
 		return tree->xmlData.elementData;
 return NULL;
}


static xmlValue _xmlGetDataValBool(node * tree, int index)
{
 //http://www.w3.org/TR/xmlschema-2/ :
//"An instance of a datatype that is defined as ·boolean· can have the following legal literals {true, false, 1, 0}."
	xmlValue a;
	a.errNum = -1;
	return a;
}


/**
@return all these functions return a pointer to a newly malloced copy of an array containing the number of elements requested or NULL if not available. the user is responsible for freeing the returned values.
*/


bool * xmlGetDataArrayBool(node * tree, const wchar_t * dot_path, int count)
{
	return NULL;	//TODO
}


double * xmlGetDataArrayDouble(node * tree, const wchar_t * dot_path, int count)
{
	if((!tree)||(!dot_path))
		return NULL;
	int i;
	double * values = NULL;
	xmlValue temp;
	values = malloc(count * sizeof(double));
	if(!values)
		return NULL;
	tree = xmlGetNodeFromDotPath(tree, dot_path);

	for(i = 0; i < count; i ++)
	{
		temp = _xmlGetDataValDouble(tree, i);
		if(temp.errNum != 0) //NaN test for errors
		{
			free(values);
			return NULL;
		}
		values[i] = temp.value.doubleVal;
	}
	return values;
}


long * xmlGetDataArrayLong(node * tree, const wchar_t * dot_path, int count)
{
	if((!tree)||(!dot_path))
		return NULL;
	int i;
	xmlValue temp;
	long * values = NULL;
	values = malloc(count * sizeof(long));
	if(!values)
		return NULL;
	tree = _xmlGetNodeFromDotPath(tree, dot_path);
	for(i = 0; i < count; i ++)
	{
		temp = _xmlGetDataValLong(tree, i);
		if(temp.errNum != 0) //NaN test for errors
		{
			free(values);
			return NULL;
		}
		values[i] = temp.value.longVal;
	}
	return values;
}


wchar_t * xmlGetDataArrayString(node * tree, const wchar_t * dot_path, int count)
{///if count is zero or less the whole String will be copied
	if((!tree)||(!dot_path))
		return NULL;
	int len;
	wchar_t * string = NULL;
	if(tree)
	{
		tree = _xmlGetNodeFromDotPath(tree, dot_path);
		if(tree->xmlData.elementData)
		{	len = (count < 1) ? wcslen(tree->xmlData.elementData) + 1 : count +1;
			string = calloc(len, sizeof(wchar_t));
			if(string)
			{
				string = wcsncpy(string, tree->xmlData.elementData, len);
				string[len-1] = L'\0';
				return string;
			}
		}
	}
	return NULL;
}

int xmlGetAttrCount(node * tree, const wchar_t * dot_path)
{
	return -1;
}


int xmlGetDataCount(node * tree, const wchar_t * dot_path)
{
	return -1;
}



