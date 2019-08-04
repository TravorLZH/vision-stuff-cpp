Image processing using C++
==========================

In order to port my face recognition modules to mobile platform, I need to have
everything redone using C++ since Python is not supported in mobile development.

The good news is that Android provides NDK to develop with C++, and iOS Apps are
using Objective-C, which means I can create a native C library to wrap OpenCV
and DLib to fulfill the purpose of porting.

## Build instructions

First, there are 2 libraries needed to be installed before the build:

* [DLib](http://dlib.net): For face recognition
* [OpenCV](https://www.opencv.org): For image processing (I think it is better
than DLib's image processing system)

If you are using Mac OSX and your HomeBrew are ready to use, you can:

    $ brew install opencv dlib

After installing prerequisites, you can build the project using CMake:

    $ mkdir build; cd build; cmake ..
    $ cmake --build .

> NOTE: The number of dots in each `cmake` command differs
