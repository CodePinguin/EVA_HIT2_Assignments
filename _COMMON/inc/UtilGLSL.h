///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilGLSL.h
//
//  \brief      Implementation of GLSL utility class for OpenGL GLSL support
//
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2014-11-27   1.10      klu      Initial file release
//     2015-09-24   1.30      klu      Update for new shader based CG course
//     2021-03-01   3.00      klu      Adjustments for FLTK 1.4.x and C++17 compatibility
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// system includes ////////////////////////////////////////////////////////////////////////////////


// application global variables, constants and macros /////////////////////////////////////////////
#define GL_BUFFER_OFFSET(offset) ((const GLvoid*) (offset))



class UtilGLSL
///////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    static void   initOpenGLDebugCallback(void);
    static void   checkShaderInfoLog(GLuint shader);
    static void   checkProgramInfoLog(GLuint program);
    static GLuint initShaderProgram(int argc, char **argv);

private:
    static char*  readShaderFile(const std::string& filename);
    static void   DebugMessageCallback(GLenum source, GLenum type, GLuint id,
            GLenum severity, GLsizei length, const GLchar* message, void* userParam);
};
// class UtilGLSL /////////////////////////////////////////////////////////////////////////////////