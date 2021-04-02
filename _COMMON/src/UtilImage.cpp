///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilImage.cpp
//
//  \brief      Implementation of OpenGL utility class for image handling
//
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2021-03-06   1.00      klu      Initial file release (using LodePNG version 20201017)
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
using namespace std;


// application helper includes ////////////////////////////////////////////////////////////////////
// (we include the .cpp file, because our automated CMake dependency evaluation can not find this) 
//#include "../inc/lodepng.h"
#include "../src/lodepng.cpp" 
#include "../inc/UtilImage.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: makeCheckImageTexture()
// purpose:  Function to create a checker board pattern image and create and return a texture ID.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilImage::makeCheckImageTexture(GLuint* id)
{
    // make checkerboard texture
    const int checkImageWidth = 64;
    const int checkImageHeight = 64;
    static GLubyte checkImage[checkImageHeight][checkImageWidth][4];


    int i, j, c;

    for (i = 0; i < checkImageHeight; i++)
    {
        for (j = 0; j < checkImageWidth; j++)
        {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkImage[i][j][0] = (GLubyte)c;   // red
            checkImage[i][j][1] = (GLubyte)c;   // green
            checkImage[i][j][2] = (GLubyte)c;   // blue
            checkImage[i][j][3] = (GLubyte)255; // alpha
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, id);
    glBindTexture(GL_TEXTURE_2D, *id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
        checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}
// makeCheckImageTexture(() ///////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: loadPNGTexture()
// purpose:  Function to load a PNG image from filename path and create and return a texture ID.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilImage::loadPNGTexture(const char* filename, GLuint* id)
{
    // load file and decode image.
    vector<unsigned char> image;
    unsigned int width, height;
    unsigned int error = lodepng::decode(image, width, height, filename);

    // if there's an error, display it.
    if (error != 0) cout << endl << "loadPNGTexture Error " << error << ": " << lodepng_error_text(error) << endl;

    glGenTextures(1, id);
    glBindTexture(GL_TEXTURE_2D, *id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
}
// loadPNGTexture(() //////////////////////////////////////////////////////////////////////////////