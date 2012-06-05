#include "../xmlDataTypes.h"
#include "../treeFunctions.h"
#include "../xmlGetters.h"
#include "../xmlGetDOM.h"
#include "../xmlFunctions.h"

int main(int argc, char ** argv)
{
	int c;
	int i = 0;;
	wchar_t * xmlData = xmlOpenFile(argv[1]);
	wchar_t * temp = xmlData;
	double * dVals = NULL;
	long * iVals= NULL;
	wchar_t *s = NULL;
	if(!xmlData)
	{
		printf("couldn't open xml File");
		exit(EXIT_FAILURE);
	}
	node * tree = NULL;
	tree = xmlGetDOM(xmlData);
	tree = xmlTreeTop(tree);
//	xmlTreePrint(tree);
//	xmlTreeSaveGraphviz(tree, "dotfile.gv", NULL, 0);
	tree = xmlGetNodeFromDotPath(tree, L"COLLADA.library_geometries.geometry.mesh.source.technique_common.accessor.param<%d",2);
	if(!tree)
	{
		printf("xmlGetNodeFromDotPathVAarg():failed\n");
		exit(EXIT_FAILURE);
	}
	printf("%ls\n", tree->xmlData.elementAttr);
	int attrC = xmlGetAttrCount(tree);
	printf("%d attributes\n", attrC);
	printf("%s\n", xmlGetErrMesg());
//	printf("%ls\n", tree->xmlData.elementData);



/*	printf("0%p:\n", tree);*/
/*	printf("%ls\n", tree->xmlData.elementID);*/
/*	printf("%ls\n", tree->xmlData.elementAttr);*/
	dVals = xmlGetDataArrayDouble(tree, 1000);
	//iVals = xmlGetDataArrayLong(tree, 52);
	//s = xmlGetDataArrayString(tree, 0);
	
	if(dVals)
	{
		i = 0;
		while(i < 52){
		printf("%f\n", dVals[i]);
		i++;}
		free(dVals);
	}
//	if(iVals)
//	{
//		i = 0;
//		while(i < 52){
//		printf("%d\n", iVals[i]);
//		i++;}
//		free(iVals);
//	}
//	if(s){
//	
//		printf("%ls\n", s);
//		free(s);
//	}
//		
	xmlTreeFree(tree);
	free(xmlData);
	return 0;
}
