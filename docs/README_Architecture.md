## Architecture
The intention behind the _modbus-data-logger_ project was not only to provide a useful tool to the public to monitor a Modbus communication flow but also to trying to implement a __clean architecture__ with a __domain driven design__.

### Background Information on this Project
After reading Robert C. Martins book about _Clean Architecture_ (https://www.amazon.com/Clean-Architecture-Craftsmans-Software-Structure/dp/0134494164) and fascinated by the fact that you can achieve a lot with a good architecture, but can also make life hard with a bad one, I wanted to try it on my own project. So I started thinking about a good project idea. Due to the fact that I worked a lot with Modbus communication in my previous employment and therefore also knew that there were few good monitoring applications, I have started to work out a concept for such an application.
Applying the rules and heuristics mentioned in Martins book I tried to create a clean architecture using a domain driven design. It was not that easy at first, but after one/two weeks I was able to present a first try. I started working on the implementation afterwards and it worked out pretty well. Although from time to time new features (or ideas) were emerging, these were easily integrated into the existing architecture. During the implementation I always kept an eye on the current architecture to not mess anything up. A few refactoring tasks were applied once something did not match correctly.

Generally speaking, I have always followed the following rules (wherever possible):
- According to the clean architecture, I created __different layers__, each having its __own responsibilities__, for example:
    - The outter layer encapsulates all framework-related implementations (e.g. Modbus slave implementation using the libmodbus-ibrary).
    - The inner layer holds all data-types which will likely not change over time, so-called _Entities_ (e.g. Modbus configuration) .
- To __decouple__ lower level modules from higher level components, I used __Dependency Inversion__ as well as __Dependency Injection__ for not relying on the concrete implementation.

As this domain application is a stand-alone module and shall later be integrated by an UI application which is able to display the Modbus data in real-time, I used domain driven design to provide a __solid__, __easy-to-understand__, __maintainable__ and __extendable__ domain source code.

### Context View
The application to develop will be integrated into the environment as shown in Figure 1.
<img width="521" alt="ContextView" src="https://user-images.githubusercontent.com/9079869/92468674-d14b7c00-f1d3-11ea-9fc6-1b8289482637.png">
*Figure 1: Context View*

### General Architecture
The implementation of the application follows the architecture shown in Figure 2.
<img width="683" alt="CurrentArchitecture" src="https://user-images.githubusercontent.com/9079869/92468705-ddcfd480-f1d3-11ea-9c77-caaaf33f21b9.png">
*Figure 2: General Architecture*
