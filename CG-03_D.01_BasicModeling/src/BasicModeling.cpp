///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-03-D.01D_BasicModeling [Version 3.1 - FLTK-1.4.x / C++17 Update]                     //
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

GLenum POLYGON_MODE = GL_FRONT_AND_BACK;
bool USE_WIREFRAME = false;
bool USE_DEPTH_TEST = false;
bool USE_CULLING = false;

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[4];

const float WIDTH = 10.0f; // width of cube
GLuint INDICES_COUNT = 0; 



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup modelview matrix
    glm::mat4 modelview(1.0f);

    // apply trackball transformation to modelview matrix
    glm::mat4 mouse = TrackBall::getTransformation();
    modelview = modelview * mouse;

    // set modelview transformation matrix in vertex shader
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));

    /* definition of cube face indices (for GL_TRIANGLES --> 30)
    static GLushort indices[] =
    {
        // 4 sides
        0, 1, 2,
        3, 0, 2,

        1, 7, 6,
        2, 1, 6,

        4, 5, 6,
        7, 4, 6,

        4, 0, 3,
        5, 4, 3,

        // floor
        0, 4, 7,
        1, 0, 7
    };
    INDICES_COUNT = sizeof(indices) / sizeof(indices[0]);
    //*/

    /* definition of two perpendicular planes face indices (for GL_TRIANGLES --> 12)
    static GLushort indices[] =
    {
        0, 4, 6,
        6, 2, 0,

        3, 5, 7,
        7, 1, 3
    };
    INDICES_COUNT = sizeof(indices) / sizeof(indices[0]);
    //*/

    /* definition of cube face indices (for GL_TRIANGLE_STRIP --> 15)
    static GLushort indices[] =
    {
        // 4 sides
        3, 0, 2, 1,
        6, 7,
        5, 4,
        3, 0,

        // floor
        0xFFFF,
        0, 4, 1, 7
    };
    INDICES_COUNT = sizeof(indices) / sizeof(indices[0]);
    glPrimitiveRestartIndex(0xFFFF);
    glEnable(GL_PRIMITIVE_RESTART);
    //*/


    // de-referencing vertices and draw geometry using glDrawArrays
    //glDrawArrays(GL_TRIANGLES, 0, 8);
    //glDrawArrays(GL_QUADS, 0, 8);


    // de-referencing vertices and draw geometry using glDrawElements with triangles
    //glDrawElements(GL_TRIANGLES, INDICES_COUNT, GL_UNSIGNED_SHORT, indices);

    // de-referencing vertices and draw geometry using glDrawElements with triangle strip
    //glDrawElements(GL_TRIANGLE_STRIP, INDICES_COUNT, GL_UNSIGNED_SHORT, indices); 
    glDrawElements(GL_TRIANGLE_STRIP, INDICES_COUNT, GL_UNSIGNED_SHORT, 0);


    //glFlush();
    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(float width)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // definition of the cube vertices (X, Y, Z)
    float side = width / 2.0f;
    GLfloat vertices[] =
    {
        -side, -side,  side, 1.0f,  // v0
         side, -side,  side, 1.0f,  // v1
         side,  side,  side, 1.0f,  // v2
        -side,  side,  side, 1.0f,  // v3
        -side, -side, -side, 1.0f,  // v4
        -side,  side, -side, 1.0f,  // v5
         side,  side, -side, 1.0f,  // v6
         side, -side, -side, 1.0f   // v7
    };

    // definition of the cube colors, each vertex has his own color definition (RGB)
    GLfloat colors[] =
    {
        1.0f, 0.2f, 0.2f,
        1.0f, 0.2f, 0.2f,
        1.0f, 0.2f, 1.0f,
        1.0f, 0.2f, 1.0f,
        1.0f, 1.0f, 0.2f,
        1.0f, 1.0f, 0.2f,
        0.2f, 0.2f, 1.0f,
        0.2f, 0.2f, 1.0f
    };

    //* definition of cube face indices (for GL_TRIANGLE_STRIP --> 15)
    static GLushort indices[] =
    {
        // 4 sides
        3, 0, 2, 1, 
        6, 7, 
        5, 4, 
        3, 0,

        // floor
        0xFFFF,
        0, 4, 1, 7
    };
    INDICES_COUNT = sizeof(indices) / sizeof(indices[0]);
    glPrimitiveRestartIndex(0xFFFF);
    glEnable(GL_PRIMITIVE_RESTART);
    //*/


    // setup Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao); 
    glBindVertexArray(vao);

    // setup Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    //* setup local Elements Buffer Object (Indices)
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // setup Indices Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
    //*/

    // get position vertex attribute location
    // (requires that the shader program has been compiled already!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    
    // get color vertex attribute location
    // (requires that the shader program has been compiled already!)
    GLuint vecColor = glGetAttribLocation(PROGRAM_ID, "vecColor");
    glVertexAttribPointer(vecColor, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(vertices)));
    glEnableVertexAttribArray(vecColor);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // clear window background
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set rendering mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");

    // setup orthographic projection matrix
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
    glUniformMatrix4fv(PROJECTION_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));
}



void initMenuChange(int entry, char* name, int value)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    MENU_ENTRY = entry;
    MENU_VALUE = value;
    MENU_ENTRY_STR[entry].assign(name);
}



void glutUpdateMenuCB(int status, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    if (status == GLUT_MENU_NOT_IN_USE)
    {
        // menu not in use, time to make changes
        glutChangeToMenuEntry(MENU_ENTRY, const_cast<char*>(MENU_ENTRY_STR[MENU_ENTRY].c_str()), MENU_VALUE);
    }
}



void glutMenuCB1(int key)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (key)
    {
        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
        case 'w': case 'W': // toggle wireframe display
        {
            USE_WIREFRAME = !USE_WIREFRAME;
            if (USE_WIREFRAME)
            {
                glPolygonMode(POLYGON_MODE, GL_LINE);
                initMenuChange(1, "Disable Wireframe [W]", 'W');
            }
            else
            {
                glPolygonMode(POLYGON_MODE, GL_FILL);
                initMenuChange(1, "Enable Wireframe [W]", 'W');
            }
            break;
        }
        case 'c': case 'C': // toggle face culling
        {
            USE_CULLING = !USE_CULLING;
            if (USE_CULLING)
            {
                glEnable(GL_CULL_FACE);
                initMenuChange(2, "Disable Culling [C]", 'C');
            }
            else
            {
                glDisable(GL_CULL_FACE);
                initMenuChange(2, "Enable Culling [C]", 'C');
            }
            break;
        }
        case 'd': case 'D': // toggle depth test
        {
            USE_DEPTH_TEST = !USE_DEPTH_TEST;
            if (USE_DEPTH_TEST)
            { 
                glEnable(GL_DEPTH_TEST);
                initMenuChange(3, "Disable Depth Buffer [D]", 'D');
                glutSetWindowTitle("Basic Modeling (Depth Buffering Enabled)");
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
                initMenuChange(3, "Enable Depth Buffer [D]", 'D');
                glutSetWindowTitle("Basic Modeling (Depth Buffering Disabled)");
            }
            break;
        }
        case 'r': case 'R': default: // reset settings
        {
            cout << "Reset Settings to Default..." << endl;

            POLYGON_MODE = GL_FRONT_AND_BACK;
            glPolygonMode(POLYGON_MODE, GL_FILL);
            USE_WIREFRAME = false;
            initMenuChange(1, "Enable Wireframe [W]", 'W');
            glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);

            glDisable(GL_CULL_FACE);
            USE_CULLING = false;
            initMenuChange(2, "Enable Culling [C]", 'C');
            glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);

            glDisable(GL_DEPTH_TEST);
            glutSetWindowTitle("Basic Modeling");
            USE_DEPTH_TEST = false;
            initMenuChange(3, "Enable Depth Buffer [D]", 'D');
            glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);

            TrackBall::resetTransformation();
            break;
        }
    }
    glutPostRedisplay();
}



void glutMenuCB2(int gl_enum)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    POLYGON_MODE = gl_enum;
    GLint modes[2];
    glGetIntegerv(GL_POLYGON_MODE, modes);
    if (POLYGON_MODE == GL_FRONT)
    {
        glPolygonMode(GL_FRONT, modes[0]);
    }
    else
    {
        glPolygonMode(GL_BACK, modes[1]);
    }
    glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    int sub_menu;

    // create sub-menu
    sub_menu = glutCreateMenu(glutMenuCB2);
    glutAddMenuEntry("Use GL_FRONT Faces", GL_FRONT);
    glutAddMenuEntry("Use GL_BACK Faces", GL_BACK);
    glutAddMenuEntry("Use GL_FRONT_AND_BACK Faces", GL_FRONT_AND_BACK);

    // register callback and create menu
    glutCreateMenu(glutMenuCB1);
    glutAddMenuEntry("Enable Wireframe [W]", 'W');
    glutAddMenuEntry("Enable Culling [C]", 'C');
    glutAddMenuEntry("Enable Depth Buffer [D]", 'D');
    glutAddSubMenu("Set Polygon Face", sub_menu);
    glutAddMenuEntry("Reset Settings [R]", 'R');
    glutAddMenuEntry("Exit [Q] or [ESC]", 'Q');

    // attach menu to right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



void glutKeyboardCB(unsigned char key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // let the glutMenuCB() do the keyboard handling
    glutMenuCB1(static_cast<int>(key));
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // init GLUT/FLTK settings
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE); //not working with FLTK
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | FL_OPENGL3);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Basic Modeling");

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
    glutMenuStatusFunc(glutUpdateMenuCB);

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
        argv[1] = "../../glsl/modeling.vert";
        argv[2] = "../../glsl/modeling.frag";
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }

    // init application
    initRendering();
    initModel(WIDTH);
    initMenu();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
