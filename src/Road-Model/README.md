# Road Output

### Principle:
The paths stored as the input by the user in .raw file are read and a road network is initialized using those.  
First, Roads and Road-separators are created followed by detection of intersections and their initialization.  
Very close intersections are merged into one and after this, road-network graph is initialized for the use by the traffic manager.  

### Controls:
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
* #### Mouse:
  * ##### Scroll -
  Scrolling-up zooms-in the model and Scrolling-down zooms-out the model.
  * ##### Mouse cursor movement -
  Direction of the front and consequently right vector of the camera is changed preserving its position.  
  Can be used to look-around the model from one-place. At the start of the program the cursor is snapped to the center of the screen.


### Code:
Source directory - /src/Road-Model and /src/Road-Graph  
Include directory - /include/road-network/Road-Model and /include/road-network/Road-Graph.
