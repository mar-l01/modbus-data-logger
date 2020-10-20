## Running the Application
The application binary is located in _builds/build_native/debug/_ or _builds/build_native/release_ depending on the build-type used.  
From there it can be executed with:  
```sh
$ ./modbus_data_logger <mbdl-config>
```
where `<mbdl-config>` needs to be replaced by the path to a configuration file, e.g. _resources/mbdl_config.json_.  

For example, if the application gets started from within the build-directory (_builds/build_native/debug/_), the command can look the following:   
```sh
$ ./modbus_data_logger ../../../resources/mbdl_config.json
```
