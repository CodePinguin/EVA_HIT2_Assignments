///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-03-A.01_3DHouse [Version 3.1 - FLTK-1.4.x / C++17 Update]                      //
//             (Non-Indexd drawing not supported on Apple)                                       //
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

const float WIDTH = 5.0f;  // ground = width * 2
const float HEIGHT = 3.0f; // height of roof

// TODO: declare two VAOs for drawing ground/fence and house/roof models seperately
GLuint GROUND_FENCE_VAO = 0;
GLuint HOUSE_ROOF_VAO = 0;

// TODO: add additional globals for fence, house and roof indices count
GLuint GROUND_INDICES_COUNT = 0;
GLuint FENCE_INDICES_COUNT = 0;
GLuint HOUSE_INDICES_COUNT = 0;
GLuint ROOF_INDICES_COUNT = 0;



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

    // bind ground and fence Vertex Array Object to current drawing context
    glBindVertexArray(GROUND_FENCE_VAO);

    // indexed drawing of ground
    glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);

    // TODO: add indexed drawing of fence
    glDrawElements(GL_TRIANGLES, FENCE_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);

    // TODO: add indexed drawing of house and roof
    glBindVertexArray(HOUSE_ROOF_VAO);
    glDrawElements(GL_TRIANGLES, HOUSE_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);
    glDrawElements(GL_TRIANGLES, ROOF_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);


    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(float width, float height)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // define ground and fence vertices
    GLfloat ground_vertices[] =
    {
        -width, -width, 0.0f, 1.0f,  // v0 ground
         width, -width, 0.0f, 1.0f,  // v1
         width,  width, 0.0f, 1.0f,  // v2
        -width,  width, 0.0f, 1.0f,  // v3

        //-width, -width, 0.0f, 1.0f,  // v4 fence boden, für bessere Farbdarstellung
        // width, -width, 0.0f, 1.0f,  // v5
        // width,  width, 0.0f, 1.0f,  // v6
        //-width,  width, 0.0f, 1.0f,  // v7

        -width, -width, 1.0f, 1.0f,  // v8 fence
         width, -width, 1.0f, 1.0f,  // v9
         width,  width, 1.0f, 1.0f,  // v10
        -width,  width, 1.0f, 1.0f,  // v11
    // TODO: add fence vertices
    };

    // define ground and fence colors, each vertex has his own color definition (RGB)
    GLfloat ground_colors[] =
    {
        0.0f, 0.4f, 0.0f, // ground
        0.0f, 0.4f, 0.0f,
        0.0f, 0.4f, 0.0f,
        0.0f, 0.4f, 0.0f,

        //0.8f, 0.5f, 0.0f, // fence
        //0.8f, 0.5f, 0.0f,
        //0.8f, 0.5f, 0.0f,
        //0.8f, 0.5f, 0.0f,

        0.8f, 0.5f, 0.0f, // fence
        0.8f, 0.5f, 0.0f,
        0.8f, 0.5f, 0.0f,
        0.8f, 0.5f, 0.0f
    // TODO: add fence colors
    };

    // definition of ground face indices (for GL_TRIANGLES --> 6)
    GLushort ground_indices[] =
    {
        0, 3, 2,
        1, 0, 2
    };
    GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);

// TODO: add definition of fence indices
    GLushort fence_indices[] =
    {
        /*4, 8, 9,
        5, 4, 9,
        5, 9, 10,
        6, 5, 10,
        6, 10, 11,
        7, 6, 11,
        7, 11, 8,
        7, 4, 8*/

        0, 4, 5,
        1, 0, 5,
        1, 5, 6,
        2, 1, 6,
        2, 6, 7,
        3, 2, 7,
        3, 7, 4,
        3, 0, 4
    };
    FENCE_INDICES_COUNT = sizeof(fence_indices) / sizeof(fence_indices[0]);


// TODO: add definition of house and roof vertices
    float side = width / 2.0f;
    GLfloat house_vertices[] =
    {
        -side, -side, 0.0f, 1.0f,  // v0 house down
         side, -side, 0.0f, 1.0f,  // v1
         side,  side, 0.0f, 1.0f,  // v2
        -side,  side, 0.0f, 1.0f,  // v3

        -side, -side, 5.0f, 1.0f,  // v4 house up
         side, -side, 5.0f, 1.0f,  // v5
         side,  side, 5.0f, 1.0f,  // v6
        -side,  side, 5.0f, 1.0f,  // v7

         0.0f, 0.0f, 7.0f, 1.0f //v12 roof
    };


// TODO: add definition of house and roof colors
    GLfloat house_colors[] =
    {
        0.0f, 0.5f, 0.5f, // house
        0.0f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f,

        0.0f, 0.5f, 0.5f, 
        0.0f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f,

        0.0f, 0.0f, 1.0f  // roof
    };


// TODO: add definition of house face indices
    GLushort house_indices[] =
    {
        0, 3, 2,
        1, 0, 2,
        1, 5, 6,
        2, 1, 6, 
        2, 6, 7,
        3, 2, 7,
        3, 7, 4,
        0, 3, 4,
        0, 4, 5,
        1, 0, 5,
    };
    HOUSE_INDICES_COUNT = sizeof(house_indices) / sizeof(house_indices[0]);


// TODO: add definition of roof face indices
    GLushort roof_indices[] =
    {
        4, 8, 5,
        5, 8, 6,
        6, 8, 7,
        7, 8, 4
    };
    ROOF_INDICES_COUNT = sizeof(roof_indices) / sizeof(roof_indices[0]);

    // create global Vertex Array Objects /////////////////////////////////////////////////////////
// TODO: create two VAOs for drawing ground/fence and house/roof models seperately
    glGenVertexArrays(1, &GROUND_FENCE_VAO);
    glGenVertexArrays(1, &HOUSE_ROOF_VAO);

    // create local vertex buffer objects
// TODO: create two vbos for drawing ground/fence and house/roof models seperately
    GLuint GROUND_FENCE_VBO;
    GLuint HOUSE_ROOF_VBO;
    glGenBuffers(1, &GROUND_FENCE_VBO);
    glGenBuffers(1, &HOUSE_ROOF_VBO);

    // create local element buffer objects
// TODO: create two ebos for drawing ground/fence and house/roof models seperately
    GLuint GROUND_FENCE_EBO;
    GLuint HOUSE_ROOF_EBO;

    glGenBuffers(1, &GROUND_FENCE_EBO);
    glGenBuffers(1, &HOUSE_ROOF_EBO);

    // get location of position and color vertex attributes
    // (requires that the shader program has been compiled already!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    GLuint vecColor = glGetAttribLocation(PROGRAM_ID, "vecColor");


    // bind Vertex Array Object for ground and fence //////////////////////////////////////////////
    glBindVertexArray(GROUND_FENCE_VAO);

    // setup vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, GROUND_FENCE_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_colors), ground_colors);

    // setup vertex attributes
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(vecColor, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(ground_vertices)));
    glEnableVertexAttribArray(vecColor);

    // setup element buffer object (indices) for ground and fence
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GROUND_FENCE_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices) + sizeof(fence_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ground_indices), ground_indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices), sizeof(fence_indices), fence_indices);



    // TODO: bind vertex array object for house and roof
     glBindVertexArray(HOUSE_ROOF_VAO);

    // TODO: setup Vertex buffer object for house and roof
    glBindBuffer(GL_ARRAY_BUFFER, HOUSE_ROOF_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(house_vertices) + sizeof(house_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(house_vertices), house_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(house_vertices), sizeof(house_colors), house_colors);

    // setup vertex attributes (house and roof)
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(vecColor, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(house_vertices)));
    glEnableVertexAttribArray(vecColor);

    // setup element buffer object (indices) for house and roof
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HOUSE_ROOF_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(house_indices) + sizeof(roof_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(house_indices), house_indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(house_indices), sizeof(roof_indices), roof_indices);
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
