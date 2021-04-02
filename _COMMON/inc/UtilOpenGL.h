///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilOpenGL.h
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




// system includes ////////////////////////////////////////////////////////////////////////////////


// application global variables, constants and macros /////////////////////////////////////////////



class UtilOpenGL
///////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    static void  showOpenGLVersion(void);
    static void  showGLSLVersion(void);
    static float checkOpenGLVersion(void);
    static void  checkOpenGLErrorCode(void);
    static void  showFPS(const std::string& title = " ");
};
// class UtilOpenGL ///////////////////////////////////////////////////////////////////////////////