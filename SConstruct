
## All files needed for shared library file
lib_obj = ['src/xmlFunctions.c', 'src/treeFunctions.c', 'src/xmlGetters.c', 'src/xmlGetDOM.c', 'src/xmlError.c']

## All files needed to run tests
test_objs = ['src/tests/test.c']


#debug version for testing
#debug = Environment(CCFLAGS='-g') 
#lib_obj = ['src/xmlFunctions.c', 'src/treeFunctions.c', 'src/xmlGetters.c', 'src/xmlGetDOM.c', 'src/xmlError.c']
#debug.SharedLibrary('twobitxml', lib_obj)

#optimized version for installation

release_env = Environment(CCFLAGS='-O2 -g') 
release = release_env.SharedLibrary('twobitxml', lib_obj)
Default(release)


