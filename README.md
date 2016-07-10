[![Build Status](https://api.travis-ci.org/FabianHahn/fakeglib.svg)](https://travis-ci.org/FabianHahn/fakeglib)

# fakeglib
A partial implementation of [glib](https://github.com/GNOME/glib)'s [C API](https://developer.gnome.org/glib/2.48/) in C++.

## Why?
C is a great programming language, but generally lacks a standard library of common dynamic data structures such as strings, lists, or maps with a native C API.
The [GNOME project](https://www.gnome.org/)'s [glib](https://github.com/GNOME/glib) is a notable exception to this rule, and is pretty fantastic for developing C on Linux.
However, it comes with its own dependencies and uses automake as build tool, making it annoying (though [not impossible](https://github.com/hexchat/gtk-win32)) to build and link on non-UNIX platforms such as Windows.
This projects aims to alleviate this issue by providing a self-contained partial implementation of [glib](https://github.com/GNOME/glib)'s [C API](https://developer.gnome.org/glib/2.48/) that is backed by the C++ STL data structures - thus the name *fakeglib*.

The requirements for *fakeglib* are:
* Self-contained: Must not depend on any thirdparty code.
* Cross-platform: Must build on at least Linux and Windows.
* Native development: Uses [CMake](https://cmake.org/) to generate native build files for each supported platform.

## What?
*fakeglib* currently implements the following parts of [glib](https://github.com/GNOME/glib)'s [C API](https://developer.gnome.org/glib/2.48/):
* [Hash Tables](https://developer.gnome.org/glib/2.48/glib-Hash-Tables.html) (partial)
