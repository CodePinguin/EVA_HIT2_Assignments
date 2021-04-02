///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   TrackBall.h
//
//  \brief      Virtual trackball helper class for easy mouse and keyboard based 3D interaction.
//              Based on an example from the book "Advanced Graphics Programming Using OpenGL,
//              Morgan Kaufmann, 2005)
//
//   Usage:     Brief usage instructions for modifiers keys (SHIFT, CTRL, ALT):
//              No Key: mouse click and drag --> rotate model (default)
//              CTRL:   mouse click and drag --> translate model in +/- X/Y direction
//              SHIFT:  mouse click and drag --> translate model in +/- Z direction
//              ALT:    mouse click and drag --> scale model up/down
//              All modifier keys can also be used with the cursor keys and work identical.
//
//              The HOME key resets the TrackBall transformation and puts the model into
//              the initial position/orientation.
//
//              Where available the MOUSEWHEEL can be used to scale the model up or down.
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2015-09-10   1.00      klu      Initial file release
//     2016-03-16   1.10      klu      Update for CPP course apps
//     2021-03-01   3.00      klu      Adjustments for FLTK 1.4.x and C++17 compatibility
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <glm/glm.hpp>



class TrackBall
///////////////////////////////////////////////////////////////////////////////////////////////////
{
private:
    enum class ETrackballMode { TM_ROTATE, TM_TRANSLATEXY, TM_TRANSLATEZ, TM_SCALE, TM_INVALID };

    static void axisAmountToMat(float aa[], float mat[]);
    static void matToAxisAmount(float mat[], float aa[]);
    static void rotateTrackball(int dx, int dy, float rotation[4]);
    static ETrackballMode evaluateTrackballMode(ETrackballMode new_mode = ETrackballMode::TM_INVALID);

private:
    // callback function pointers
    inline static void (*_AppDoubleClickFunctionCB) (int x, int y) = nullptr;
    inline static void (*_AppMouseButtonFunctionCB) (int x1, int y1, int x2, int y2) = nullptr;
    inline static void (*_AppMouseMotionFunctionCB) (int x1, int y1, int x2, int y2) = nullptr;

    inline static float _Translation[3] = { 0.0f, 0.0f, 0.0f };
    inline static float _Offset[3] = { 0.0f, 0.0f, 0.0f };
    inline static float _Rotation[4] = { 0.0f, 0.0f, 1.0f, 0.0f }; // rotation axis may not be zero!
    inline static float _Scale[3] = { 1.0f, 1.0f, 1.0f };
    inline static glm::mat4 _TrackBallMatrix = glm::mat4(1.0f);

    inline static int    _OldMouseX = 0;
    inline static int    _OldMouseY = 0;
    inline static int    _ButtonEventMouseX = 0;
    inline static int    _ButtonEventMouseY = 0;
    inline static bool   _MouseButtonPressed = false;

public:
    // glut callback functions
    static void glutMouseMotionCB(int x, int y);
    static void glutMouseButtonCB(int button, int state, int x, int y);
    static void glutSpecialFuncCB(int key, int x, int y);

    static void registerDoubleClick(void (*func)(int x, int y) = nullptr);
    static void registerMouseButton(void (*func)(int x1, int y1, int x2, int y2) = nullptr);
    static void registerMouseMotion(void (*func)(int x1, int y1, int x2, int y2) = nullptr);

    // use and reset trackball transformation
    static void applyTransformation(void);
    static glm::mat4& getTransformation(void);
    static void resetTransformation(void);

    // if required, set model origin offset
    static void setOffset(const float offset[3]) {for (int i=0; i<3; i++) _Offset[i] = offset[i];};
    static void getOffset(float offset[3]) { offset = _Offset; };
};
// class TrackBall ////////////////////////////////////////////////////////////////////////////////