 OpenGL Projects Readme (macOS)
===

*Note:* we support current Apple Hardware (at most 2 years old), and macOS (current), but not older than 1 version behind current (e.g. macOS 11, 10.15). Further, to be able to do any meaningful work on macOS with either Objective-C, 
C or C++ you must first install the Apple Development toolchain. 
To do so, log in to the Apple App Store and install Xcode. You will
be guided through the rest of the setup (incl. the command line tools).

You need to install CMake. Download it from here:
https://cmake.org/download/ and install it accordingly. 
Then follow the instructions given in class. Or use `homebrew` to install it: `brew install cmake`

Please be aware the we officially only support Xcode. So, your mileage may vary with other IDEs (Visual Studio Code, CLion etc.).

OpenGL
---

Either Core OpenGL up to 4.1 or (legacy compatible) OpenGL 2.1 is supported! 
Eliminate everything in the code which uses features > 4.1.

GLEW
---

Not needed on macOS. To be consistent with the other platforms,
the library is there anyway. Therefore, checks are made easier and
no preprocessor hacks are needed.

The header file `<GL/glew.h>` needs to be included before any OpenGL/GLUT specific headers.

There might be some `OpenGL Get Error Code: 1280 (invalid enumerant)`
error messages during rendering. This is due to GLEW according to:
https://www.opengl.org/wiki/OpenGL_Loading_Library (GLEW section).

FLTK for macOS
---

We provide precompiled libraries for FLTK 1.4 (pre-release). CMake build scripts should automatically find and include all files necessary.

FLTK uses either the Core Profile or the Legacy Profile (default). To enable the Core Profile, 
add the `FL_OPENGL3` flag to the argument list of `glutInitDisplayMode(...| FL_OPENGL3);`

GLUT
---

Replace all include directives with `<FL/glut.H>`

Freeglut
---

Not needed on macOS. Replaced with FLTK's GLUT implementation.

*Document version*

Author and Date: peter.vonniederhaeusern@bfh.ch, 5 February 2021
