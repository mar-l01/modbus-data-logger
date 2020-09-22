## Building the Application
The application can either be built by calling the scripts contained in the _scripts/_ directory or by using VSCodes _Build Task_.  
Currently, it is possible to build the project as command line application or shared library to be included in other projects.  

### Command line
#### Native
Within VSCodes integrated terminal run following command from within the _scripts/_ directory:  
```sh
$ ./build_native
```  
Add `-a` parameter to select a different target.  
Add `-t` parameter to additionally build all unit-tests. This will build a debug-target.  
Add `-d` parameter to build a debug target.  
Add `-r` parameter to build a release target.  

#### Shared
Within VSCodes integrated terminal run following command from within the _scripts/_ directory:  
```sh
$ ./build_shared
```  
Add `-a` parameter to select a different target.  
Add `-d` parameter to build a debug target.  
Add `-r` parameter to build a release target.  

### VSCode Build Task
Within VSCode go to __Terminal__ > __Run Build Tasks...__ and here you can select _Build Native DEBUG_, _Build Native RELEASE_, _Build Native & Unittests_, _Build Shared DEBUG_ or Build Shared RELEASE_ to either build the application as console application or shared library in debug or release mode or the unit-tests as well.
