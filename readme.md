twobitxml
=========
libtwobitxml is a lightweight DOM-based xml parsing library written in C. It is non-validating and intended to simplify a programmers route to proper XML import. 

**Features**

  - A very simple and elegant API that allows proper XML file import, search, and C-native-type extraction in less than twelve parsecs (about 4 function calls).
  - Fairly robust, ignoring unknown entities, and gracefully handling badly formed XML.
  - All text files are converted internally to full unicode, using libiconv, so almost all text encodings can be handled with little or no modification to the library.
  - Graphviz visualisation of the XML DOM heirarchy.

**Three Step Tutorial**
-
> **1 - Import your XML file**

    wchar_t * xmlText = xmlOpenFile("path/to/my/file.xml");
    
    
> **2 - Extract everything and build DOM**

    node  * tree = xmlGetDOM(xmlText);
    

> **3 - Get the data you want as a C-native type using familiar Javascript like dotted separators**
    
    tree = xmlGetNodeFromDotPath(tree, L"DocRoot.drawing.pixels.rgb_triplets");   	//find node
    pixels = xmlGetDataArrayDouble(tree, 10);										//get a native C array of 12 data elements
  

Dependencies and Supported systems
-----------

twobitxml expects internal UCS-4 support, and libiconv. Therefore this should work pretty well on any unixish system. For a number of reasons, it is unlikely that this library is going to work on Windows without a bit of hacking. This should not be difficult. If you want to help this effort, please get in touch.

* [libiconv] - GNU software's conversion routines for text transformation etc.

Building
--------------

After cloning the source tree, you can build this very quickly.  The simplest possible scons file has been included, but if you don't use scons, this should do:
1. `cd twobitxml/src`
2. `gcc *.c -fPIC -shared -olibtwobitxml.so`

Documentation
--------------

A Doxygen generated programmers' manual is available in PDF, HTML etc, and can be built from the source.

1. Clone the repo
2. `cd twobitxml`
3. `doxygen` 
4. `cd docs/latex`
5. `make`
6. Read the refman.pdf file located in the latex folder.



[libiconv]: www.gnu.org/s/libiconv/
  
    
