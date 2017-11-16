## Shot Physics, 
### A project to teach how to use OpenGL and Physics with ODE 
 This project uses OpenGL, QtCreator and a physics engine, with ODE, to build a virtual world with interactions among the objects. The basic project employs an application similar to Angry Birds game where is necessary adapts the launch of a sphere ( velocity and angle) to hit cylindrical objects.
 
-------------------------------------------------------------
## Necessary tools
- OpenGL
	- Install on linux: apt-get install freeglut3 freeglut3-dev
- Qt Creator
- ODE
	- Install ODE 0.12, configuration on linux:
    ./configure --enable-double-precision --with-cylinder-cylinder=libccd  --with-box-cylinder=libccd  --with-capsule-cylinder=libccd  --with-convex-box=libccd  --with-convex-capsule=libccd  --with-convex-cylinder=libccd  --with-convex-sphere=libccd  --with-convex-convex=libccd
    
---------------------------------------------------------
## User guide

Camera manipulation with mouse
- Zoom: press right and left buttons, and move the mouse to front or back
- Rotation: press left button and rotate the mouse
- Translation: press right button and move the mouse 

Projectile manipulation
- UP and DOWN: to rotate the base (cylinder) to top and bottom
- LEFT and RIGHT: to rotate de base (cylinder) to left and right
- A: to increase the velocity of the ball
- D: to decrease the velocity of the ball
- SPACE: to shot the sphere (projectile)

Others
- V: to enable or disable the viewport visualization
- P: to save a print of the screen (is necessary create the folder /screenshot)
