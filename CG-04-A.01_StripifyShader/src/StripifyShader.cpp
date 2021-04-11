///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-04-A.01_StripifyShader [Version 3.1 - FLTK-1.4.x / C++17 Update]               //
//             (Identical with CG-04_D.01_BasicRendering)                                        //
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
#include "../../_COMMON/inc/vboteapot.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;

glm::mat4 TheCameraView(1.0f);
VBOTeapot* TheTeapot = nullptr;



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup modelview matrix, init with camera view
    glm::mat4 modelview = TheCameraView;

    // apply trackball transformation to modelview matrix
    glm::mat4 mouse = TrackBall::getTransformation();
    modelview = modelview * mouse;

    // move teapot into origin and rotate 270 degree around x-axis (post multiply order)
    glm::mat4 teapot_matrix(1.0f);
    teapot_matrix = glm::rotate(teapot_matrix, glm::radians<float>(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    teapot_matrix = glm::translate(teapot_matrix, glm::vec3(0.0f, 0.0f, -1.5f));
    modelview = modelview * teapot_matrix;

    // set modelview transformation matrix in vertex shader
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));

    // draw teapot with offset (i.e. origin centered)
    TheTeapot->draw();

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    glm::mat4 lidTransform(1.0f);
    lidTransform = glm::scale(lidTransform, glm::vec3(1.08f, 1.08f, 1.0f));
    TheTeapot = new VBOTeapot(16, lidTransform);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set rendering mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // setup the camera view matrix
    TheCameraView = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");
}



void glutResizeCB(int width, int height)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // this function is called before the window's first display

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // adjust aspect value for new viewport size
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // setup perspective projection matrix for new viewport size
    glm::mat4 projection = glm::perspective(glm::radians<float>(60.0f), aspect, 1.0f, 20.0f);
    glUniformMatrix4fv(PROJECTION_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));

    glutPostRedisplay();
}



void glutMenuCB(int key)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    int argc = 0; char* argv[10];

    switch (key)
    {
        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
        case '1': // delete old shader program and setup toonify1 shader files
        {
            if (PROGRAM_ID) glDeleteProgram(PROGRAM_ID);

            argc = 4;
            argv[0] = "";
            argv[1] = "../../glsl/toonify.vert";
            argv[2] = "../../glsl/toonify.frag";
            argv[3] = "../../glsl/toonify1.frag";
            PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);

            // reset the projection and camera matrix uniform of new shader program
            glutResizeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;
        }
        case '2': // delete old shader program and setup toonify2 shader files
        {
            if (PROGRAM_ID) glDeleteProgram(PROGRAM_ID);

            argc = 4;
            argv[0] = "";
            argv[1] = "../../glsl/toonify.vert";
            argv[2] = "../../glsl/toonify.frag";
            argv[3] = "../../glsl/toonify2.frag";
            PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);

            // reset the projection and camera matrix uniform of new shader program
            glutResizeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            break;
        }
// TODO: add additional code to setup stripify shader pipeline
        case 'r': case 'R': default: // reset settings
        {
            TrackBall::resetTransformation();
            break;
        }
    }
    glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // register callback and create menu
    glutCreateMenu(glutMenuCB);
    glutAddMenuEntry("Use Toonify1 Shader [1]", '1');
    glutAddMenuEntry("Use Toonify2 Shader [2]", '2');
// TODO: add additional menu to select stripify shader
    glutAddMenuEntry("Exit [Q] or [ESC]", 'Q');

    // attach menu to right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



void glutKeyboardCB(unsigned char key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // let the glutMenuCB() do the keyboard handling
    glutMenuCB(static_cast<int>(key));
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // init GLUT/FLTK settings
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Stripify Shader");

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
    glutReshapeFunc(glutResizeCB);

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
        argc = 4;
        argv[0] = "";
        argv[1] = "../../glsl/toonify.vert";
        argv[2] = "../../glsl/toonify.frag";
        argv[3] = "../../glsl/toonify1.frag";
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }

    // init application
    initRendering();
    initModel();
    initMenu();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
