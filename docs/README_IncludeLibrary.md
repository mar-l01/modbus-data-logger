## Include application as shared library
Build this application as shared library (called _libmbdl_) to include it in your own project.  

### Prerequisities
In order to include this application as shared library it is necessary to have the package _PkgConfig_ installed.  
The CMakeLists.txt file uses this package to locate the _libmbdl_ library. Run following command from within the command line:  
```sh
$ sudo apt-get install pkg-config
```  

### Include it into your CMakeLists.txt file
Add following lines to your _CMakeLists.txt_ file, which will search for the _libmbdl_ library on your system:  
```sh
find_package(PkgConfig)
pkg_check_modules(libmbdl REQUIRED libmbdl>=0.1.0)
```  

Do not forget to link libmbdl.so.0.1.0 to your project, e.g. link it to _test_project_ like this:
```sh
target_link_libraries(test_project PRIVATE /usr/include/libmbdl.so.0.1.0)
``` 