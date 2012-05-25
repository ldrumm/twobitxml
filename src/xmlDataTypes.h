/*treeDataTypes.h*/
#include <stdbool.h>
#include <wchar.h>

#ifndef XML_DATATYPES_H
#define XML_DATATYPES_H
typedef struct
{
	wchar_t * elementID;	// the basic name or identifier tag without the angled brackets i.e. COLLADA
	wchar_t * elementAttr;	// the attributes from the opening tag i.e. <tag units="meters" name='Luke "Drumgay" Drummond"> 
	wchar_t * elementData;	// the data between the <tag>10 2 0 4 7</tag>
}xmlTreeData;

/* the basic treeNode type
this is the concrete type 
handled by the treeFunctions explicitly*/ 
typedef struct node
{
	int data;
	wchar_t * member;
	struct node * left;
	struct node * right;
	struct node * parent;
	xmlTreeData xmlData;	// structure containing refs to elementID, elementAttr
}node;

typedef struct
{
	wchar_t * from;
	wchar_t * to;
}range;
	
typedef union
{
	double doubleVal;
	long longVal;
	bool boolVal;
	const wchar_t * stringVal;
}xmlValueUnion;

typedef struct {
	xmlValueUnion value;
	int errNum;
}xmlValue;
#endif


