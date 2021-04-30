# SiCG-Part-1

Simulation in Computer Graphics Project Part 1

## Running

### Mac

- On mac, make sure to `brew install libpng` and install C/C++ and MakeFile Tools packages
- Change the non-Windows flags to `CXX_EXTRA_FLAGS = -framework GLUT -framework OpenGL -lpng`

### Windows

- Replace all `<GLUT/glut.h>` by `<GL/glut.h>` (or just GLUT -> GL)
- Make sure your msys and MinGW are set up correctly
