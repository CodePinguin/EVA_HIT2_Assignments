///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-02-D.02_HelloGLSL [Version 3.1 - FLTK-1.4.x / C++17 Update]                          //
///////////////////////////////////////////////////////////////////////////////////////////////////




// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <FL/glut.H>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
using namespace std;


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../../_COMMON/inc/UtilCPP.h"
#include "../../_COMMON/inc/TrackBall.h"
#include "../../_COMMON/inc/UtilOpenGL.h"
#include "../../_COMMON/inc/UtilGLSL.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    glClear(GL_COLOR_BUFFER_BIT);

    // setup modelview matrix
    glm::mat4 modelview(1.0f);

    // apply trackball transformation to modelview matrix
    glm::mat4 mouse = TrackBall::getTransformation(); // es dreht sich um den KOS-Ursprung
    modelview = modelview * mouse;

    // set modelview transformation matrix in vertex shader
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));

    // draw triangle around origin
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // define triangle vertices
    float base = 5.0f;
    GLfloat vertices[] =
    {
        -base, -base, 0.0f, 1.0f,  // v0
         base, -base, 0.0f, 1.0f,  // v1
            0,  base, 0.0f, 1.0f,  // v2
    };

    // setup and bind Vertex Array Object for triangle
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // setup Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // get vertex position attribute location and setup vertex attribute pointer
    // (requires that the shader program has been compiled already!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    // clear window background
    glClear(GL_COLOR_BUFFER_BIT);

    // set rendering mode
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // set default drawing color
    GLint location = glGetUniformLocation(PROGRAM_ID, "drawColor");
    float drawColor[3] = {0.8f, 0.6f, 0.0f};
    glUniform3fv(location, 1, drawColor);

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");

    // setup orthographic projection matrix
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f); //x (von bis), y (von bis), z (von bis) in arbitrary units
    glUniformMatrix4fv(PROJECTION_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));
}



void glutKeyboardCB(unsigned char key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (key)
    {
        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
    }
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // init GLUT/FLTK settings
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | FL_OPENGL3);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Hello GLSL");

    // register extension wrapper library (GLEW)
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: GLEW not initialized: " << glewInit() << endl;
        return -1;
    }

    // show version of OpenGL and GLSL
    UtilOpenGL::showOpenGLVersion();
    UtilOpenGL::showGLSLVersion();
    cout << endl;

    // show version of C++ and compiler
    UtilCPP::showCPPLanguageVer();
    UtilCPP::showCPPCompilerVer();
    cout << endl;

    // check for shader 4.x support
    if (UtilOpenGL::checkOpenGLVersion() < 4.0)
    {
        cout << "Program requires OpenGL Version >= 4.x (Supported: ";
        cout << UtilOpenGL::checkOpenGLVersion() << ") - exiting!" << endl;
        cout << "Press ENTER to continue... " << endl;
        cin.ignore();
        exit(1);
    }

    // register OpenGL debugging callback (if available)
    UtilGLSL::initOpenGLDebugCallback();

    // register GLUT/FLTK callbacks
    glutDisplayFunc(glutDisplayCB);
    glutKeyboardFunc(glutKeyboardCB);

    // register mouse handler callbacks
    glutMouseFunc(TrackBall::glutMouseButtonCB);
    glutMotionFunc(TrackBall::glutMouseMotionCB);
    glutSpecialFunc(TrackBall::glutSpecialFuncCB);

    // check for command line argument supplied shaders
    if (argc > 1)
    {
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }
    else
    {
        argc = 3;
        argv[0] = "";
        argv[1] = "../../glsl/helloglsl.vert"; // Shader-Files
        argv[2] = "../../glsl/helloglsl.frag";
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }

    // init application
    initRendering();
    initModel();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}