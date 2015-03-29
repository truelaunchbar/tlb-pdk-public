# True Launch Bar Plugins Development Kit

This is the complete set of the libraries for creating the plugins for True Launch Bar. You need the Microsoft Visual Studio 2013 to compile the source codes.

## Libraries included in TLB PDK

True Launch Bar Plugins Development Kit contain some libraries. Most of the libraries are the helpers, so you can ignore them.

### TLBPDK

This is the low-level API library. This library is required, but generally you don't need to use it directly.

### tlbpdklib

This the main library. It is wrap the low-level TLBPDK with the C++ classes. To use this library just include the tlbpdklib.h into your source file.

### cairo

[Cairo](http://www.cairographics.org/) is a 2D graphics library with support for multiple output devices. Currently supported output targets include the X Window System (via both Xlib and XCB), Quartz, Win32, image buffers, PostScript, PDF, and SVG file output. Experimental backends include OpenGL, BeOS, OS/2, and DirectFB.

Cairo is designed to produce consistent output on all output media while taking advantage of display hardware acceleration when available (eg. through the X Render Extension).

We are using the [precompiled binaries for Windows](https://github.com/tordex/cairo). If you want to use Cairo library define the `TLB_USE_CAIRO` before `tlbpdklib.h`.

```C++
#define TLB_USE_CAIRO
#include <tlbpdklib.h>
```

### httpReader

The simple library to read something from the web. The library is optional. httpReader is based on curl project. To use httpReader define the `TLB_USE_HTTPREADER` before `tlbpdklib.h`.

```C++
#define TLB_USE_HTTPREADER
#include <tlbpdklib.h>
```
### txdib

The optional library for simple image processing based on FreeImage? library. To use txdib define the `TLB_USE_TXDIB` before `tlbpdklib.h`.

```C++
#define TLB_USE_TXDIB
#include <tlbpdklib.h>
```

### tlbskinslib

The optional library for skins creating. This library allow you to make the skins support for your plugin. To use tlbskinslib define the `TLB_USE_TLBSKIN` before `tlbpdklib.h`.

```C++
#define TLB_USE_TLBSKIN
#include <tlbpdklib.h>
```

### XUILIB

This the recommended library for creating user interface for the plugins.
