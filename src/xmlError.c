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
#include <stdarg.h>
#include <stdio.h>
#include "xmlError.h"


static const char * _xmlError(const char * message, va_list argp)
{
	static char error[10][XML_ERR_LEN];
	static int last_error_index = 0;
	
	if(!message)			//get last error by calling with NULL
	{
		last_error_index--;
		if(last_error_index < 0)
			last_error_index = 9;
		return (const char *)&(error[last_error_index+1]);
	}
	last_error_index++;
	last_error_index %= 10;
	vsnprintf(&(error[last_error_index][0]), XML_ERR_LEN-1, message, argp);
}



void _xmlSetError(const char * message, ...)
{
	if(!message)	//avoid overwriting error list with NULLS
		return;
	va_list argp;
	va_list argp2;
	va_start(argp, message);
	va_copy(argp2, argp);
	_xmlError(message, argp2);
}

const char * xmlGetErrMesg(void)
{
	return _xmlError(NULL, NULL);
}

