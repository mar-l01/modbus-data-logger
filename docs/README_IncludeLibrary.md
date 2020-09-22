## Include application as shared library
Build this application as shared library (called _libmbdl_) to include it in your own project.

### Prerequisities
In order to include this application as shared library it is necessary to have the package _PkgConfig_ installed.
The _CMakeLists.txt_ file uses this package to locate the _libmbdl_ library. Run following command from within the command line:
```sh
$ sudo apt-get install pkg-config
```

### Build shared library
Run script _build_shared.py_ in _scripts/_ directory to build the modbus-data-logger as shared library. For example, to build it in release mode run following command, which will build the shared library and install the required components to the respective parts in the filesystem:
```sh
$ python scripts/build_shared.py -r
```

### Include it into your CMakeLists.txt file
Add following lines to your _CMakeLists.txt_ file, which will search for the _libmbdl_ library on your system:
```sh
find_package(PkgConfig)
pkg_check_modules(LIBMBDL REQUIRED libmbdl>=0.1.0)
```

Do not forget to link _libmbdl.so.0.1.0_ to your project, e.g. link it to _test_project_ like this:
```sh
target_link_libraries(test_project PRIVATE ${LIBMBDL_LIBDIR}/libmbdl.so.${LIBMBDL_VERSION})
```

### Example CMakeLists.txt
A simple _CMakeLists.txt_ file which includes the _libmbdl.so_ library can look as following:
```sh
cmake_minimum_required(VERSION 3.10)

project(test_project)

set(CMAKE_CXX_STANDARD 17)

# find pkg-config package on system
find_package(PkgConfig)

# check if libmbdl is installed with version 0.1.0 or newer
pkg_check_modules(LIBMBDL REQUIRED libmbdl>=0.1.0)

add_executable(test_project "TestProjectMain.cpp")
target_compile_options(test_project PUBLIC -Wall -Wextra)

# link to libmbdl.so.0.1.0 library
target_link_libraries(test_project PRIVATE ${LIBMBDL_LIBDIR}/libmbdl.so.${LIBMBDL_VERSION})
```
