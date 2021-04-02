///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilCPP.cpp
//
//  \brief      Implementation of C++ general utility class
//
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2021-02-20   1.00      klu      Initial file release 
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////




// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <sstream>



// application helper includes ////////////////////////////////////////////////////////////////////
#include "../inc/UtilCPP.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showCPPLanguageVer()
// purpose:  Function to show the C++ language standard version in console window.
//
// remarks:  For MSVC to be ISO compatible the "/Zc:__cplusplus" compiler flag needs to be set
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilCPP::showCPPLanguageVer(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    std::cout << "C++ Version    : ";
    if (__cplusplus > 201703L) std::cout << "C++17 and later";
    //else if (__cplusplus == 202002L) std::cout << "C++20";
    else if (__cplusplus == 201703L) std::cout << "C++17";
    else if (__cplusplus == 201402L) std::cout << "C++14";
    else if (__cplusplus == 201103L) std::cout << "C++11";
    else if (__cplusplus == 199711L) std::cout << "C++98";
    else std::cout << "Pre-Standard C++";
    std::cout << std::endl;
}
// showCPPLanguageVer() ///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showCPPCompilerVer()
// purpose:  Function to show the C++ compiler and name and version in console window.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilCPP::showCPPCompilerVer()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    std::cout << "C++ Compiler   : ";

    std::stringstream version;
    #ifdef _MSC_VER
    version << std::to_string(_MSC_FULL_VER).substr(0,2) << ".";
    version << std::to_string(_MSC_FULL_VER).substr(2,2) << ".";
    version << std::to_string(_MSC_FULL_VER).substr(4) << ".";
    version << std::to_string(_MSC_BUILD);
    
    std::cout << "Visual Studio ";
        if (_MSC_VER > 1928) std::cout << "2019 v16.8 and later";
        else if (_MSC_VER == 1928) std::cout << "2019 v16.8";
        else if (_MSC_VER == 1927) std::cout << "2019 v16.7";
        else if (_MSC_VER == 1926) std::cout << "2019 v16.6";
        else if (_MSC_VER == 1925) std::cout << "2019 v16.5";
        else if (_MSC_VER == 1924) std::cout << "2019 v16.4";
        else if (_MSC_VER == 1923) std::cout << "2019 v16.3";
        else if (_MSC_VER == 1922) std::cout << "2019 v16.2";
        else if (_MSC_VER == 1921) std::cout << "2019 v16.1";
        else std::cout << "older than 2019";
        std::cout << " (" << version.str() << ")" << std::endl;
    #elif __GNUG__ // symbol defined in both GCC and CLANG
        #ifndef __clang__
        std::cout << "GNU Compiler Collection (GCC), version ";
            version << std::to_string(__GNUC__) << ".";
            version << std::to_string(__GNUC_MINOR__) << ".";
            version << std::to_string(__GNUC_PATCHLEVEL__);
            std::cout << version.str() << std::endl;
        #else
        std::cout << "LLVM CLANG Compiler, version ";
            /*
            version << std::to_string(__clang_major__) << ".";
            version << std::to_string(__clang_minor__) << ".";
            version << std::to_string(__clang_patchlevel__);
            std::cout << version.str() << std::endl;*/
            std::string version_ = __clang_version__;
            std::cout << version_ << std::endl;
        #endif
    #else
    std::cout << "Unknown" << std::endl;
    #endif
}
// showCPPCompilerVer() ///////////////////////////////////////////////////////////////////////////
