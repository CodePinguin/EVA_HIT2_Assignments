///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilFLTK.h
//
//  \brief      Deklaration of FLTK utility class with general utils for FLTK/GLUT/OS support
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2016-03-16   1.00      klu      Initial version of FLTK system nativ common dialog boxes
//     2021-03-01   3.00      klu      Adjustments for FLTK 1.4.x and C++17 compatibility
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// system includes ////////////////////////////////////////////////////////////////////////////////



class UtilFLTK
///////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    static bool getFilePath(std::string& filepath,
        const std::string& filter = "All Files (*.*)\n*.*\n\0",
        const std::string& startdir = "" , bool open = true);

    static void showMessageBox(
        const std::string& msg,
        const std::string& title = "Error Message");
};
// class UtilFLTK /////////////////////////////////////////////////////////////////////////////////
