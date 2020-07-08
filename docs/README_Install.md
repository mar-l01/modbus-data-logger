## Installing the Application
Following is a manual to install the _modbus-data-logger_ application.

### Clone this repository
Execute following command in a terminal window and change into the repository:
```sh
$ git clone https://github.com/Markus2101/modbus-data-logger.git
$ cd modbus-data-logger
```

### Install prerequisities
In order to get all required libraries and modules execute following scripts inside the _scripts/_ directory:
```sh
$ cd scripts/
$ ./install_prerequisities.sh
```

### Check if all required modules were installed
To make sure that all system requirements are met, execute
```sh
$ python check_system_requirements.py
```
which will check if the installed versions of the target _OS_, _CMake_ and _GCC_ are sufficient in order to build the application in the next step. If an error occurs, try to update the required module. Otherwise, building it might not work successfully.

### Build the application
Run the build-script to build the application in release mode for your target:
```sh
$ ./build_native.sh -r
```
By default, x86_64-linux-gnu is set. To build for another target, add `-a <TARGET_ARCHITECTURE>`.

### Move dependencies to specified directories
After a successful build of the application, the application and its dependencies can be installed on the system, meaning they are moved into their respective target folders. Following scripts will do exactly that:
```sh
$ ./install.sh
```

### Starting the application
The application got installed into _/opt/mbdl_ together with its configuration file. Open the file *mbdl_config.json* and adjust all necessary parameters to fit your needs, e.g. ip-addresses and ports. Afterwards the application an be started, for instance, manually with:
```sh
$ cd /opt/mbdl
$ ./modbus_data_logger mbdl_config.json
```
