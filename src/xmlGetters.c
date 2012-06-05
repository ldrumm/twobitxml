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
*//**@file*/

#include "xmlGetters.h"


/**@brief Search function that uses Javascript like dotted heirarchy for notating the xml node.
	This function uses a similar syntax to the familar printf family of routines. allowing access to xml data by index, and attribute
@param [in] tree Tree node to search from
@param [in] dot_path The dotted path indicating heirarchy of the data.
@param [in] ... optional list of variables as indicated by format specifiers in the dotpath.
@return The node requested or NULL on failure.*/
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

/**<
@usage
<d = 	int representing Nth matching node on branch:
<n =	wchar_t * representing attribute name
<v =	wchar_t * representing attribute value
<a = 	wchar_t * representing name="value" pair
 
tree = getNodeFromDotPath("movie.credits.cast.extra<d", 3) will return the 3rd 'extra' listed as a child of movie->credits->cast
i.e.
\verbatim
	[movie]
    _|__
   x    [credits]
       _|____________________________
       x    [cast]			[crew etc].
            |____________________________________________________V_____________________
            x    [lead]  [extra(0)]  [extra(2)]  [other(0)]  [extra (3)]  [director (0)]
\endverbatim*/


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


static xmlValue _xmlGetDataValLong(const wchar_t * string, int index)
{
	xmlValue ret;
	ret.value.longVal = 0;
	ret.errNum = -1;
	if(!string)
		return ret;	
	//if(!tree->xmlData.elementData)
	//	return ret;

	//wchar_t data[wcslen(tree->xmlData.elementData) + 1];
	//wchar_t * text = data;
	wchar_t * tok = NULL;
	//wchar_t * tempptr = NULL;
	//wcsncpy(data, tree->xmlData.elementData, wcslen(tree->xmlData.elementData));

	int i = 0;
	while(i <= index)
	{
		i++;
		tok = wcschr(string, L' ');
		
		if(!tok)
			break;
			
		string = tok+1;
	}
	ret.errNum = 0;
	ret.value.longVal = wcstol(tok, NULL, 10);
	return ret;
}


static xmlValue _xmlGetDataValDouble(const wchar_t * string, int index)
{	//wcstok destructively modifies the string given as its first argument.  Therefore, a copy needs to be made.
	// This is extremely costly in terms of performance, and needs to be streamlined // TODO
	xmlValue ret;
	ret.value.doubleVal = 0.0;
	ret.errNum = -1;
	
	if(!string)
		return ret;
		
	int i = 0;
	//wchar_t data[wcslen(tree->xmlData.elementData) + 1];
	//wchar_t * text = data;
	wchar_t * tok = NULL;
	//wchar_t * tempptr = NULL;
	
	//wcsncpy(data, tree->xmlData.elementData, wcslen(tree->xmlData.elementData));

	while(i <= index)
	{
		
		i++;
		tok = wcschr(string, L' ');
		
		if(!tok)
			break;
			
		string = tok+1;
	}
//	if(i != index)
//	{	
//		FIXME
//		_xmlSetError("couldn't get requested index, not enough values in array%d\n", i);
//		return ret;	// if an out of bounds index was requested, return NaN
//	}
	ret.value.doubleVal = wcstod(string, NULL);
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


static xmlValue _xmlGetDataValBool(node * tree, int index)//TODO
{
 //http://www.w3.org/TR/xmlschema-2/ :
//"An instance of a datatype that is defined as ·boolean· can have the following legal literals {true, false, 1, 0}."
	xmlValue a;
	a.errNum = -1;
	return a;
}


bool * xmlGetDataArrayBool(node * tree, int count)//TODO
{
	return NULL;
}


double * xmlGetDataArrayDouble(node * tree, int count)
{
	if(!tree)
		return NULL;
	int i;
	double * values = NULL;
	xmlValue temp;
//	if(count == 0)											//FIXME
//		count = xmlGetDataCount(tree, XML_DATATYPE_LONG);	//FIXME
	values = malloc(count * sizeof(double));
	if(!values)
		return NULL;

	for(i = 0; i < count; i ++)
	{
		temp = _xmlGetDataValDouble(tree->xmlData.elementData, i);
		if(temp.errNum != 0) //NaN test for errors
		{
			free(values);
			return NULL;
		}
		values[i] = temp.value.doubleVal;
	}
	return values;
}


long * xmlGetDataArrayLong(node * tree, int count)
{
	if(!tree)
		return NULL;
	int i;
	xmlValue temp;
	long * values = NULL;
//	if(count == 0)											//FIXME
//		count = xmlGetDataCount(tree, XML_DATATYPE_LONG);	//FIXME

	values = malloc(count * sizeof(long));
	if(!values)
		return NULL;

	for(i = 0; i < count; i ++)
	{
		temp = _xmlGetDataValLong(tree->xmlData.elementData, i);
		if(temp.errNum != 0) //NaN test for errors
		{
			free(values);
			return NULL;
		}
		values[i] = temp.value.longVal;
	}
	return values;
}


wchar_t * xmlGetDataArrayString(node * tree, int count)
{///if count is zero or less the whole string will be copied
	if(!tree)
		return NULL;
	int len;
	wchar_t * string = NULL;
	if(tree)
	{
		if(tree->xmlData.elementData)
		{	len = (count < 1) ? wcslen(tree->xmlData.elementData) + 1 : count + 1;
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


char * xmlGetDataArrayStringUTF8(node * tree, int count)
{
	return NULL;

}


int xmlGetAttrCount(node * tree)
{
	if(!tree)
		return -1;
	int count = 0;
	wchar_t delim = L'\0';
	wchar_t * string = tree->xmlData.elementAttr;

	while(1)
	{
		string = wcschr(string, L'=');
		if(!string)
			break;
		if(string[1] == L'\"')
			delim =  L'\"';
		else if(string[1] == L'\'')
		{
			delim =  L'\'';
		}
		else
		{
			_xmlSetError("malformed attribute string: %ls", tree->xmlData.elementAttr);
			return -1;
		}
		
		string++;
		while(1)
		{
			string++;
			if(*string == L'\0')
			{
				_xmlSetError("malformed attribute string: %ls", tree->xmlData.elementAttr);
				return -1;
			}
			if(*string == delim)
			{
				count++;
				break;
			}			
		}
		string++;
	}
	return count;
}


int xmlGetDataCount(node * tree, int datatype)
{
	return -1;
}



