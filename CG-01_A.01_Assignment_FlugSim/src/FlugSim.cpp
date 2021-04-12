///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-03-A.01S_3DHouse Solution [Version 3.1 - FLTK-1.4.x / C++17 Update]            //
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

#include "../inc/Aircraft.h"

// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;
GLint COLOR_VEC3_LOCATION = 0;

GLenum POLYGON_MODE = GL_FRONT_AND_BACK;
bool USE_WIREFRAME = false;
bool USE_DEPTH_TEST = false;
bool USE_CULLING = false;

// import from Aircraft file later
float velocity = 0.0f;
float PITCH = 0.0f;
float ROLL = 0.0f;
float YAW = 0.0f;

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[4];

const float WIDTH = 5.0f;  // width of house (ground = width * 2)
const float HEIGHT = 3.0f; // height of roof

// TODO: declare two VAOs for seperate ground/shaft and house/roof drawing
GLuint VAO;

// TODO: add additional globals for shaft, house and roof indices count
GLuint GROUND_INDICES_COUNT = 0;
GLuint SHAFT_INDICES_COUNT = 0;
GLuint64 SHAFT_DRAW_OFFSET = 0;



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
    modelview = glm::translate(modelview, glm::vec3(velocity, 0.0f, 0.0f));
    modelview = glm::rotate(modelview, glm::radians<float>(ROLL), glm::vec3(1.0f, 0.0f, 0.0f));
    modelview = glm::rotate(modelview, glm::radians<float>(PITCH), glm::vec3(0.0f, 0.0f, 1.0f));
    modelview = glm::rotate(modelview, glm::radians<float>(YAW), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));


    // bind ground/shaft VAO to current drawing context
    glBindVertexArray(VAO);

    // indexed drawing of ground
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(-0.01f, 1.0f);
    glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);
    //glDisable(GL_POLYGON_OFFSET_FILL);

    // TODO: add indexed drawing of shaft
    bool reenable = false;  // local variable to re-enable culling 

    if (glIsEnabled(GL_CULL_FACE)) // shaft must not be culled
    {
        glDisable(GL_CULL_FACE);
        reenable = true;
    }

    // indexed drawing of shaft
    glDrawElements(GL_TRIANGLE_STRIP, SHAFT_INDICES_COUNT, GL_UNSIGNED_SHORT, GL_BUFFER_OFFSET(SHAFT_DRAW_OFFSET));

    if (reenable) // re-enable culling for rest of model
    {
        glEnable(GL_CULL_FACE);
        reenable = false;
    }

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(float width, float height)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // definition of ground/shaft vertices (ground = width * 2)
    GLfloat ground_vertices[] =
    {
        //-width,  width, 0.0f, 1.0f,  // v0
        //-width,   0.0f, 0.0f, 1.0f,  // v1
        //-width, -width, 0.0f, 1.0f,  // v2
        // width,   0.0f, 0.0f, 1.0f,  // v3

        //// TODO: add shaft verices
        //-width,   0.0f, -1.0f, 1.0f,  // v1

        -width,  0.0f, -width, 1.0f,
        -width,  0.0f,   0.0f, 1.0f,
        -width,  0.0f,  width, 1.0f,
         width,  0.0f,   0.0f, 1.0f,
        -width, -1.5f,   0.0f, 1.0f
    };

    // definition of ground/shaft colors, each vertex has its own color definition (RGB)
    GLfloat ground_colors[] =
    {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.9f, 0.9f, 0.9f
    };

    // definition of ground face indices (using GL_TRIANGLES --> 6)
    GLushort ground_indices[] =
    {
        0, 1, 3,
        2, 1, 3
    };
    GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);
    SHAFT_DRAW_OFFSET = sizeof(ground_indices);

    // TODO: add definition of shaft face indices (using GL_TRIANGLE_STRIP --> 10)
    GLushort shaft_indices[] =
    {
        4, 1, 3
    };
    SHAFT_INDICES_COUNT = sizeof(shaft_indices) / sizeof(shaft_indices[0]);


    // TODO: create two global VAOs for seperate ground/shaft and house/roof drawing //////////////
    glGenVertexArrays(1, &VAO);

    // TODO: create two local VBOs for seperate ground/shaft and house/roof setup
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // TODO: create two local EBOs for seperate ground/shaft and house/roof setup
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // get position and color vertex attribute locations (requires compiled shader program!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    COLOR_VEC3_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");


    // bind VAO for ground/shaft setup ////////////////////////////////////////////////////////////
    glBindVertexArray(VAO);

    // setup VBO for ground/shaft position and color vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_colors), ground_colors);

    // define ground/shaft position and color vertex attributes data format and enable them
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(COLOR_VEC3_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(ground_vertices)));
    glEnableVertexAttribArray(COLOR_VEC3_LOCATION);

    // setup EBO for ground/shaft face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices) + sizeof(shaft_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ground_indices), ground_indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, SHAFT_DRAW_OFFSET, sizeof(shaft_indices), shaft_indices);


   
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // clear window background
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



void glutMenuCB(int key)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (key)
    {
        case 32: //handles spacebar
        {
            velocity += 1.0f;
            break;
        }

        // pitch around x-axis
        case 'w': case 'W':
        {
            PITCH += 10.0f;
            break;
        }

        case 's': case 'S':
        {
            PITCH -= 10.0f;
            break;
        }

        // roll around y-axis
        case 'a': case 'A':
        {
            ROLL += 10.0f;
            break;
        }

        case 'd': case 'D':
        {
            ROLL -= 10.0f;
            break;
        }

        // yaw around z-axis
        case 'o': case 'O':
        {
            YAW += 10.0f;
            break;
        }

        case 'p': case 'P':
        {
            YAW -= 10.0f;
            break;
        }

        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
        
        // Reset Settings
        case 'r': case 'R': default:
        {
            cout << "Reset Settings to Default..." << endl;

            PITCH = 0.0f;
            ROLL = 0.0f;
            YAW = 0.0f;
            velocity = 0.0f;

            TrackBall::resetTransformation();
            break;
        }
    }
    glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);
    glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    int sub_menu;

    // register callback and create menu
    glutCreateMenu(glutMenuCB);
    glutAddMenuEntry("Pitch + [W]", 'W');
    glutAddMenuEntry("Pitch - [S]", 'S');
    glutAddMenuEntry("Roll + [A]", 'A');
    glutAddMenuEntry("Roll - [D]", 'D');
    glutAddMenuEntry("Yaw + [O]", 'O');
    glutAddMenuEntry("Yaw - [P]", 'P');
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
    /* Airplaneairplane = Airplane();

    if (keypressed W) {
        airplane.IncreaseVelo();
    }*/


    // init GLUT/FLTK settings
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Flight Simulator");

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
        argv[1] = "../../glsl/FlugSim.vert";
        argv[2] = "../../glsl/FlugSim.frag";
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }

    // init application
    initRendering();
    initModel(WIDTH, HEIGHT);
    initMenu();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
