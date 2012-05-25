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
	xmlTreePrint(tree);
	xmlTreeSaveGraphviz(tree, "dotfile.gv", NULL, 0);
	tree = xmlGetNodeFromDotPathVAarg(tree, L"COLLADA.library_effects.effect<d.profile_COMMON.newparam", 9);
	if(!tree)
	{
		printf("xmlGetNodeFromDotPathVAarg():failed\n");
		exit(EXIT_FAILURE);
	}
	printf("%ls\n", tree->xmlData.elementAttr);
	printf("%ls\n", tree->xmlData.elementData);



/*	printf("0%p:\n", tree);*/
/*	printf("%ls\n", tree->xmlData.elementID);*/
/*	printf("%ls\n", tree->xmlData.elementAttr);*/
	dVals = xmlGetDataArrayDouble(treeTop(tree), L"COLLADA.library_geometries.geometry.mesh.source.float_array", 52);
	iVals = xmlGetDataArrayLong(treeTop(tree), L"COLLADA.library_geometries.geometry.mesh.triangles.p", 52);
	s = xmlGetDataArrayString(treeTop(tree), L"COLLADA.asset.modified.", 0);
	
	if(dVals)
	{
		i = 0;
		while(i < 52){
		printf("%f\n", dVals[i]);
		i++;}
	}
	if(iVals)
	{
		i = 0;
		while(i < 52){
		printf("%d\n", iVals[i]);
		i++;}
	}
	if(s)
		printf("%ls\n", s);
		free(s);
		free(iVals);
		free(dVals);
	xmlTreeFree(tree);
	free(xmlData);
	return 0;
}
