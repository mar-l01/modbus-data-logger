## Getting started
Following instructions help you in setting up the project locally on your computer.

### Docker
In order to be able to work in a Linux-based environment on a Windows operating system, this repository enables the user to open the project in Visual Studio Code in a Debian container.  

#### Install Docker on Windows
Download and install __Docker__ from https://hub.docker.com/editions/community/docker-ce-desktop-windows.  
Once installed, right-click on the Docker taskbar item and navigate to __Resources > File Sharing__. Here, select the local drive, which should be available in the container (e.g. the local drive this repository gets cloned to).

#### Docker image
The Docker image which is used during development is fetched from DockerHub (https://hub.docker.com/r/markus2101/mbdl-dev). It contains the necessary software components (libraries, frameworks, etc.) required for implementing the application. The Dockerfile can be changed here: https://github.com/Markus2101/mbdl-dev-docker-image

### Visual Studio Code
VS Code has a good integration for starting a Docker container and opening the project within.

#### Install Visual Studio Code on Windows
Download and install VS Code from https://code.visualstudio.com/.  
To enable the container integration, the __Remote Development extension pack__ (https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack) needs to be installed. This is best done inside VS Code by searching inside the Extension tab on the left sidebar for __Remote Development__ and install it.

### Fork git-repository
Create a fork of this repository (https://help.github.com/en/github/getting-started-with-github/fork-a-repo). Clone your forked repository.  
Change the name of your remote from _origin_ to any name you like (e.g. your name). Add another remote called _origin_ which references the original respository your forked from. Following commands need to be executed (replace <your-fork-remote> with your name):  
```sh
$ git remote rename origin <your-fork-remote>
$ git remote add origin https://github.com/Markus2101/modbus-data-logger.git
$ git remote set-url --push origin foo
```
The last step prevents your from accidentally pushing to the main repository.