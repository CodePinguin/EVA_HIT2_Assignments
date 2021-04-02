///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilOpenGL.cpp
//
//  \brief      General utility class for OpenGL/OS support
//
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2006-11-06   1.00      klu      Initial file release
//     2009-10-02   1.01      schar9   Mac OS X compatible, C++ standard conform
//     2015-10-12   2.00      klu      Update for new GLUT/FLTK GUI
//     2021-03-01   3.00      klu      Adjustments for FLTK 1.4.x and C++17 compatibility
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <FL/glut.H>


// system includes ////////////////////////////////////////////////////////////////////////////////
#include <ctime>
#include <iostream>
#include <sstream>
using namespace std;


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../inc/UtilOpenGL.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showOpenGLVersion()
// purpose:  Function to show the OpenGL implementation and version.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilOpenGL::showOpenGLVersion(void)
{
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);

    cout << "OpenGL Vendor  : " << (char*)vendor << " (" << (char*)renderer << ")" << endl;
    cout << "OpenGL Version : " << (char*)version << endl;
}
// showOpenGLVersion() ////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: checkOpenGLErrorCode()
// purpose:  This function is called to check for OpenGL errors status flag.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilOpenGL::checkOpenGLErrorCode(void)
{
    GLenum errCode;
    const GLubyte* errString;

    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        cout << "OpenGL Get Error Code: " << errCode << " (" << errString << ")" << endl << endl;
    }
}
// checkOpenGLErrorCode() /////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showGLSLVersion()
// purpose:  Function to show the supported GLSL version.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilOpenGL::showGLSLVersion()
{
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    cout << "  GLSL Version : " << (char*)glslVersion << endl;
}
// showGLSLVersion() //////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: checkOpenGLVersion()
// purpose:  Function to return the OpenGL version as a float number for version checking.
///////////////////////////////////////////////////////////////////////////////////////////////////
float UtilOpenGL::checkOpenGLVersion()
{
    GLint gl_major, gl_minor;
    glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
    glGetIntegerv(GL_MINOR_VERSION, &gl_minor);
    //cout << "DEBUG: OpenGL Version: " << *major << "." << *minor << endl;

    return gl_major + (0.1f * gl_minor);
}
// checkOpenGLVersion() ///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showFPS()
// purpose:  Function to replace current GLUT window title with frames per second (FPS) added
// remarks:  "Append" can not be implemented, since there is no GLUT function to get the 
//           current window title.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilOpenGL::showFPS(const string& title)
{
    // calculate frames per second (FPS)
    static int frame = 0;
    static clock_t time = 0;
    static clock_t timebase = 0;
    stringstream title_fps;

    frame++;
    time = std::clock();
    if (time - timebase > CLOCKS_PER_SEC)
    {
        title_fps.setf(std::ios::fixed, std::ios::floatfield);
        title_fps.precision(2);
        title_fps << title << " ( FPS: " << float(frame * CLOCKS_PER_SEC) / (time - timebase) << " )";
        glutSetWindowTitle(const_cast<char*>((title_fps.str()).c_str()));
        timebase = time;
        frame = 0;
    }
}
// showFPS() //////////////////////////////////////////////////////////////////////////////////////
