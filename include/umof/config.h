/*********************************************************************
This file is part of the uMOF library.
Copyright (C) 2014 Artem Shal
artiom.shal@gmail.com

The uMOF library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.
**********************************************************************/

#pragma once

#define UMOF_VERSION_STR "1.0"
/*!
UMOF_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define UMOF_VERSION 0x10000
/*!
Can be used like #if (UMOF_VERSION >= UMOF_VERSION_CHECK(1, 0, 0))
*/
#define UMOF_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#if defined(_WIN32) && !defined(__CYGWIN__)
#define PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if defined(TARGET_OS_MAC)
#define PLATFORM_MAC
#elif defined(TARGET_OS_IPHONE)
#define PLATFORM_IOS
#endif
#elif defined(__linux__)
#if defined(__ANDROID__)
#define PLATFORM_ANDROID
#else
#define PLATFORM_LINUX
#endif
#else
#error Platform not defined!
#endif

#if defined(PLATFORM_WINDOWS)
#if defined(__GNUC__) //GCC on Windows
#define DECL_EXPORT __attribute__ ((dllexport))
#define DECL_IMPORT __attribute__ ((dllimport))
#elif defined(_MSC_VER) //Visual Studio
#define DECL_EXPORT __declspec(dllexport)
#define DECL_IMPORT __declspec(dllimport)
#else //Other compiler
#define DECL_EXPORT
#define DECL_IMPORT
#endif
#else
#if __GNUC__ >= 4 //GNU GCC
#define DECL_EXPORT __attribute__ ((visibility ("default")))
#define DECL_IMPORT
#else //Other
#define DECL_EXPORT
#define DECL_IMPORT
#endif
#endif

#if defined(UMOF_LIBRARY) && !defined(UMOF_STATIC)
#define UMOF_EXPORT DECL_EXPORT
#elif !defined(UMOF_STATIC)
#define UMOF_EXPORT DECL_IMPORT
#else
#define UMOF_EXPORT
#endif
