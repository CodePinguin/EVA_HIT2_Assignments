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

float windowLength = 5000.0f;

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
    float thick = 1.5f;
    float thickShaft = 1.5f;

    GLfloat plane_vertices[] =
    {
        -width*1.5,  0.0f,      -width/3, 1.0f, //v0
        -width,      0.0f,        0.0f,   1.0f, //v1
        -width*1.5,  0.0f,       width/3, 1.0f, //v2
         width,      0.0f,        0.0f,   1.0f, //v3
                            
        -width,  -width/4,  thickShaft,   1.0f, //v4 shaft right
                            
        -width*1.5, thick,      -width/3, 1.0f, //v5 // thickness of plain
        -width,     thick,        0.0f,   1.0f, //v6
        -width*1.5, thick,       width/3, 1.0f, //v7
         width,     thick,        0.0f,   1.0f, //v8

        -width,  -width/4, -thickShaft, 1.0f  //v9 // shaft left
        
    };


    // definition of ground colors, each vertex has its own color definition (RGB)
    GLfloat plane_colors[] =
    {
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 1.0f
    };

    // definition of plane face indices
    GLushort plane_indices[] =
    {
        0, 1, 3, // left
        5, 6, 8,
        2, 1, 3, // right
        7, 6, 8,
        2, 7, 8, // side right
        3, 2, 8,
        0, 5, 8, // side left
        0, 3, 8,
        4, 1, 3, // shaft
        9, 1, 3,
        9, 4, 1,
        9, 4, 3
    };
    PLANE_INDICES_COUNT = sizeof(plane_indices) / sizeof(plane_indices[0]);

    /////////////////////////////////////////////////////////////////////////////////////////
    // define ground vertices
    GLfloat ground_vertices[24 * 3] = 
    {
        // Front
       -windowLength, -windowLength,  windowLength, //v0
        windowLength, -windowLength,  windowLength, //v1
        windowLength,  windowLength,  windowLength, //v2
       -windowLength,  windowLength,  windowLength, //v3
       // Righ       
        windowLength, -windowLength,  windowLength, //v4
        windowLength, -windowLength, -windowLength, //v5
        windowLength,  windowLength, -windowLength, //v6
        windowLength,  windowLength,  windowLength, //v7
       // Back      
       -windowLength, -windowLength, -windowLength, //v8
       -windowLength,  windowLength, -windowLength, //v9
        windowLength,  windowLength, -windowLength, //v10
        windowLength, -windowLength, -windowLength, //v11
       // Left       
       -windowLength, -windowLength,  windowLength, //v12
       -windowLength,  windowLength,  windowLength, //v13
       -windowLength,  windowLength, -windowLength, //v14
       -windowLength, -windowLength, -windowLength, //v15
       // Bottom    
       -windowLength, -windowLength,  windowLength, //v16
       -windowLength, -windowLength, -windowLength, //v17
        windowLength, -windowLength, -windowLength, //v18
        windowLength, -windowLength,  windowLength, //v19
       // Top      
       -windowLength,  windowLength,  windowLength, //v20
        windowLength,  windowLength,  windowLength, //v21
        windowLength,  windowLength, -windowLength, //v22
       -windowLength,  windowLength, -windowLength  //v23
    };

    GLfloat ground_colors[24 * 3] = 
    {
        // Front
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
        // Right      
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
        // Back 
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
        // Left 
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        // Bottom      
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
        // Top         
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f
    };

    float ground_tex[24 * 2] = 
    {
        // Front
        1.0f/4.0f, 1.0f/3.0f,
        2.0f/4.0f, 1.0f/3.0f,
        2.0f/4.0f, 0.0f/3.0f,
        1.0f/4.0f, 0.0f/3.0f,
        // Right
        2.0f/4.0f, 1.0f/3.0f,
        2.0f/4.0f, 2.0f/3.0f,
        3.0f/4.0f, 2.0f/3.0f,
        3.0f/4.0f, 1.0f/3.0f,
        // Back
        1.0f/4.0f, 2.0f/3.0f,
        1.0f/4.0f, 3.0f/3.0f,
        2.0f/4.0f, 3.0f/3.0f,
        2.0f/4.0f, 2.0f/3.0f,
        // Left
        1.0f/4.0f, 1.0f/3.0f,
        0.0f/4.0f, 1.0f/3.0f,
        0.0f/4.0f, 2.0f/3.0f,
        1.0f/4.0f, 2.0f/3.0f,
        // Bottom
        1.0f/4.0f, 1.0f/3.0f,
        1.0f/4.0f, 2.0f/3.0f,
        2.0f/4.0f, 2.0f/3.0f,
        2.0f/4.0f, 1.0f/3.0f,
        // Top
        4.0f/4.0f, 1.0f/3.0f,
        3.0f/4.0f, 1.0f/3.0f,
        3.0f/4.0f, 2.0f/3.0f,
        4.0f/4.0f, 2.0f/3.0f
    };

    GLushort ground_indices[] = 
    {
        0,  1,   2, // front
        0,  2,   3,
        4,  5,   6, // right
        4,  6,   7,
        8,  9,  10, // back
        8,  10, 11,
        12, 13, 14, // left
        12, 14, 15,
        16, 17, 18, //bottom
        16, 18, 19,
        20, 21, 22, // top 
        20, 22, 23
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
    glBufferData(GL_ARRAY_BUFFER, 24*3 * sizeof(float), ground_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), ground_colors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(1);  // Vertex normal

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(float), ground_tex, GL_STATIC_DRAW);
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
    
    if (aircraft.GetPos()[0] <= -windowLength || aircraft.GetPos()[0] >= windowLength ||
        aircraft.GetPos()[1] <= -windowLength || aircraft.GetPos()[1] >= windowLength ||
        aircraft.GetPos()[2] <= -windowLength || aircraft.GetPos()[2] >= windowLength)
    {
        aircraft.Reset();
        std::cout << "You crashed, but you get another chance." << std::endl;
    }

    glutPostRedisplay();
}

/// <summary>
/// Takes the plane's transform and returns a camera transform behind the plane
/// </summary>
/// <param name="planePos">The position of the aircraft</param>
/// <param name="planeRot">The rotation matrix (as a Homogeneous Transformation) matrix</param>
/// <returns></returns>
glm::mat4 GetCamTransform(glm::vec3 planePos, glm::mat4 planeRot) {
    glm::vec4 offset = glm::vec4(-240.0f, 30.0f, 0.0f, 1.0f); // offset of camera behind aircraft (homogeneous matrix)
    glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    offset = planeRot * offset;
    up = planeRot * up;
    
    glm::vec3 offset_rotated = glm::vec3(offset); // not homogeneous anymore
    glm::vec3 up_rotated = glm::vec3(up);         // not homogeneous anymore

    glm::vec3 camPos = planePos + offset_rotated;
    glm::vec3 targetLookAt = planePos + up_rotated * 66.0f; //tested value to move the plane down in the view port...
    return glm::lookAt(camPos, targetLookAt, up_rotated);
}


void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    // clear window background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // change camera view
    glm::mat4 camRot = aircraft.GetRot();
    glm::vec4 camPos = aircraft.GetPos();

    glm::mat4 planeTransform = camRot;
    planeTransform[3][0] = camPos.x;
    planeTransform[3][1] = camPos.y;
    planeTransform[3][2] = camPos.z;

    glm::mat4 cameraTransform = TrackBall::getTransformation() * GetCamTransform(glm::vec3(camPos), camRot); //*TheCameraView;

    //////////////
    // Aircraft //
    //////////////
    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(cameraTransform));

    planeTransform = cameraTransform * planeTransform;

    glUniformMatrix4fv(MODELVIEW_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(planeTransform));

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

    GLint location = glGetUniformLocation(PROGRAM_ID, "ReplaceColor");
    glUniform1i(location, true);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    UtilImage::loadPNGTexture("../../png/world.png", &TEX_NAME); // Pointer to image data
 
    // setup the camera view matrix
    TheCameraView = glm::lookAt(glm::vec3(0.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // get modelview matrix uniform location
    MODELVIEW_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");

    // get projection matrix uniform location
    PROJECTION_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");

    // get texture matrix uniform location
    TEXTURE_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");

    // setup orthographic projection matrix
    glm::mat4 projection = glm::perspective(glm::radians<float>(60.0f), 1.0f, 1.0f, 2 * 10000.0f);
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
    double delta = .05;
    double transDelta = 3;
    switch (key)
    {
        //case 't': // key for testing
        //t += 0.1;
        //break;

        //case 'T': // key for testing
        //    t -= 0.1;
        //    break;

        case 'b': case 'B': //reset the view after mouse movements
            TrackBall::resetTransformation();
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

        case 'u':
            TheCameraView[3][0] += transDelta;
            break;
        case 'j':
            TheCameraView[3][0] -= transDelta;
            break;
        case 'h':
            TheCameraView[3][2] -= transDelta;
            break;
        case 'k':
            TheCameraView[3][2] += transDelta;
            break;
        case '.':
            TheCameraView[3][1] += transDelta;
            break;
        case ',':
            TheCameraView[3][1] -= transDelta;
            break;

        
        // Reset Settings
        case 'r': case 'R':
        {
            std::cout << "Reset Settings to Default..." << endl;

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
    glutAddMenuEntry("Reset View [B]", 'B');
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
    std::cout << endl;

    // show version of C++ and compiler
    UtilCPP::showCPPLanguageVer();
    UtilCPP::showCPPCompilerVer();
    std::cout << endl;

    // check for shader 4.x support
    if (UtilOpenGL::checkOpenGLVersion() < 4.0)
    {
        std::cout << "Program requires OpenGL Version >= 4.x (Supported: ";
        std::cout << UtilOpenGL::checkOpenGLVersion() << ") - exiting!" << endl;
        std::cout << "Press ENTER to continue... " << endl;
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
    std::cout << "----------------------------------------------------------------" << endl;
    std::cout << "This is a small flight simulator." << endl;
    std::cout << "You can control the paper aircraft with pitch, roll and yaw inputs." << endl;
    std::cout << "Key for pitching: W and S" << endl;
    std::cout << "Key for yawing: O and P" << endl;
    std::cout << "Key for rolling: A and S" << endl;
    std::cout << "Key for increasing the velocity: spacebar" << endl;
    std::cout << "Key for decreasing velocity: M" << endl;
    std::cout << "When you are out of the flight zone, your state is reset." << endl;
    std::cout << "Have fun." << endl;
    std::cout << "----------------------------------------------------------------" << endl;

    // init application
    initRendering(windowLength);
    initModel(windowLength);
    initMenu();

    // entering GLUT/FLTK main event loop until user exits
    glutMainLoop();
    return 0;
}
