///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: Flight Simulator by Celina Vetter                                                 //
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

#include "../inc/Aircraft.h"

// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MODELVIEW_MAT4_LOCATION = 0;
GLint PROJECTION_MAT4_LOCATION = 0;
GLint TEXTURE_MAT4_LOCATION = 0;
GLint COLOR_VEC3_LOCATION = 0;


int FPS = 50;
float t = 0.0f;

glm::mat4 TheCameraView(1.0f);

GLuint TEX_NAME;

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[3];

float windowLength = 1000.0f;

// VAO
GLuint VAO[2];

// additional globals for indices count
GLuint GROUND_INDICES_COUNT = 0;
GLuint PLANE_INDICES_COUNT = 0;


Aircraft aircraft;


void initModel(float windowLength)
///////////////////////////////////////////////////////////////////////////////////////////////////
{   
    // definition of plane vertices
    float width = 50.0f;
    GLfloat plane_vertices[] =
    {
        -width,  0.0f,  -width, 1.0f, //v0
        -width,  0.0f,    0.0f, 1.0f, //v1
        -width,  0.0f,   width, 1.0f, //v2
         width,  0.0f,    0.0f, 1.0f, //v3
        -width, -width/2, 0.0f, 1.0f  //v4
    };


    // definition of ground colors, each vertex has its own color definition (RGB)
    GLfloat plane_colors[] =
    {
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f 
    };

    // definition of plane face indices
    GLushort plane_indices[] =
    {
        0, 1, 3,
        2, 1, 3,
        4, 1, 3
    };
    PLANE_INDICES_COUNT = sizeof(plane_indices) / sizeof(plane_indices[0]);

    /////////////////////////////////////////////////////////////////////////////////////////
    // define ground vertices
    float length = windowLength* 2/3;
    GLfloat ground_vertices[] =
    {
        -length,  -windowLength,   length, 1.0f, //v0
         length,  -windowLength,   length, 1.0f, //v1
         length,  -windowLength,  -length, 1.0f, //v2
        -length,  -windowLength,  -length, 1.0f  //v3          
    };

    GLfloat ground_colors[] =
    {
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
        0, 3, 2,
        1, 0, 2
    };
    GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);

    glGenVertexArrays(1, &VAO[0]);

    // create local VBOs
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // create local EBOs
    GLuint ebo;
    glGenBuffers(1, &ebo);
    

    glGenVertexArrays(1, &VAO[0]);
    // get position and color vertex attribute locations (requires compiled shader program!)
    GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
    COLOR_VEC3_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");

    glGenVertexArrays(1, &VAO[0]);

    // bind VAO for plane setup ////////////////////////////////////////////////////////////
    glBindVertexArray(VAO[0]);

    // setup VBO for plane position and color vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices) + sizeof(plane_colors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plane_vertices), plane_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(plane_vertices), sizeof(plane_colors), plane_colors);

    // define plane position and color vertex attributes data format and enable them
    glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vecPosition);
    glVertexAttribPointer(COLOR_VEC3_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, GL_BUFFER_OFFSET(sizeof(plane_vertices)));
    glEnableVertexAttribArray(COLOR_VEC3_LOCATION);

    // setup EBO for plane face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(plane_indices), plane_indices);
    

    ////////////////////////////////////////// GROUND //////////////////////////////////////
    glBindVertexArray(VAO[1]);

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


void timerCB(int value) { 
    // update physics the whole time
    //Restart time for next frame
    int sleep_time = 1000 / FPS;
    glutTimerFunc(sleep_time, timerCB, value + sleep_time);

    //draw frame
    aircraft.UpdatePhysics(value);
    
    if (aircraft.GetPos()[0] <= -windowLength* 2/3 || aircraft.GetPos()[0] >= windowLength * 2 / 3 ||
        aircraft.GetPos()[1] <= -windowLength* 2/3 || aircraft.GetPos()[1] >= 1000 ||
        aircraft.GetPos()[2] <= -windowLength* 2/3 || aircraft.GetPos()[2] >= windowLength * 2 / 3)
    {
        //aircraft.Reset();
        std::cout << "You crashed, but you get another chance." << std::endl;
    }

    glutPostRedisplay();
}

/// <summary>
/// Takes the plain's transform and returns a camera transform behind the plain
/// </summary>
/// <param name="plainPos">The position of the airplain</param>
/// <param name="plainRot">The rotation matrix (as a Homogeneous Transformation) matrix</param>
/// <returns></returns>
glm::mat4 GetCamTransform(glm::vec3 plainPos, glm::mat4 plainRot) {
    glm::vec4 offset = glm::vec4(-240.0f, 20.0f, 0.0f, 1.0f);
    glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    offset = plainRot * offset;
    up = plainRot * up;
    
    glm::vec3 offset_rotated = glm::vec3(offset);
    glm::vec3 up_rotated = glm::vec3(up);

    glm::vec3 camPos = plainPos + offset_rotated;
    glm::vec3 targetLookAt = plainPos + up_rotated * 66.0f; //tested value to move the plain down in the view port...
    cout << t << endl;
    return glm::lookAt(camPos, targetLookAt, up_rotated);
}


void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 camRot = aircraft.GetRot();
    glm::vec4 camPos = aircraft.GetPos();

    glm::mat4 plainTransform = camRot;
    plainTransform[3][0] = camPos.x;
    plainTransform[3][1] = camPos.y;
    plainTransform[3][2] = camPos.z;

    glm::mat4 cameraTransform = TrackBall::getTransformation() * GetCamTransform(glm::vec3(camPos), camRot);
    //////////////
    // Airplane //
    //////////////
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(cameraTransform));

    plainTransform = cameraTransform * plainTransform;

    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(plainTransform));

    // bind VAO to current drawing context
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, PLANE_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);



    //////////////
    //  Ground  //
    //////////////
    
    // setup texture matrix
    glm::mat4 texture_matrix(1.0f);

    //// bind currently selected texture
    glBindTexture(GL_TEXTURE_2D, TEX_NAME);
    // setup texture matrix
    texture_matrix = glm::scale(texture_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(TEXTURE_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(cameraTransform));

    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, nullptr);

    glutSwapBuffers();
    UtilOpenGL::checkOpenGLErrorCode();
}


void initRendering(float windowLength)
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
    glm::vec4  ambient(0.8f, 0.8f, 0.8f, 1.0f);
    glm::vec4  diffuse(0.8f, 0.8f, 0.8f, 1.0f);
    glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);

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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    UtilImage::loadPNGTexture("../../png/map.png", &TEX_NAME); // Pointer to image data
    
    // setup the camera view matrix
    TheCameraView = glm::lookAt(glm::vec3(-100.0f, -666.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");

    // get texture matrix uniform location
    TEXTURE_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");

    // setup orthographic projection matrix
    glm::mat4 projection = glm::perspective(glm::radians<float>(60.0f), 1.0f, 1.0f, 2000.0f);
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
    double delta = .1;
    switch (key)
    {
        case 't':
        t += 0.1;
        break;

        case 'b': //beset
            TrackBall::resetTransformation();
            break;

        case 'T':
            t -= 0.1;
            break;

        case 32: //handles spacebar
        {
            aircraft.increaseVel(delta * 100);
            break;
        }

        case 'm': case 'M':
            aircraft.increaseVel(-delta * 100);
            break;

        // pitch around x-axis
        case 'w': case 'W':
        {
            aircraft.IncreaseAngle(2, -delta);
            break;
        }

        case 's': case 'S':
        {
            aircraft.IncreaseAngle(2, delta);
            break;
        }

        // roll around y-axis
        case 'a': case 'A':
        {
            aircraft.IncreaseAngle(0, -delta);
            break;
        }

        case 'd': case 'D':
        {
            aircraft.IncreaseAngle(0, delta);
            break;
        }

        // yaw around z-axis
        case 'o': case 'O':
        {            
            aircraft.IncreaseAngle(1, delta);

            break;
        }

        case 'p': case 'P':
        {
            aircraft.IncreaseAngle(1, -delta);
            break;
        }

        case 'q': case 'Q': case 27: // handle escape keys [q],[Q],[ESC] and exit
        {
            exit(0);
            break;
        }
        
        // Reset Settings
        case 'r': case 'R':
        {
            cout << "Reset Settings to Default..." << endl;

            aircraft.Reset();
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
    glutAddMenuEntry("Pitch + [W]", 'W');
    glutAddMenuEntry("Pitch - [S]", 'S');
    glutAddMenuEntry("Roll + [D]", 'D');
    glutAddMenuEntry("Roll - [A]", 'A');
    glutAddMenuEntry("Yaw + [P]", 'P');
    glutAddMenuEntry("Yaw - [O]", 'O');
    glutAddMenuEntry("Velocity + [Spacebar]", '32');
    glutAddMenuEntry("Velocity - [M]", 'M');
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
    aircraft = Aircraft();

    // init GLUT/FLTK settings
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(750, 750);
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
    glutTimerFunc(0, timerCB, 0);
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

    // text message
    cout << "----------------------------------------------------------------" << endl;
    cout << "This is a small flight simulator." << endl;
    cout << "You can control the paper plane with pitch, roll and yaw inputs." << endl;
    cout << "Key for pitching: W and S" << endl;
    cout << "Key for yawing: O and P" << endl;
    cout << "Key for rolling: A and S" << endl;
    cout << "Key for increasing the velocity: spacebar" << endl;
    cout << "Key for decreasing velocity: M" << endl;
    cout << "When you are out of the flight zone, your state is reset." << endl;
    cout << "Have fun." << endl;
    cout << "----------------------------------------------------------------" << endl;

    // init application
    initRendering(windowLength);
    initModel(windowLength);
    initMenu();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
