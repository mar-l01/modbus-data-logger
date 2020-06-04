## Building the Application
The application can either be built by calling _build_native.sh_, which is contained in the _scripts/_ directory, directly or by using VSCodes _Build Task_.

### Command line
Within VSCodes integrated terminal run following command from within the _scripts/_ directory:  
```sh
$ ./build_native
```
Add `-t` parameter to additionally build all unit-tests.  

### VSCode Build Task
Within VSCode go to __Terminal__ > __Run Build Tasks...__ and here you can select _Build Native_ or _Build Native & Unittests_ to either only build the application or the unit-tests as well.

