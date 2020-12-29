## Building the UI
The UI application can either be built by calling the script contained in the _gui/_ directory or by using VSCodes _Build Task_.

#### Command line
Within VSCodes integrated terminal run following command from within the _gui/_ directory:
```sh
$ ./build_ui.sh
```

### VSCode Build Task
Within VSCode go to __Terminal__ > __Run Build Tasks...__ and here you can select _Build UI_, to build the UI application.

### Requirements
#### Adjust DISPLAY environment variable
It is important to adapt the IP-address given in the _.devcontainer/devcontainer.json_ file's _"DISPLAY"_ variable of the _"containerEnv"_ part. Keep the _:0.0_ part which is appended to the IP address.

#### Install X11 Server
To be able to show the UI application window inside the container, a X11 server needs to be installed on the host system. For example, on a Windows host, one can install _VcXsrv_ (from sourceforge: https://sourceforge.net/projects/vcxsrv/).

##### VcXsrv
Download the _.exe_ file using above link and follow the installation commands. Apply the default settings, except in the _Extra Settings_ step. Check _Disable access control_ as seen in Figure 1.  

<img width="500" alt="Extra Settings" src="https://user-images.githubusercontent.com/9079869/103296473-75149e00-49f6-11eb-86aa-dd73a2020a63.JPG">
*Figure 1: Extra Settings checkbox*



