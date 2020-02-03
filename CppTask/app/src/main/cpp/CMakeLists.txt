# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")


add_library( # Sets the name of the library.
        native-lib
        SHARED
        native-lib.cpp
        Model2D.cpp
        Renderer.cpp)



add_subdirectory("Delaunator")


find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        native-lib
        GLESv3
        Delaunator
#        Renderer
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})