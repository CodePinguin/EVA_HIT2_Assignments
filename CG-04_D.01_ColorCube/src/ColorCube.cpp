///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-04_D.01_ColorCube [Version 3.1 - FLTK-1.4.x / C++17 Update]                          //
///////////////////////////////////////////////////////////////////////////////////////////////////




// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <FL/glut.H>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>
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

glm::mat4 TheCameraView(1.0f);

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[4];

const float WIDTH = 4.0f; // width of cube
GLuint INDICES_COUNT = 0; 
bool USE_WIREFRAME = false;
GLfloat TRANSPARENCY = 1.0f;



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

    // set modelview transformation matrix in vertex shader
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));

    // render vertices and draw cube using glDrawElements with triangles
    glDrawElements(GL_TRIANGLES, INDICES_COUNT, GL_UNSIGNED_SHORT, 0);

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(float width)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // definition of cube vertices (x, y, z)
    float base = width / 2.0f;
    GLfloat cube_vertices[] =
    {
        -base, -base,  base, 1.0f,  // v0
         base, -base,  base, 1.0f,  // v1
         base,  base,  base, 1.0f,  // v2
        -base,  base,  base, 1.0f,  // v3
        -base, -base, -base, 1.0f,  // v4
        -base,  base, -base, 1.0f,  // v5
         base,  base, -base, 1.0f,  // v6
         base, -base, -base, 1.0f   // v7
    };

    // definition of cube colors, each vertex has its own color definition (RGB)
    GLfloat cube_colors[] =
    {
        0.0, 0.0, 1.0, 1.0, // v0 BLUE
        1.0, 0.0, 1.0, 1.0, // v1 MAGENTA (B+R)
        1.0, 1.0, 1.0, 1.0, // v2 WHITE
        0.0, 1.0, 1.0, 1.0, // v3 CYAN (B+G)
        0.0, 0.0, 0.0, 1.0, // v4 BLACK
        0.0, 1.0, 0.0, 1.0, // v5 GREEN
        1.0, 1.0, 0.0, 1.0, // v6 YELLOW (R+G)
        1.0, 0.0, 0.0, 1.0  // v7 RED
    };

    // definition of cube face indices (using GL_TRIANGLES --> 36)
    static GLushort cube_indices[] =
    {
        // side faces
        3, 0, 2,
        2, 0, 1,
        1, 7, 2,
        2, 7, 6,
        6, 7, 4,
        4, 5, 6,
        4, 0, 3,
        3, 5, 4,
        // top faces
        3, 2, 5,
        5, 2, 6,
        // bottom faces
        0, 4, 1,
        1, 4, 7
    };
    INDICES_COUNT = sizeof(cube_indices) / sizeof(cube_indices[0]);

    // create local Vertex Array Object (VAO) for cube setup //////////////////////////////////////
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // create local VBO for cube setup
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // create local EBO for cube setup
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // get position and color vertex attribute locations (requires compiled shader program!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    GLuint vecColor = glGetAttribLocation(PROGRAM_ID, "vecColor");


    // bind VAO for cube setup ////////////////////////////////////////////////////////////////////
    glBindVertexArray(vao);

    // setup VBO for cube position and color vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices) + sizeof(cube_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_vertices), cube_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_vertices), sizeof(cube_colors), cube_colors);

    // define cube position and color vertex attributes data format and enable them
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(vecColor, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(cube_vertices)));
    glEnableVertexAttribArray(vecColor);

    // setup EBO for cube face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(cube_indices), cube_indices);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set rendering mode
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // enable blending to use alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
    glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

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
                glPolygonMode(GL_FRONT, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT, GL_FILL);
            }
            break;
        }
        case '1': // increase alpha value
        {
            if (TRANSPARENCY < 1.0)
            {
                TRANSPARENCY += 0.1f;
                glBlendColor(TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, 1.0f);
            }
            break;
        }
        case '2': // decrease alpha value
        {
            if (TRANSPARENCY > 0.0)
            {
                TRANSPARENCY -= 0.1f;
                glBlendColor(TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, 1.0f);
            }
            break;
        }
        case 'r': case 'R': default: // reset settings
        {
            cout << "Reset Settings to Default..." << endl;

            glPolygonMode(GL_FRONT, GL_FILL);
            TRANSPARENCY = 1.0f;
            glBlendColor(TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, 1.0f);

            TrackBall::resetTransformation();
            break;
        }
    }

    cout << setiosflags(ios::fixed);
    cout << "Transparency: " << setprecision(1) << TRANSPARENCY << endl;

    glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // register callback and create menu
    glutCreateMenu(glutMenuCB);
    glutAddMenuEntry("Toggle Wireframe [W]", 'W');
    glutAddMenuEntry("Increase Alpha [1]", '1');
    glutAddMenuEntry("Decrease Alpha [2]", '2');
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
    glutCreateWindow("Color Cube");

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
        argc = 3;
        argv[0] = "";
        argv[1] = "../../glsl/colorcube.vert";
        argv[2] = "../../glsl/colorcube.frag";
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
