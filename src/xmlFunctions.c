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


wchar_t * xmlStripHeader(wchar_t * xmlData)
{
	wchar_t * temp = NULL;
	if(!xmlData)
		return NULL;
	
	temp = wcsstr(xmlData, L"?>");
	if(!temp)
	{
		fprintf(stderr, "xmlStripHeader():no header found - skipping");
		return xmlData;
	}
	else return temp + 3;
}


wchar_t  * xmlConvertEncoding(char * xmlData, int len)
{
	int err = 0;
	int i = 0;
	size_t inbytesleft = 0;
	size_t outbytesleft = 0;
	int convertedChars = 0;
	wchar_t * out = NULL;
	char * outbytestream = NULL;
	iconv_t cd = NULL;
	
	if(!xmlData)
		return NULL;
	len = strlen(xmlData);
	out = calloc(len, sizeof(wchar_t));
	if(!out)
		return NULL;
	
	outbytestream = (char *) out;
	while (isspace((int)xmlData[i]))	
		i++; //	skip whitespace

	inbytesleft = strlen(xmlData);
	outbytesleft = inbytesleft * sizeof(wchar_t);
	if((strcasestr(xmlData+i, "encoding=\"UTF-8\"")) != NULL)
	{
		cd = iconv_open("WCHAR_T", "UTF-8");
		if(cd == (iconv_t) -1)
		{
			fprintf(stderr, "iconv_open() failed\n");
			free(out);
			return NULL;
		}
	}
	else if((*(uint16_t *) xmlData == 0xfffe) || ((*(uint16_t *) xmlData == 0xfeff)))
	{
		cd = iconv_open("WCHAR_T", "UTF-16");
		if(cd == (iconv_t) -1)
		{
			fprintf(stderr, "iconv_open() failed\n");
			free(out);
			return NULL;
		}		
	}
	else{	//presume utf-8
		fprintf(stderr, "xmlConvertEncoding(): WARNING: no encoding found, presuming utf-8\n");
		cd = iconv_open("WCHAR_T", "UTF-8");
		if(cd == (iconv_t) -1)
		{
			fprintf(stderr, "iconv_open() failed\n");
			free(out);
			return NULL;
		}
	}

	iconv (cd, NULL, NULL, &outbytestream, (size_t *)&outbytesleft);
	err = errno;
	if(err != 0)
		printf("conversion error\n");
	
	convertedChars = iconv (cd, &xmlData, (size_t *)&inbytesleft, &outbytestream, (size_t *)&outbytesleft);
	if(convertedChars == -1)
	{
		err = errno;
		if (err == EINVAL)
		{
			printf("conversion not available\n");
			free(out);
			return NULL;
		}
	}
	//printf("convertedChars:%d\n", convertedChars);
	// resize the buffer and nul terminate
	//if(convertedChars < len)
	//	out = realloc(out, (convertedChars + 1) * sizeof(wchar_t));
	if(out)
	{
	;//	out[convertedChars] = L'\0';
	}
	//printf("%ls\n", out);
	iconv_close(cd);
	return out;
}


wchar_t * xmlOpenFile(const char * filePath)
{
	/*open file*/
	FILE * xmlFile = NULL;
	char * xmlData = NULL;
	wchar_t * wXmlData = NULL;
	size_t fSize = 0;
	
	if(!filePath)
		return NULL;
		
	xmlFile = fopen(filePath,"rb");
	if( xmlFile == NULL)
	{
		fprintf(stderr, "file open fail\n");
		return NULL;
	}

/* allocate storage */	
	(void)fseek(xmlFile, 0, SEEK_END); 
	fSize = ftell(xmlFile);
	xmlData = malloc((fSize +1)* sizeof(char));
	if(xmlData == NULL)
	{
		fprintf(stderr, "malloc fail\n");
		return NULL;
	}

/* copy file to memory */
	(void) rewind(xmlFile);
	if (fread(xmlData,(sizeof(char)), fSize, xmlFile)!=fSize)
	{
		fprintf(stderr, "file copy failure\n");
		free(xmlData);
		fclose(xmlFile);
		return NULL;
	}
	
	xmlData[fSize] = '\0';	//NULL terminate

	wXmlData = xmlConvertEncoding(xmlData, fSize);
	free(xmlData);
	fclose(xmlFile);
	return wXmlData;
}


/* 	
xmlDelComment strips comments from a given textfile and returns a malloced pointer to the processed string
change the value of comment fields below to repurpose
this routine for any language style of comment e.g c++:
openComment = "//"
closeComment = "\n"
function returns a different pointer from the one with which it was called after freeing the original.*/
wchar_t * xmlDelComment(wchar_t * xmlData)
{
	#ifdef DEBUG
	printf("xmlDelComment()\n");
	#endif
	wchar_t openComment[] = L"<!--";
	wchar_t closeComment[] = L"-->";
	int openLen = 4;
	int closeLen = 4;
	int i = 0;	//wchar_ts read
	int j = 0;	//wchar_ts removed/memory to realloc
	int dataLen = wcslen(xmlData) + 1;
	wchar_t * buffer = NULL;
	
	buffer = (wchar_t *)malloc((dataLen * sizeof(wchar_t)) + 1);
	if (!buffer)	
	{	
		#ifdef DEBUG
		fprintf(stderr, "xmlDelComment():alloc fail\n");
		#endif
		return NULL;
	}
	
	for(i = 0; i < dataLen; i++)	{
		if(xmlData[i]== L'<'){	// the first wchar_t of opencomment is tested so that wcsncmp is not called every byte
			if ((wcsncmp (openComment, xmlData+i, (size_t)openLen)) == 0)	{
				while(1)	{
					j++;
					i++;
					if(xmlData[i] == L'>'){
						break;
					}
				}
			}	
		}
		buffer[i-j] = xmlData[i];
	}

	#ifdef DEBUG
	fprintf(stderr, "xmlDelComment:\tremoved %d bytes\n",j); 
	#endif
	if (j > 0)
	{
		buffer = realloc(buffer , (dataLen - j) * (sizeof(wchar_t)) + 1);
		buffer[(i - j) + 1] = L'\0';
	}
	
	free(xmlData);
	xmlData = NULL;

	return buffer;
}


wchar_t * xmlGetStringFromRange(range temp)
{
	wchar_t * string = NULL;
	int len = 0;

	if((!temp.from) || (!temp.to))
		return NULL;
	len = temp.to - temp.from;
	string = calloc(len  + 1, sizeof(wchar_t));
	if(!string)
		return NULL;
	if(len < 0)
	{	
		#ifdef DEBUG
		fprintf(stderr, "xmlGetStringFromRange():invalid range\n");
		#endif
		return NULL;
	}
	wcsncpy(string, temp.from, len);
	string[len] = L'\0';
	return string;
}


int xmlCharsToFirstElement(wchar_t * string)		// < the VERY beginning of the tag - this is THE fundamental function
{
// returns the number of characters to point the string to the very beginning of the next opening tag
	
	#ifdef DEBUGFUNCTIONS
	printf("xmlCharsToFirstElement()\n");
	#endif
	if(!string)
		return -1;
	wchar_t * start = string;	
/*
*locate the next appearance of '<' and ensure that it is not the beginning of a closing tag by checking for the '/' character
*/	
	while(1)
	{	
		
		start = wcsstr(start, L"<");	//find '<' character
		if(!start)
		{
			#ifdef DEBUG
			fprintf (stderr, "xmlCharsToFirstElement(): no next element found\n");
			#endif
			return -1;
		}
		else if (start[1] == L'/')	//make sure location isn't the beginning of an ending tag
		{
			start++;
			continue;
		}
		else if (*(start + 1) == L'!')
		{
			if(wcsncmp(start + 1, L"![CDATA[", 8) == 0)
			{
				start = wcsstr(start, L"[[>");
				if(!start)
					return -1;
				#ifdef DEBUG
				fprintf(stderr, "xmlCharsToFirstElement(): skipping 'CDATA' section\n");
				#endif
				start++;
				continue;
			}
			else
			{
				// !DOCTYPE etc - currently unhandled
				#ifdef DEBUG
				printf("xmlCharsToFirstElement():skipping unhandled identifier:%lc%lc%lc%lc%lc%lc%lc%lc...\n",\
					start[1],start[2],start[3],start[4],start[5],start[6],start[7], start[8]);
				#endif
				start = wcsstr(start, L">");
				if(start)continue;
				else return -1;
			}
		}
		else if (*(start + 1) == L'?')
		{
			if(wcsncmp(start + 1, L"?xml", 4) == 0)
			{
				start = wcsstr(start, L"?>");
				if(!start)
					return -1;
				#ifdef DEBUG
				fprintf(stderr, "xmlCharsToFirstElement(): skipping '?xml' section\n");
				#endif
				start++;
				continue;
			}
			else
			{
				// - currently unhandled
				#ifdef DEBUG
				printf("xmlCharsToNextElement():skipping unhandled identifier:%lc%lc%lc%lc%lc%lc%lc%lc...\n",\
					start[1],start[2],start[3],start[4],start[5],start[6],start[7], start[8]);
				#endif
				start = wcsstr(start, L">");
				if(start)continue;
				else return -1;
			}
		}
		else break;
	}
	
	return start - string;
}


int xmlCharsToNextElement(wchar_t * string)
{	//skips the first element if it's already being pointed to and return the number of chars to the next available element
	// returns -1 if not found.
	int i = 0;
	if(!string)
		return -1;
	i = xmlCharsToFirstElement(string);
	if(i == -1)
	{
		return -1;
	}
	string+=i;
	string++;
	return xmlCharsToFirstElement(string) + 1;
}


range xmlFirstElementIDRange(wchar_t * string)
{
	
	int i = 0;
	wchar_t * start  = NULL;
	range ret = 
	{
		.from = NULL,
		.to = NULL
	};
	
	if(!string)
		return ret;
		
	i = xmlCharsToFirstElement(string);
	if(i == -1)
		return ret;
		
	start = string + i;
	
	/* move to start of text */
	start++;

/* find the end of the elementID text*/
/*FIXME: this does not catch dodgy XML names*/
/*from http://www.w3schools.com/XML/xml_elements.asp*/
/*XML elements must follow these naming rules:*/
/*Names can contain letters, numbers, and other characters*/
/*Names cannot start with a number or punctuation character*/
/*Names cannot start with the letters xml (or XML, or Xml, etc)*/
/*Names cannot contain spaces*/
/*Any name can be used, no words are reserved.*/

	/*catch the above cases*/
	if(iswalpha(*start) == 0)
	{
	#ifdef DEBUG
		fprintf(stderr, \
		"firstElementIDRange(): malformed xml tag - starts with number, punctuation or some other showstopper.  \
		Please check document syntax:\
		\"%ls\"\n", start);
	#endif
		return ret;
	}

	/*skip whitespace*/
	i = 0;
	while(iswspace((wint_t) * (start+i)))
	{
		++i;
	}
	start += i;

/* get bounds of the element name */
/* TODO: optimize - this loop is consuming 27% of execution time */
	while(1)
	{
		i++;
		if(iswspace((wint_t)start[i])!= 0 || (wcsncmp(start+i, L">", 1) == 0))
		{
			break;
		}
	}

/*
strip out the slash if it's a closing tag in dual role.  In some cases
we could probably get away with passing the slash, but it's best not to do that. sheesh
don't be dick
*/	
	if(wcsncmp(start + i - 1, L"/", 1) == 0)
	{
		i--;
	}

	ret.from = start;
	ret.to = start+i;
	return ret;
}


wchar_t * xmlGetFirstElementIDString(wchar_t * string)
{
	if(!string)
		return NULL;	
	return xmlGetStringFromRange(xmlFirstElementIDRange(string));
}


range xmlFirstElementAllRange(wchar_t * string) 	// contains "<elID attr="1.0">to</elID>"
{
	wchar_t * openStart = NULL;		// first character of the xml Element including '<'
	wchar_t * openEnd = NULL;		// last character of the opening elementID: '>' 
	wchar_t * closeStart = NULL;	// first character of the closing tag: '<'
	wchar_t * closeEnd = NULL;		// last character of the closing tag '<'
	wchar_t * closing = NULL;		// final character including NULL termination wchar_t
	wchar_t * elementID = NULL;
	int openIsClose = 0;			// control flow boolean for the event that a tag is single ended i.e. <element/>

	int i = 0;						// counter
	int j = 0;						// counter
	range ret = 
	{
		.from = NULL,
		.to = NULL
	};
	#ifdef DEBUGFUNCTIONS
	printf("firstElementAllRange()\n");
	#endif
	if(!string)
		return ret;
	
	
/* get opening tag start location and elementID*/
	i = xmlCharsToFirstElement(string);
	if(i == -1)
		return ret;
	openStart = string + i;
	elementID = xmlGetFirstElementIDString(string);
	if(!elementID)
		return ret;

	if(*(openStart)!= L'<')
	{
	//	printf("firstElementAllRange():TAG has no start bracket:%c\n", (openStart));
		return ret;
	}

/* get opening tag end location */
	while(*(openStart + i) != '>' )
	{
		++i;
	}
	openEnd = openStart+i;

/*check to see if there is a separate endtag*/
	if(*(openEnd - 1) == L'/')
	{
		openIsClose = 1;
		openEnd += 1;	// the above loop exits at the end of the elementID before the '>' make sure this is not the case got
	}

/* generate closing tag text */
	if(openIsClose == 0)
	{
		i = wcslen(elementID);
		
	/*generate closing tag string by adding the elementID between the characters: </>	*/
		closing = calloc((i + 4) , sizeof(wchar_t));	//FREEME
		if(closing==NULL)
		{
			fprintf(stderr, "firstElementAllRange(): alloc fail - couldn't assign memory\n");
			free(closing);
			return ret;
		}
		*(closing+0) = L'<';
		*(closing+1) = L'/';
		for(j = 0; j < i; j++)
		{
			*(closing + j + 2) = elementID[j];
			//printf("%lc\n", elementID[j]);
		}
		*(closing+j+2)= L'>';					//FIXME: invalid write according to valgrind
		*(closing+j+3)= L'\0';
		//printf("%ls\n", closing);
		closeStart = wcsstr(string, closing);	//FIXME: invalid read according to valgrind		
		if(closeStart == NULL)
		{
			fprintf(stderr, "firstElementAllRange(): malformed tag in xmlFile -  no closing tag\n");
			free(closing);
			free(elementID);
			return ret;
		}
		closeEnd = closeStart + j + 3;
	}
	else if(openIsClose == 1)
	{
		closeStart = openStart;
		closeEnd = openEnd;
	}
	else	//unhandled state. fail gracefully
	{
		free(closing);
		free(elementID);
		return ret;
	}
	ret.from = openStart;
	ret.to = closeEnd;
	free(elementID);
	free(closing);
	return ret;
}


wchar_t * xmlGetFirstElementAllString(wchar_t * string)
{
	if(!string)
		return NULL;	
	return xmlGetStringFromRange(xmlFirstElementAllRange(string));
}


range xmlFirstElementAttrRange(wchar_t * string)	//: contains "attr=1.0"
{
	#ifdef DEBUGFUNCTIONS
	printf("firstElementAttrRange()\n");
	#endif
	wchar_t * start = NULL;				// first character of the xml Element including '<'
	wchar_t * end = NULL;
	int i = 0;
	range ret =	
	{
		.from = NULL,
		.to = NULL
	};
	
	if(!string)
		return ret;

/* get opening tag start location */
	i = xmlCharsToFirstElement(string);
	if(i == -1)
		return ret;

	start = string + i;
	
	/* get opening tag end location */
	i = 0;
	while(start[i] != L'>' && start[i] != L'\0')
	{
		++i;
	}
	if(start[i] == L'\0')
	{
		fprintf(stderr, "firstElementAttrRange():unexpected end of document. please check syntax\n");
		return ret;
	}
	if(start[i - 1] == L'/')
	{
		i--;
	}
	end = start+i;
	if(*(start)== L'<')	//move to the end of elementID
	{
		while((iswspace((wint_t)*start) == 0) && start < end)
		{
			start++;
		}
		//	start++;
	}

	else
	{
		return ret;
	}

	if(end - start == 0)
		return ret;
	ret.from = start+1;
	ret.to = end;
	return ret;
}


wchar_t * xmlGetFirstElementAttrString(wchar_t * string)
{
	if(!string)
		return NULL;	
	return xmlGetStringFromRange(xmlFirstElementAttrRange(string));
}


range xmlFirstElementDataRange(wchar_t * string)
{
	range ret = {NULL,NULL};
	range id = {NULL,NULL};
	
	wchar_t *openStart = NULL;			// first character of the xml Element including '<'
	wchar_t *openEnd = NULL;			// last character of the opening elementID: '>' 
	wchar_t *closeStart = NULL;			// first character of the closing tag: '<'
//	wchar_t *closeEnd = NULL;			// last character of the closing tag '<'
	wchar_t * closing = NULL;			// final character including NULL termination wchar_t
	
	int openIsClose = 0;				// control flow boolean for the event that a tag is single ended i.e. <element/>
	int i = 0;							// counter
	int j = 0;							// counter
	int len;							// length of the xmlElement including NULL termination

	#ifdef DEBUGFUNCTIONS
	printf("xmlFirstElementDataRange()\n");
	#endif

/* get opening tag start location */
	i = xmlCharsToFirstElement(string);
	if(i == -1)
		return ret;
	id = xmlFirstElementIDRange(string);
	if((!id.from) || (!id.to))
		return ret;
	if((id.from) > (id.to))
	{
		printf("invalid range\n");
	}
	openStart = string + i;
	if(*(openStart) != L'<')
	{	
		printf("given bad tag start%lc\n", *openStart);
		return ret;
	}
/* get opening tag end location */
	i = 0;
	while(*(openStart+i) != L'>' )
	{
		++i;
	}
	openEnd=openStart+i;

/*check to see if there is a separate endtag*/
	if(*(openEnd - 1)== L'/')
	{
		openIsClose = 1;
	}

/* generate closing tag text */
	if(openIsClose == 0)
	{

	/*generate closing tag string by adding the elementID to the characters </>	*/
		closing = calloc(((id.to -id.from) + 4), sizeof(wchar_t));	//FREEME
		if(closing==NULL)
		{
			fprintf(stderr, "xmlFirstElementDataRange(): alloc fail - couldn't assign memory\n");
			return ret;
		}
		*(closing + 0) = L'<';
		*(closing + 1) = L'/';
		
		i = id.to - id.from;
		for(j = 0; j <= i; j++)
		{
			closing  [j + 2] = id.from[j];
		}
		*(closing + j + 1) = L'>';				/*FIXME: invalid write according to valgrind*/	
		*(closing + j + 2) = L'\0';
		closeStart = wcsstr(string, closing);	/*FIXME: invalid read according to valgrind*/		
		if(closeStart == NULL)
		{
			#ifdef DEBUG
			fprintf(stderr, "xmlFirstElementDataRange(): malformed tag in xmlFile - no closing tag\n");
			#endif
			free(closing);
			return ret;
		}
	//	closeEnd = closeStart + j + 4;
	}
	else if(openIsClose == 1)
	{
		closeStart = openStart;
	//	closeEnd = openEnd;
	}
	else
	{
		free(closing);
		return ret;
	}
	len = closeStart - openEnd;

/*get everything between > & < as long as there is no other nested element or other tag character*/
	for( i = 0; i < len; i++)
	{
		if(*(openEnd + i) == L'<')
		{
			free(closing);
			return ret;
		}
	}
	//assert(len >= 0);
	if(len < 0)
	{
		free(closing);
		return ret;
	}
	openEnd += 1;	//skip the last character of the opening tag


/* print the relevant section of the string */
	#ifdef DEBUG
	fprintf(stderr, "xmlFirstElementDataRange():%ls\n", xmlElementData);
	#endif
	
	ret.from = openEnd;
	ret.to = openEnd + len -1;
	free(closing);
	return ret;

}


wchar_t * xmlGetFirstElementDataString(wchar_t * string)
{
	if(!string)
		return NULL;	
	return xmlGetStringFromRange(xmlFirstElementDataRange(string));
}


wchar_t * xmlGetNamedElementAll( wchar_t * string,  wchar_t * elementID)
{
	return NULL;
}


wchar_t * xmlGetNamedElementData( wchar_t * string,  wchar_t * elementID)
{
	return NULL;
}


wchar_t * xmlGetNamedElementAttrString(wchar_t * string,  wchar_t * elementID)
{
	return NULL;
}


int xmlFirstElementAllLen(wchar_t * string)
{
	if(!string)
		return -1;
	range temp = xmlFirstElementAllRange(string);
	if((!temp.to)||(!temp.from))
		return -1;
	return temp.to - temp.from;
}


int xmlCharsToFirstChildElement(wchar_t * string)
{	// returns the number of characters to the first child element of first element found after the given pointer.
	// returns -1 if no child element found.
	wchar_t *temp;
	int i = 0;
	range elRange;
	elRange.from = NULL;
	elRange.to = NULL;
	elRange = xmlFirstElementAllRange(string);
	if((!elRange.from)||(!elRange.to))
		return -1;
	
	temp = xmlGetStringFromRange(elRange);
	if(!temp)
		return -1;
	i = xmlCharsToFirstElement(temp+1);
	free(temp);
	return (i >=0)? i+1+elRange.from - string: i;
}


int xmlSeekNextElement(wchar_t ** fp)
{	//moves the given pointer to the beginning of the next tag.
	//returns number of characters moved, or -1 on error (not found)
	int i = 0;
	if(!fp)
		return -1;
	if(!*fp)
		return -1;
	//*fp++;
	i = xmlCharsToNextElement(*fp);
	if(i == -1)
		return i;
	*fp += i;
	return i;
}



int xmlFirstElementHasSiblings(wchar_t * string)
{	/// given a pointer to the beginning of an xml tag, this function returns 1 if the firstmost tag found has an immediate sibling, and a negative number if not.  negative numbers represent the number of closing tags found between the first located element end, and the second element start.  this is how far back up the tree to climb. 
	/// unlike most search functions, returns zero on error, 1 if the element has an immediate sibling.
	int i = 0;
	wchar_t * temp = NULL;
	wchar_t * temp2 = NULL;
	i = xmlCharsToFirstElement(string);
	if(i == -1)
		return 0;
	string += i;
	
	int a = xmlFirstElementAllLen(string);
	string += a;	//skip to the end of the element
	int l = xmlCharsToFirstElement(string);

	
	if(a < 0 || l < 0)
		return 0;
	
	temp = calloc(l + 1, sizeof(wchar_t));
	if(!temp)
		return 0;
	wcsncpy(temp, string, l + 1);
	temp[l] = L'\0';
	temp2=temp;
//	search for closing tags
	i = 0;
	while((temp2 = wcsstr(temp2, L"</")) != NULL)
	{
		temp2++;
		i++;
	}
	free(temp);
	return (i > 0) ? (0 - i) : 1;
}


