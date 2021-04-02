///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-03-D.02_Pyramid [Version 3.1 - FLTK-1.4.x / C++17 Update]                            //
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
#include <stack>
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

const float WIDTH = 5.0f;   
const float HEIGHT = 4.5f;

GLuint VA_COLOR_LOCATION = 0;

GLuint GROUND_INDICES_COUNT = 0;
GLuint PYRAMID_INDICES_COUNT = 0;
GLuint64 PYRAMID_DRAW_OFFSET = 0;



void drawPyramid(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // disable color vertex attribute to set default color for ground drawing
    glDisableVertexAttribArray(VA_COLOR_LOCATION);
    // indexed drawing of ground
    glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);
    // re-enable color vertex attribute to use color array for pyramid drawing
    glEnableVertexAttribArray(VA_COLOR_LOCATION);
    // indexed drawing of pyramid
    glDrawElements(GL_TRIANGLE_FAN, PYRAMID_INDICES_COUNT, GL_UNSIGNED_SHORT, GL_BUFFER_OFFSET(PYRAMID_DRAW_OFFSET));
}



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup modelview matrix
    glm::mat4 modelview(1.0f);
    stack<glm::mat4> modelview_stack;

    // apply trackball transformation to modelview matrix
    glm::mat4 mouse = TrackBall::getTransformation();
    modelview = modelview * mouse;

    // rotate coordinate system 90 degree around x-axis
    modelview = glm::rotate(modelview, glm::radians<float>(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelview = modelview * mouse;

    // draw pyramids with offset around origin
    float offset = WIDTH / 2.0f;
    modelview_stack.push(modelview);   
        modelview = glm::translate(modelview, glm::vec3(offset, offset, 0.0));
        glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));
        drawPyramid();
        modelview = modelview_stack.top();

        modelview = glm::translate(modelview, glm::vec3(-offset, offset, 0.0));
        glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));
        drawPyramid();
        modelview = modelview_stack.top();

        modelview = glm::translate(modelview, glm::vec3(-offset, -offset, 0.0));
        glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));
        drawPyramid();
        modelview = modelview_stack.top();

        modelview = glm::translate(modelview, glm::vec3(offset, -offset, 0.0));
        glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));
        drawPyramid();
        modelview = modelview_stack.top();

        modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, -HEIGHT));
        glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));
        drawPyramid();
        modelview = modelview_stack.top();

    modelview_stack.pop();

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}



void initModel(float width, float height)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    float base = width / 2.0f;

    // define pyramid vertices
    GLfloat pyramid_vertices[] =
    {
        -base, -base, 0.0f, 1.0f,  // v0
         base, -base, 0.0f, 1.0f,  // v1
         base,  base, 0.0f, 1.0f,  // v2
        -base,  base, 0.0f, 1.0f,  // v3

         0.0f,  0.0f,  -height, 1.0f  // v4
    };

    // definition of the pyramid colors, each vertex has his own color definition (RGB)
    GLfloat pyramid_colors[] =
    {
        0.0f, 0.0f, 0.2f,
        0.0f, 0.0f, 0.4f,
        0.0f, 0.0f, 0.6f,
        0.0f, 0.0f, 0.8f,
        0.0f, 0.0f, 1.0f
    };

    // definition of ground face indices (for GL_TRIANGLES --> 6)
    GLushort ground_indices[] =
    {
        0, 1, 2,
        3, 0, 2
    };
    GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);
    PYRAMID_DRAW_OFFSET = sizeof(ground_indices);

    // definition of pyramid face indices (for GL_TRIANGLE_FAN --> 6)
    GLushort pyramid_indices[] =
    {
        4, 0, 3, 2, 1, 0
    };
    PYRAMID_INDICES_COUNT = sizeof(pyramid_indices) / sizeof(pyramid_indices[0]);


    // setup Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao); 

    // setup local Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // setup local Index Buffer Object
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // get position and color vertex attributes location
    // (requires that the shader program has been compiled already!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    VA_COLOR_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");


    // setup Vertex Array Object for ground and pyramid
    glBindVertexArray(vao);

    // setup Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices) + sizeof(pyramid_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid_vertices), pyramid_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), sizeof(pyramid_colors), pyramid_colors);

    // setup Vertex Attributes
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glVertexAttribPointer(VA_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(pyramid_vertices)));
    glVertexAttrib3f(VA_COLOR_LOCATION, 0.4f, 0.4f, 1.0f); // set default draw vertex color if array is disabled
    glEnableVertexAttribArray(vecPosition);
    glEnableVertexAttribArray(VA_COLOR_LOCATION);

    // setup Indices Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices) + sizeof(pyramid_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ground_indices), ground_indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, PYRAMID_DRAW_OFFSET, sizeof(pyramid_indices), pyramid_indices);
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



void glutKeyboardCB(unsigned char key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    switch (key)
    {
        case 'q': case 'Q': case 27: // handle escape keys and exit
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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 640);
    glutCreateWindow("Pyramid");

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
        argv[1] = "../../glsl/pyramid.vert";
        argv[2] = "../../glsl/pyramid.frag";
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }

    // init application
    initRendering();
    initModel(WIDTH, HEIGHT);

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
