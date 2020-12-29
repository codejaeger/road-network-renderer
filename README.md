![Build Status](https://travis-ci.com/codejaeger/road-network-renderer.svg?branch=master)
# Road Network 3D Rendering using OpenGL

### Abstract

This project focuses on the task of creating a UI for visualizing traffic flow in a city(a road network) using OpenGL. The problem of traffic congestion has seen different types of approaches from using simple heuristics to complex machine learning based approaches to predict correct traffic signals in a road network. This project does not aim to solve the traffic congestion problem but serve as a tool to help solving the aforementioned problem.


### Details of the project

We will be using OpenGL to render a city and its various components. Elaborating on the previous line , we will create models in C++ using OpenGL to simulate objects like roads, cars, traffic signals, road junctions etc . The UI will be able to read object meshes provided by user for models. All this can be modeled in OpenGL without significant efforts , but the important part is to integrate the UI with the visual platform for e.g. rendering turns on roads as realistically as possible(using curve interpolations). The UI will be as simplistic as possible for usage. This means that the user will be able to provide the road network in the form of a road network file or a more graphical format. The major part of the project will be on creating a crude UI developed at first and then adding extra features. For example possible additions could be -

1. Allowing widely varying dimensions of vehicle models

2. Check for collisions and abort with proper messaging or feedback.


### Compiling

To compile the project, use the command:
> make clean;  
> make;

There would be two executables present in ./bin  
1. input  
  For creating the road network and assigning the source and the destination for the cars.
2. output  
  Rendering the road Network and animating the Traffic flow.


### Controls for input
* #### Keyboard:
  * ##### 'R' -
  Resumes the input of control points.
  * ##### 'N' -
  Moves to next path. Input of control stops until 'R' is pressed.
  * ##### 'P' -
  Moves to previous path. Input of control stops until 'R' is pressed.
  * ##### 'D' -
  Deletes the last control point in the current path.
  * ##### 'S' -
  The control points and interpolated points are stored in two raw files, with extension '.min.raw' and '.raw' respectively in models/Bezier-Curve.
  * ##### 'L' -
  The control points stored in the '.min.raw' are loaded and the paths are rendered with the screen displaying from Path 1.
  To start editing the paths, the user must press 'R' to resume.
  * ##### 'ESC' -
  The window is closed and all unsaved work is lost.
* #### Mouse / Touchpad:
  * ##### Left-click -
  Takes the location of left click by user as the input of control points.
  * ##### Right-click -
  Stops the input of control points until 'R' is pressed.

### Controls for output
* #### Keyboard:
  * ##### 'A', 'D' -
  Rotates the road-network about y-axis.
  * ##### 'W', 'S' -
  Rotates the road-network about x-axis.
  * ##### 'Q', 'E' -
  Rotates the road-network about z-axis.
  * ##### UP arrow button -
  Translates the camera in the direction of it's Up-vector.
  * ##### DOWN arrow button -
  Translates the camera in the opposite direction of it's Up-vector.
  * ##### RIGHT arrow button -
  Translates the camera in the direction of it's Right-vector.
  * ##### RIGHT arrow button -
  Translates the camera in the opposite direction of it's Right-vector.
  * ##### 'I' -
  Zooms-in the model by moving the camera in the direction of it's front vector.
  * ##### 'O' -
  Zooms-out the model by moving the camera in the opposite direction of it's front vector.
* #### Mouse / Touchpad:
  * ##### Scroll -
  Scrolling-up zooms-in the model and Scrolling-down zooms-out the model.
  * ##### Mouse cursor movement -
  Direction of the front and consequently right vector of the camera is changed preserving its position.  
  Can be used to look-around the model from one-place. At the start of the program the cursor is snapped to the center of the screen.


### For further reading:
[Bezier Curve](./src/Bezier-Curve)  
[Road Model](./src/Road-Model)  
[OpenGL Resource](./resources/Addison.Wesley.OpenGL.Programming.Guide.8th.Edition.Mar.2013.ISBN.0321773039.pdf)
