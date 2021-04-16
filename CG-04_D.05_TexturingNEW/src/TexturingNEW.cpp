///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-04_D.05_Texturing [Version 3.1 - FLTK-1.4.x / C++17 Update]                          //
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
#include "../../_COMMON/inc/UtilImage.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;
GLint TEXTURE_MAT4_LOCATION = 0;
GLint COLOR_VEC3_LOCATION = 0;

glm::mat4 TheCameraView(1.0f);

GLuint TEX_NAME;

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[3];


GLuint VAO;
GLuint GROUND_INDICES_COUNT = 0;



void initModels(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    float width = 0.3f;
    GLfloat ground_vertices[] =
    {
        -width, -width-1,  width, 1.0f,  // v0
         width, -width-1,  width, 1.0f,  // v1
         width, -width-1, -width, 1.0f,  // v2
        -width, -width-1, -width, 1.0f,  // v3
    };

    GLfloat ground_colors[] =
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    float ground_tex[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    GLushort ground_indices[] =
    {
        0, 3, 2, 1, 0, 2,
    };

    GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);

    glGenVertexArrays(1, &VAO);

    // create local VBOs
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // create local EBOs
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // get position and color vertex attribute locations (requires compiled shader program!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    COLOR_VEC3_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");


    // bind VAO
    glBindVertexArray(VAO);

    unsigned int handle[4];
    glGenBuffers(4, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), ground_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), ground_colors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(1);  // Vertex normal

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), ground_tex, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(2);  // texture coords

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), ground_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}


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


    
    

    //////////////
    //  Ground  //
    //////////////
    
    // setup texture matrix
    glm::mat4 texture_matrix(1.0f);

    // bind currently selected texture
    glBindTexture(GL_TEXTURE_2D, TEX_NAME);

    // setup texture matrix
    texture_matrix = glm::scale(texture_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(TEXTURE_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(modelview));
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);
    
    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}


void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // set background color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set rendering mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // define fixed light source position and properties (in eye space coordinates)
    glm::vec4 position(0.0f, 0.0f, 20.0f, 1.0f);
    glm::vec4  ambient(0.8f, 0.8f,  0.8f, 1.0f);
    glm::vec4  diffuse(0.8f, 0.8f,  0.8f, 1.0f);
    glm::vec4 specular(1.0f, 1.0f,  1.0f, 1.0f);

    // define material properties
    glm::vec4  mat_ambient(0.1f, 0.1f, 0.1f, 1.0f);
    glm::vec4  mat_diffuse(0.1f, 0.1f, 0.9f, 1.0f);
    glm::vec4 mat_specular(1.0f, 1.0f, 1.0f, 1.0f);
    GLfloat  mat_shininess = 64.0f;

    // setup Uniform Buffer Objects (UBO)
    GLuint ubo[2];
    glGenBuffers(2, &ubo[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 4, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(specular));

    glBindBuffer(GL_UNIFORM_BUFFER, ubo[1]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3 + sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mat_ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mat_diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mat_specular));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(GLfloat), &mat_shininess);


    // bind uniform buffer objects to uniform blocks using the shader provided binding point
    GLuint ubo_index = 0;
    GLint ubo_binding = 0;

    ubo_index = glGetUniformBlockIndex(PROGRAM_ID, "LightProperties");
    glGetActiveUniformBlockiv(PROGRAM_ID, ubo_index, GL_UNIFORM_BLOCK_BINDING, &ubo_binding);
    glBindBufferBase(GL_UNIFORM_BUFFER, ubo_binding, ubo[0]);

    ubo_index = glGetUniformBlockIndex(PROGRAM_ID, "MaterialProperties");
    glGetActiveUniformBlockiv(PROGRAM_ID, ubo_index, GL_UNIFORM_BLOCK_BINDING, &ubo_binding);
    glBindBufferBase(GL_UNIFORM_BUFFER, ubo_binding, ubo[1]);

    GLint location = glGetUniformLocation(PROGRAM_ID, "ReplaceColor");
    glUniform1i(location, true);

    // load textures from files
    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    UtilImage::loadPNGTexture("../../res/yoda_256.png", &TEX_NAME); // Pointer to image data
    //UtilImage::loadPNGTexture("../../res/metal_256.png", &TEX_NAME);

    // setup the camera view matrix
    TheCameraView = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");

    // get texture matrix uniform location
    TEXTURE_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");
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
        case 'm': case 'M': // toggle modulation/replacement of material color
        {
            GLint location = glGetUniformLocation(PROGRAM_ID, "ReplaceColor");
            GLint replace_color; // there is no glGetUniform for boolean types (use int)
            glGetUniformiv(PROGRAM_ID, location, &replace_color);
            glUniform1i(location, !replace_color);
            break;
        }
        case 'r': case 'R': default: // reset settings
        {
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
    // register callback and create menu
    glutCreateMenu(glutMenuCB);
    glutAddMenuEntry("Toggle Color Modulation [M]", 'M');
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
    glutCreateWindow("Texturing");

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
        argv[1] = "../../glsl/texturingNEW.vert";
        argv[2] = "../../glsl/texturingNEW.frag";
        PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
    }

    // init application
    initRendering();
    initModels();
    initMenu();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
