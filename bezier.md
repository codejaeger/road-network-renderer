# Road Input

### Principle:
User can enter multiple roads (hereafter called paths).  
When the user calls the executable, he/she is at Path 1.  
Each path consists of control points given as input by user, which are converted to an approximated smooth curve. The interpolation is done using nth-order Bezier-curve parameterization. The parameterization of the path is updated and rendered on screen at every input.

### Controls:
* ##### Left-click -
Takes the location of left click by user as the input of control points.
* ##### Right-click -
Stops the input of control points until 'R' is pressed.
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

### Code:
Source directory - /src/Bezier-Curve
Include directory - /include/road-network/Bezier-Curve
