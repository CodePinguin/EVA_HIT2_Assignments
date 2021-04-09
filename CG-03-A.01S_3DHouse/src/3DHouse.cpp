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


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;
GLint COLOR_VEC3_LOCATION = 0;

GLenum POLYGON_MODE = GL_FRONT_AND_BACK;
bool USE_WIREFRAME = false;
bool USE_DEPTH_TEST = false;
bool USE_CULLING = false;

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[4];

const float WIDTH = 5.0f;  // width of house (ground = width * 2)
const float HEIGHT = 3.0f; // height of roof

// TODO: declare two VAOs for seperate ground/fence and house/roof drawing
GLuint VAO[2];

// TODO: add additional globals for fence, house and roof indices count
GLuint GROUND_INDICES_COUNT = 0;
GLuint FENCE_INDICES_COUNT = 0;
GLuint64 FENCE_DRAW_OFFSET = 0;

GLuint HOUSE_INDICES_COUNT = 0;
GLuint ROOF_INDICES_COUNT = 0;
GLuint64 ROOF_DRAW_OFFSET = 0;



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
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));

    // bind ground/fence VAO to current drawing context
    glBindVertexArray(VAO[0]);

    // indexed drawing of ground
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(-0.01f, 1.0f);
    glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);
    //glDisable(GL_POLYGON_OFFSET_FILL);

    // TODO: add indexed drawing of fence
    bool reenable = false;  // local variable to re-enable culling 

    if (glIsEnabled(GL_CULL_FACE)) // fence must not be culled
    {
        glDisable(GL_CULL_FACE);
        reenable = true;
    }

    // indexed drawing of fence
    glDrawElements(GL_TRIANGLE_STRIP, FENCE_INDICES_COUNT, GL_UNSIGNED_SHORT, GL_BUFFER_OFFSET(FENCE_DRAW_OFFSET));

    if (reenable) // re-enable culling for rest of model
    {
        glEnable(GL_CULL_FACE);
        reenable = false;
    }


    // TODO: add indexed drawing of house/roof
    // bind house/roof VAO to current drawing context
    glBindVertexArray(VAO[1]);

    // disable color vertex attribute to set default color for house drawing
    glDisableVertexAttribArray(COLOR_VEC3_LOCATION);

    // indexed drawing of house
    glDrawElements(GL_TRIANGLE_STRIP, HOUSE_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);

    // re-enable color vertex attribute to use color array for roof drawing
    glEnableVertexAttribArray(COLOR_VEC3_LOCATION); 

    // indexed drawing of roof
    glDrawElements(GL_TRIANGLE_FAN, ROOF_INDICES_COUNT, GL_UNSIGNED_SHORT, GL_BUFFER_OFFSET(ROOF_DRAW_OFFSET));

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(float width, float height)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // definition of ground/fence vertices (ground = width * 2)
    GLfloat ground_vertices[] =
    {
        -width, -width, 0.0f, 1.0f,  // v0
         width, -width, 0.0f, 1.0f,  // v1
         width,  width, 0.0f, 1.0f,  // v2
        -width,  width, 0.0f, 1.0f,  // v3
        // TODO: add fence verices
        -width, -width, -2.0f, 1.0f,  // v4
        -width,  width, -2.0f, 1.0f,  // v5
         width,  width, -2.0f, 1.0f,  // v6
         width, -width, -2.0f, 1.0f   // v7
    };

    // definition of ground/fence colors, each vertex has its own color definition (RGB)
    GLfloat ground_colors[] =
    {
        0.0f, 0.4f, 0.0f,
        0.0f, 0.4f, 0.0f,
        0.0f, 0.4f, 0.0f,
        0.0f, 0.4f, 0.0f,
        // TODO: add fence colors
        0.0f, 0.6f, 0.0f,
        0.0f, 0.7f, 0.0f,
        0.0f, 0.8f, 0.0f,
        0.0f, 0.9f, 0.0f
    };

    // definition of ground face indices (using GL_TRIANGLES --> 6)
    GLushort ground_indices[] =
    {
        0, 3, 2,
        1, 0, 2
    };
    GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);
    FENCE_DRAW_OFFSET = sizeof(ground_indices);

    // TODO: add definition of fence face indices (using GL_TRIANGLE_STRIP --> 10)
    GLushort fence_indices[] =
    {
        0, 4, 1,
        7, 2, 6,
        3, 5,
        0, 4
    };
    FENCE_INDICES_COUNT = sizeof(fence_indices) / sizeof(fence_indices[0]);

    // TODO: add definition of house/roof vertices
    float base = width / 2.0f;
    GLfloat house_vertices[] =
    {
        -base, -base,    0.0f, 1.0f,  // v0
         base, -base,    0.0f, 1.0f,  // v1
         base,  base,    0.0f, 1.0f,  // v2
        -base,  base,    0.0f, 1.0f,  // v3

        -base, -base, -width, 1.0f,  // v4
        -base,  base, -width, 1.0f,  // v5
         base,  base, -width, 1.0f,  // v6
         base, -base, -width, 1.0f,  // v7

         0.0f,  0.0f, -width-height, 1.0f  // v8
    };

    // TODO: add definition of house/roof colors, each vertex has its own color definition (RGB)
    GLfloat house_colors[] =
    {
        1.0f, 1.0f, 0.0f,
        0.9f, 0.9f, 0.0f,
        0.7f, 0.7f, 0.0f,
        0.6f, 0.6f, 0.0f,

        0.0f, 0.0f, 0.3f,
        0.0f, 0.0f, 0.4f,
        0.0f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.6f,

        0.0f, 0.0f, 1.0f
    };

    // TODO: add definition of house face indices (using GL_TRIANGLE_STRIP --> 10)
    GLushort house_indices[] =
    {
        // 4 walls
        0, 4, 1, 7,
        2, 6,
        3, 5,
        0, 4

        /* draw bottom of house to demo z-buffer fighting
        ,0xFFFF,
        0, 1, 3, 2
        //*/
    };
    //glPrimitiveRestartIndex(0xFFFF);
    //glEnable(GL_PRIMITIVE_RESTART);
    HOUSE_INDICES_COUNT = sizeof(house_indices) / sizeof(house_indices[0]);
    ROOF_DRAW_OFFSET = sizeof(house_indices);

    // TODO: add definition of roof face indices (using GL_TRIANGLE_FAN --> 6)
    GLushort roof_indices[] =
    {
        8, 4, 5, 6, 7, 4
    };
    ROOF_INDICES_COUNT = sizeof(roof_indices) / sizeof(roof_indices[0]);


    // TODO: create two global VAOs for seperate ground/fence and house/roof drawing //////////////
    glGenVertexArrays(2, &VAO[0]);

    // TODO: create two local VBOs for seperate ground/fence and house/roof setup
    GLuint vbo[2];
    glGenBuffers(2, &vbo[0]);

    // TODO: create two local EBOs for seperate ground/fence and house/roof setup
    GLuint ebo[2];
    glGenBuffers(2, &ebo[0]);

    // get position and color vertex attribute locations (requires compiled shader program!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    COLOR_VEC3_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");


    // bind VAO for ground/fence setup ////////////////////////////////////////////////////////////
    glBindVertexArray(VAO[0]);

    // setup VBO for ground/fence position and color vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_colors), ground_colors);

    // define ground/fence position and color vertex attributes data format and enable them
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(COLOR_VEC3_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(ground_vertices)));
    glEnableVertexAttribArray(COLOR_VEC3_LOCATION);

    // setup EBO for ground/fence face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices) + sizeof(fence_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ground_indices), ground_indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, FENCE_DRAW_OFFSET, sizeof(fence_indices), fence_indices);


    // TODO: bind VAO for house/roof setup ////////////////////////////////////////////////////////
    glBindVertexArray(VAO[1]);

    // TODO: setup VBO for house/roof position and color vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(house_vertices) + sizeof(house_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(house_vertices), house_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(house_vertices), sizeof(house_colors), house_colors);

    // define color vertex attribute default draw color if array is disabled
    glVertexAttrib3f(COLOR_VEC3_LOCATION, 1.0f, 1.0f, 0.0f);

    // TODO: define house/roof vertex attributes data format and enable them
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(COLOR_VEC3_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(house_vertices)));
    glEnableVertexAttribArray(COLOR_VEC3_LOCATION);

    // TODO: setup EBO for house/roof face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(house_indices) + sizeof(roof_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(house_indices), house_indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, ROOF_DRAW_OFFSET, sizeof(roof_indices), roof_indices);
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
        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
        case 'c': case 'C': // toggle face culling
        {
            USE_CULLING = !USE_CULLING;
            if (USE_CULLING)
            {
                glEnable(GL_CULL_FACE);
                initMenuChange(1, "Disable Culling [C]", 'C');
            }
            else
            {
                glDisable(GL_CULL_FACE);
                initMenuChange(1, "Enable Culling [C]", 'C');
            }
            break;
        }
        case 'd': case 'D': // toggle depth test
        {
            USE_DEPTH_TEST = !USE_DEPTH_TEST;
            if (USE_DEPTH_TEST)
            { 
                glEnable(GL_DEPTH_TEST);
                initMenuChange(2, "Disable Depth Buffer [D]", 'D');
                glutSetWindowTitle("Basic Modeling (Depth Buffering Enabled)");
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
                initMenuChange(2, "Enable Depth Buffer [D]", 'D');
                glutSetWindowTitle("Basic Modeling (Depth Buffering Disabled)");
            }
            break;
        }
        case 'w': case 'W': // toggle wireframe display
        {
            USE_WIREFRAME = !USE_WIREFRAME;
            if (USE_WIREFRAME)
            {
                glPolygonMode(POLYGON_MODE, GL_LINE);
                initMenuChange(3, "Disable Wireframe [W]", 'W');
            }
            else
            {
                glPolygonMode(POLYGON_MODE, GL_FILL);
                initMenuChange(3, "Enable Wireframe [W]", 'W');
            }
            break;
        }
        case 'r': case 'R': default: // reset settings
        {
            cout << "Reset Settings to Default..." << endl;

            glDisable(GL_CULL_FACE);
            USE_CULLING = false;
            initMenuChange(1, "Enable Culling [C]", 'C');
            glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);

            glDisable(GL_DEPTH_TEST);
            glutSetWindowTitle("Basic Modeling");
            USE_DEPTH_TEST = false;
            initMenuChange(2, "Enable Depth Buffer [D]", 'D');
            glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);

            POLYGON_MODE = GL_FRONT_AND_BACK;
            glPolygonMode(POLYGON_MODE, GL_FILL);
            USE_WIREFRAME = false;
            initMenuChange(3, "Enable Wireframe [W]", 'W');
            glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);

            TrackBall::resetTransformation();
            break;
        }
    }
    glutUpdateMenuCB(GLUT_MENU_NOT_IN_USE, 0, 0);
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
    glutCreateMenu(glutMenuCB);
    glutAddMenuEntry("Enable Culling [C]", 'C');
    glutAddMenuEntry("Enable Depth Buffer [D]", 'D');
    glutAddMenuEntry("Enable Wireframe [W]", 'W');
    glutAddSubMenu("Set Wireframe Mode", sub_menu);
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
    glutCreateWindow("3D House");

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
        argv[1] = "../../glsl/house.vert";
        argv[2] = "../../glsl/house.frag";
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
