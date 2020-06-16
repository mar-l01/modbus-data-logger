## Building the Application
The application can either be built by calling _build_native.sh_, which is contained in the _scripts/_ directory, directly or by using VSCodes _Build Task_.

### Command line
Within VSCodes integrated terminal run following command from within the _scripts/_ directory:  
```sh
$ ./build_native
```
Add `-t` parameter to additionally build all unit-tests. This will build a debug-target.   
Add `-d` parameter to build a debug target.  
Add `-r` parameter to build a release target.  

### VSCode Build Task
Within VSCode go to __Terminal__ > __Run Build Tasks...__ and here you can select _Build Native DEBUG_, _Build Native RELEASE_ or _Build Native & Unittests_ to either build the application in debug or release mode or the unit-tests as well.

