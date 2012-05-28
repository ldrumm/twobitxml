#include "xmlDataTypes.h"
#ifndef XML_GET_DOM
#define XML_GET_DOM

node * xmlGetDOM(wchar_t * xmlText);
node * xmlGetDOMFromPath(const char * path);

#endif
