Image processing using C++
==========================

In order to port my face recognition modules to mobile platform, I need to have
everything redone using C++ since Python is not supported in mobile development.

The good news is that Android provides NDK to develop with C++, and iOS Apps are
using Objective-C, which means I can create a native C library to wrap OpenCV
and dlib to fulfill the purpose of porting.
