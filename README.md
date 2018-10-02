NatBox - Sanbox for rendering natural phenomenon

* Philospohy - project consists of two main modules - application and engine. 
The engine is a collection of helper classes that are used by the application 
to assist the developmental process. 
Application is derived from IPlatformApp class and is called in the main function.
There are many application in a project.
Study applications are used to study rendering of certain phenomenon and to assist
building of the engine. 
Main applications are purpose of the project and are fully functional rendering systems
that use the engine.

* Engine
Consists of several modules but all have the same purpose - to assist application development.
- platform - helps setting up platform for application (application base class, gui, keyboard etc.).
- renderer - assists rendering process - abstraction of graphics API, helper classes for models, images etc.).
- util - anything that will help.
- core - everything else that is needed
- elements - 

* Available apps (set as first command line argument wwhen running program)
- terrain
- grass

* Development process
a) Create study app to test certain aspect of some rendering.
b) Think weather some part can be universalized and included into engine.
c) Add what is learned to the main application (fully functional rendering system) 

