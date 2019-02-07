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

Dogmas:
- Right-handed coordinate system with Y axis going up
- Positive angle rotations are counter-clockwise when looking from positive axis of rotation
- All models initially should face positive Z-axis (0 rotation angle about Y axis) and UP being positive Y-axis
- 1 space unit is 1 meter

* Dependencies
- C++17 compiler
- OpenGL 4.5 drivers
- glfw3 lib
- glm lib
