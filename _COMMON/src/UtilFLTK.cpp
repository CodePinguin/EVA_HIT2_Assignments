///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilFLTK.cpp
//
//  \brief      Implementation of FLTK utility class with general utils for FLTK/GLUT/OS support
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2016-03-16   1.00      klu      Initial version of FLTK system nativ common dialog boxes
//     2021-03-01   3.00      klu      Adjustments for FLTK 1.4.x and C++17 compatibility
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <FL/glut.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>


// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>


// application includes ///////////////////////////////////////////////////////////////////////////
#include "../inc/UtilFLTK.h"



bool UtilFLTK::getFilePath(std::string& filepath,
    const std::string& filter, const std::string& initial, bool open)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    bool result = false;

    // Create the local native file chooser

    Fl_Native_File_Chooser fltk_filechooser;
    if (open)
    {
        fltk_filechooser.title("Open GLDraw2D Drawing File");
        fltk_filechooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    }
    else
    {
        fltk_filechooser.title("Save GLDraw2D Drawing File");
        fltk_filechooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    }
    fltk_filechooser.filter(filter.c_str());
    fltk_filechooser.directory(initial.c_str());  // default directory to use

    switch (fltk_filechooser.show()) // Show native chooser
    {
        case -1: std::cout << "ERROR" << fltk_filechooser.errmsg() << std::endl; break;  // ERROR
        case  1: std::cout << "CANCEL" << std::endl;  break;  // CANCEL
        default: 
            filepath = std::string(fltk_filechooser.filename());
            result = true;
            break;  // FILE CHOSEN
    }

    return result;
}
// UtilFLTK::getFilePath() ////////////////////////////////////////////////////////////////////////



void UtilFLTK::showMessageBox(const std::string& msg, const std::string& title)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    fl_message_title(title.c_str());
    fl_message(msg.c_str());
}
// end: UtilFLTK::showMessageBox() ////////////////////////////////////////////////////////////////