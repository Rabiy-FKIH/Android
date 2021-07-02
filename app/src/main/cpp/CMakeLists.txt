# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.llog
set(pathToProject /home/rabiy/AndroidStudioProjects/test3/app)
set(libs ${pathToProject}/src/main/jniLibs)


project("test3")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp )

add_library( greio

             SHARED

             #use the IMPORTED flag to tell CMake that you only want to import the library into your project
             IMPORTED )

set_target_properties( # Specifies the target library.
        greio

        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION

        # Provides the path to the library you want to import.
        #greio/src/${ANDROID_ABI}/libgreio.so
        ${libs}/${ANDROID_ABI}/libgreio.so
        )
        #include_directories( greio/include/ )

find_library( # Defines the name of the path variable that stores the
        # location of the NDK library.
        log-lib

        # Specifies the name of the NDK library that
        # CMake needs to locate.
        lib-log )

# Specifies a path to native header files.
include_directories(include/)

target_link_libraries( native-lib greio )
#target_link_libraries( native-lib ${log-lib})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#[[

find_library(# Sets the name of the path variable.
        # Set the name of the dependency library, which will be used in the following link library
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.

        # Find log dependency Library
        # {sdk-path}/ndk-bundle/sysroot/usr/include/android/log.h
        log )
        # Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib


                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
]]
