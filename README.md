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
   
 