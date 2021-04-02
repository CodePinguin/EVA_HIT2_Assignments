///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-03-D.03_SimpleAnimation [Version 3.1 - FLTK-1.4.x / C++17 Update]                    //
///////////////////////////////////////////////////////////////////////////////////////////////////




// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <FL/glut.H>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>


// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <stack>
using namespace std;


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../../_COMMON/inc/UtilCPP.h"
#include "../../_COMMON/inc/TrackBall.h"
#include "../../_COMMON/inc/UtilOpenGL.h"
#include "../../_COMMON/inc/UtilGLSL.h"
#include "../../_COMMON/inc/vbosphere.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;

float MOVE_X = 0.0f;
bool ANIMATION_RUNNING = false;
int SPEED = 80;

VBOSphere* TheSphere = nullptr;



void glutTimerCB(int timer_id)
/////////////////////////////////////////////////////////////////////////////////////////
{
    static float alpha = 0.0f;
    alpha += 0.2f;
    MOVE_X = 4 * sin(alpha);
    //cout << "MOVE_X: " << MOVE_X << endl;

    // reset timer if the animation is running
    if (ANIMATION_RUNNING) glutTimerFunc(SPEED, glutTimerCB, 0);

    glutPostRedisplay();
}



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup modelview matrix
    glm::mat4 modelview(1.0f);

    // apply trackball transformation to modelview matrix
    glm::mat4 mouse = TrackBall::getTransformation();
    modelview = modelview * mouse;

    // set modelview transformation matrix in vertex shader
    modelview = glm::rotate(modelview, glm::radians<float>(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelview = glm::scale(modelview, glm::vec3(2.0f, 2.0f, 2.0f));
    modelview = glm::translate(modelview, glm::vec3(MOVE_X, 0.0f, 0.0f));
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));

    TheSphere->draw();

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    TheSphere = new VBOSphere(1.0f, 50, 25);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set rendering mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");

    // setup orthographic projection matrix
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
    glUniformMatrix4fv(PROJECTION_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));
}



void glutMenuCB(int key)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (key)
    {
        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
        case ' ': // toggle animation on/off
        {
            ANIMATION_RUNNING = !ANIMATION_RUNNING;
            if (ANIMATION_RUNNING)
            {
                glutTimerFunc(SPEED, glutTimerCB, 0);
            }
            break;
        }
        case '1': // increase animation speed
        {
            SPEED -= 1;
            if (SPEED < 2) SPEED = 1;
            cout << "ANIMATION_SPEED: " << SPEED << endl;
            break;
        }
        case '2': // decrease animation speed
        {
            SPEED += 1;
            cout << "ANIMATION_SPEED: " << SPEED << endl;
            break;
        }
        case 'r': case 'R': default: // reset settings
        {
            MOVE_X = 0.0f;
            ANIMATION_RUNNING = false;
            SPEED = 80;
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
    glutAddMenuEntry("Start Animation [SPACE]", ' ');
    glutAddMenuEntry("Increase Speed [1]", '1');
    glutAddMenuEntry("Decrease Speed [2]", '2');
    glutAddMenuEntry("Reset Settings [R]", 'R');
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
    glutCreateWindow("Simple Animation");

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
        argv[1] = "../../glsl/animation.vert";
        argv[2] = "../../glsl/animation.frag";
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
