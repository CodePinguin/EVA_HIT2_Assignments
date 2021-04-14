# Unified CMake file for CPP/CG module projects
#
# Version 2.0 (Urs Kuenzler)


cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)


# get project name from current CMakeList.txt file directory 
string(FIND ${CMAKE_CURRENT_LIST_DIR} / NAME_START_POS REVERSE)
math(EXPR NAME_START_POS "${NAME_START_POS}+1")
string(SUBSTRING ${CMAKE_CURRENT_LIST_DIR} ${NAME_START_POS} -1 PROJECT_NAME)
#message("Adding Project: " ${PROJECT_NAME})
project(${PROJECT_NAME})

# determine source file contaning main() function and ignore project name suffix (i.e. EX or numbers)
string(FIND ${PROJECT_NAME} _ NAME_START_POS REVERSE)
math(EXPR NAME_START_POS "${NAME_START_POS}+1")
string(SUBSTRING ${PROJECT_NAME} ${NAME_START_POS} -1 MAIN_SOURCE_FILE)
string(REGEX REPLACE "EX.*$" "" MAIN_SOURCE_FILE ${MAIN_SOURCE_FILE})
string(REGEX REPLACE "[0-9][0-9]*$" "" MAIN_SOURCE_FILE ${MAIN_SOURCE_FILE})
string(APPEND MAIN_SOURCE_FILE ".cpp")
message(STATUS "Parsing main() function file: " ${MAIN_SOURCE_FILE})


# adjust some global CMake configuration settings
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_COLOR_MAKEFILE TRUE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_PREFIX_PATH "${CMAKE_HOME_DIRECTORY}/_LIBS;${CMAKE_PREFIX_PATH}")


# setup package root directories
set(FLTK_ROOT_DIR "${CMAKE_HOME_DIRECTORY}/_LIBS/FLTK")
set(GLEW_ROOT_DIR "${CMAKE_HOME_DIRECTORY}/_LIBS/GLEW")
set(GLM_ROOT_DIR "${CMAKE_HOME_DIRECTORY}/_LIBS/GLM")


if(WIN32)
   if(MSVC)
      add_definitions(-DGLEW_STATIC)
      string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE " /INCREMENTAL:NO")
      string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " /DEBUG /NODEFAULTLIB:MSVCRT")
      string(APPEND CMAKE_CXX_STANDARD_LIBRARIES " comctl32.lib ws2_32.lib")
      string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
   endif(MSVC)      
endif(WIN32)


# setup build configurations
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug Release)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we need"
        FORCE)
endif()


#configure the Visual Studio user file with debug path
if(WIN32)
    if(MSVC)
        # find user and system name
        set(VC_USER_SYSTEM_NAME $ENV{USERDOMAIN} CACHE STRING SystemName)
        set(VC_USER_USER_NAME $ENV{USERNAME} CACHE STRING UserName)

        # configure the template file
        set(USER_FILE ${PROJECT_NAME}.vcxproj.user)
        set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})

        # setup working directories in template file
        set(USERFILE_WORKING_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug)
        set(USERFILE_WORKING_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release)
        set(USERFILE_ARGUMENTS_RELEASE "")
        set(USERFILE_ARGUMENTS_DEBUG "")
        configure_file(${CMAKE_HOME_DIRECTORY}/_CMAKE/MSVS.VCXPROJ.USERTEMPLATE.xml ${OUTPUT_PATH} @ONLY)
    endif(MSVC)
endif(WIN32)


# check for Linux and make output path adjustments
if ("${CMAKE_SYSTEM}" MATCHES "Linux.*")
    # set the binary output path to correspond to windows defaults
    set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
endif()


# check for CLion IDE from Jetbrains
if ($ENV{CLION_IDE})
    # set the binary output path to correspond to windows defaults
    set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
endif ()


# find project COMMON header and source file dependencies from main file includes
# (needs ';' as file separator to be compatible with CMake lists needed by add_executable()!)
set(COMMON_HDRS "")
file(STRINGS src/${MAIN_SOURCE_FILE} COMMON_HDRS REGEX "#include.*COMMON.*")
if(NOT COMMON_HDRS STREQUAL "")
    string(REPLACE "#include \"../.." "${CMAKE_HOME_DIRECTORY}" COMMON_HDRS ${COMMON_HDRS})
    string(REPLACE "\"" ";" COMMON_HDRS ${COMMON_HDRS})
    #message("Found COMMON headers: " ${COMMON_HDRS})
    string(REPLACE "inc" "src" COMMON_SRCS ${COMMON_HDRS})
    string(REPLACE ".h" ".cpp;" COMMON_SRCS ${COMMON_SRCS})
    #message("Found COMMON sources: " ${COMMON_SRCS})
    source_group("Common Dependencies" FILES ${COMMON_HDRS})
    source_group("Common Dependencies" FILES ${COMMON_SRCS})
endif()


# find project local header files (and let them show up in the IDEs)
file(GLOB PROJECT_HDRS
    inc/*.h
    inc/*.hpp
)
source_group("inc" FILES ${PROJECT_HDRS})
list(APPEND HDRS ${COMMON_HDRS} ${PROJECT_HDRS} )
#message("Found headers: " ${HDRS})

# find project local source files (and let them show up in the IDEs)
file(GLOB PROJECT_SRCS
    src/*.c
    src/*.cpp
)
source_group("src" FILES ${PROJECT_SRCS})
list(APPEND SRCS ${COMMON_SRCS} ${PROJECT_SRCS})
#message("Found sources: " ${SRCS})


# find GLSL shaders files (and let them show up in the IDEs)
file(GLOB GLSL
    glsl/*.frag
    glsl/*.vert
    glsl/*.geom
    glsl/*.tese
    glsl/*.tesc
)
source_group("glsl" FILES ${GLSL})


# find packages and libs
find_package(OpenGL REQUIRED)
find_package(FLTK REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)


# find framework (specific to mac)
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
elseif()
    set(COCOA_LIBRARY " ")
endif()


# setup package headers
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${FLTK_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
)


# setup debug/release libraries
set(LIBS_DEBUG)
foreach(lib; ${OPENGL_LIBRARIES}
             ${FLTK_LIBRARIES_DEBUG}
             ${GLEW_LIBRARIES_DEBUG})
    list(APPEND LIBS_DEBUG debug ${lib})
endforeach()

set(LIBS_RELEASE)
foreach(lib; ${OPENGL_LIBRARIES}
             ${FLTK_LIBRARIES}
             ${GLEW_LIBRARIES})
    list(APPEND LIBS_RELEASE optimized ${lib})
endforeach()


# define target dependencies with build instructions (and append user name if available)
set(EXECUTABLE_NAME ${PROJECT_NAME}${USER_NAME})
add_executable(${EXECUTABLE_NAME} ${SRCS} ${HDRS} ${GLSL})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 17)


# check if we have a solution folder name defined
if(DEFINED SOLUTION_NAME)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES FOLDER ${SOLUTION_NAME})
endif()


# add framework (specific to mac)
target_link_libraries(${EXECUTABLE_NAME} ${LIBS_RELEASE} ${LIBS_DEBUG} ${COCOA_LIBRARY})