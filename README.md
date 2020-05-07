# modbus-data-logger
Application software which can be used to monitor data during a Modbus TCP communication. This software is located between both communication partners (master and slave) and provides a gateway to forward messages from master to slave and vice versa. This way, the transferred data can be accessed and logged.

![Continuous Integration](https://github.com/Markus2101/modbus-data-logger/workflows/Continuous%20Integration/badge.svg?branch=master)

## Getting started
Following instructions help you in setting up the project locally on your computer.

### Docker
In order to be able to work in a Linux-based environment on a Windows operating system, this repository enables the user to open the project in Visual Studio Code in a Debian container.  

#### Install Docker on Windows
Download and install __Docker__ from https://hub.docker.com/editions/community/docker-ce-desktop-windows.  
Once installed, right-click on the Docker taskbar item and navigate to __Resources > File Sharing__. Here, select the local drive, which should be available in the container (e.g. the local drive this repository gets cloned to).

### Visual Studio Code
VS Code has a good integration for starting a Docker container and opening the project within.

#### Install Visual Studio Code on Windows
Download and install VS Code from https://code.visualstudio.com/.  
To enable the container integration, the __Remote Development extension pack__ (https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack) needs to be installed. This is best done inside VS Code by searching inside the Extension tab on the left sidebar for __Remote Development__ and install it.

### Fork git-repository
Create a fork of this repository (https://help.github.com/en/github/getting-started-with-github/fork-a-repo). Clone your forked repository.  
Change the name of your remote from _origin_ to any name you like (e.g. your name). Add another remote called _origin_ which references the original respository your forked from. Following commands need to be executed (replace <your-fork-remote> with your name):  
`$ git remote rename origin <your-fork-remote>`  
`$ git remote add origin https://github.com/Markus2101/modbus-data-logger.git`  
`$ git remote set-url --push origin foo`  
The last step prevents your from accidentally pushing to the main repository.

## Contributing & Developing
This section gives you a basic overview about the desired development process. Make sure you have forked this repository as described in a section above.

### Opening the project in VS Code
Open VS Code and click on the green-button on the bottom left corner. In the opening command palette select __Remote Containers: Open folder in container__ and select the root of your cloned repository (e.g. modbus-data-logger).

### Working on a new feature
Create a branch from the current _origin/master_ with a meaningful name describing the feature you start working. Working on a feature called _feature/my_first_feature_ requries following console commands, the first getting the current master version, the second creating the actual local feature branch:  
`$ git fetch origin`  
`$ git checkout -b feature/my_first_feature origin/master`  

### Push to remote repository
In order to also save your changed remotely, perform following three steps inside your repository:  
`$ git add .`  
`$ git commit -m "first feature implementation"`  
`$ git push <your-fork-remote> feature/my_first_feature`  

### Creating a Pull Request
Once you have finished your feature, create a pull request. Therefore, go to the GitHub repository, select your feature-branch and click __Create Pull-Request__. Here you can find an article about how to create a Pull-Request: https://help.github.com/en/desktop/contributing-to-projects/creating-an-issue-or-pull-request

### Merging into master
A pull request shall only be merged into the master branch if:  
- all checks have passed successfully
- at least one approval is present
