## Contributing & Developing
This section gives you a basic overview about the desired development process. Make sure you have forked this repository as described in a section above.

### Opening the project in VS Code
Open VS Code and click on the green-button on the bottom left corner. In the opening command palette select __Remote Containers__: Open folder in container__ and select the root of your cloned repository (e.g. modbus-data-logger).

### Project structure
- __domain__: contains the actual source code separated into different subfolders. These folders represent various namespaces used to structure the project. Within each folder a _includes_, _sources_, (_interfaces_) and _testing_ subfolder is present which contain header files (.hpp), source files (.cpp), (abstract classes acting as interfaces (.hpp)) and unittests. The latter might contain a _gmock_ folder holding mocked objects.
    - __application__: holds all components used to be accessed from external components (e.g. a facade pattern) and a main() method to start-up the application in the console
    - __entity__: holds all core business data related components, e.g. data structures of Modbus data
    - __framework__: holds all framework-dependent components, e.g. files including 'libmodbus' library
    - __gateway__: holds all controllers and gateway related components, e.g. object used to set up the basic Modbus gateway communication
    - __logging__: holds all logging related components, e.g. a file logger
    - __utility__: holds utility components, e.g. a timer which can be started or it can be used as single-shot timer
- __integrationtests__: contains all files necessary to set up integrationtests.
    - __fixtures__: holds all fixture classes for running the integrationtests
- __scripts__: contains several scripts used for building the project or running some checks or test-scripts.

### Working on a new feature
Create a branch from the current _origin/master_ with a meaningful name describing the feature you start working. Working on a feature called _feature/my_first_feature_ requries following console commands, the first getting the current master version, the second creating the actual local feature branch:
```sh
$ git fetch origin
$ git checkout -b feature/my_first_feature origin/master
```

### Push to remote repository
In order to also save your changed remotely, perform following three steps inside your repository:
```sh
$ git add .
$ git commit -m "first feature implementation"
$ git push <your-fork-remote> feature/my_first_feature
```

### Creating a Pull Request
Once you have finished your feature, create a pull request. Therefore, go to the GitHub repository, select your feature-branch and click __Create Pull-Request__. Here you can find an article about how to create a Pull-Request: https://help.github.com/en/desktop/contributing-to-projects/creating-an-issue-or-pull-request

### Merging into master
A pull request shall only be merged into the master branch if:
- all checks have passed successfully
- at least one approval is present
