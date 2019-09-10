# NatBox - sandbox framework for graphics experiments with OpenGL 4.5 API.
## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Usage](#usage)
* [Dependancies](#dependancies)
* [Framework architecture](#framework-architecture)
* [Adding new application](#adding-new-application)
* [Existing applications](#existing-applications)
* [Author](#author)
### General info
NatBox is an application framework for graphics programmers who want to experiment with various graphics programming techniques by
building small to medium sized projects. It uses OpenGL as a graphics API and provides wrappers and various helpers to save time on *mundane* boilerplate tasks
like OpenGL initialization and platform set-up. 
### Technologies
*NatBox* is developed in C++ and compiled with C++17 compatable compiler.
The application is tested on Ubuntu Linux with GCC 8.x compiler and AMD open-source graphics drivers.
ApenGL 4.5 is used as an graphics API. 
### Usage
Users of the *NatBox* are usually developers themselves. New application is created inside the framework and selected by passing it's name
as a command line argument. 
### Dependancies
The dependancies are:  
- GLAD OpenGL loader.
- GLFW multi-platform library for creating window, receiving input.
- [GLM](https://github.com/g-truc) optimized linear math library.
- [JSON](https://github.com/nlohmann) library.
- [IMGUI](https://github.com/ocornut/imgui) user interface library.  
Source code of the dependacies are provided with the project.
### Framework architecture
The project consists of two main modules:
- Application
- Engine  

The *engine* is a collection of helper classes that are used by the application 
to assist the developmental process.  
Application is an actual implementation of a rendering application that uses *engine* facilities.
There are many application in a project.  
Study applications are used to study rendering of certain phenomenon while basic applications are usually larger in scale.  
Engine consists of several sub-modules, but all have the same purpose - to assist application development.  
- platform - helps setting up platform for application (application base class, gui, keyboard etc.).
- renderer - assists rendering process - abstraction of graphics API, helper classes for models, images etc.).
- util - helper utilities.
- core - basic types and concepts used by all applications.  
### Adding new application
To add new application:
1. Add new folder in the '*src/app/'* directory and name it after your application.
2. Add application source files (.h and .cpp) that inherit from IPlatformApp application interface.
3. Configure various parameter by adding *config.h* file.
4. Add application to main factory.
5. Pass application name as a first command line argument.
6. Develop an application using */engine* helpers. 
### Existing applications
*grass* - Physically based grass simulation based on ["A simulation on grass swaying with dynamic wind force"](https://link.springer.com/article/10.1007/s00371-016-1263-7) paper 
(Lee, Lo, Chu, Chang 2016).
### Author
Designed and developed by [Ivars Rb.](https://github.com/ivarsrb)