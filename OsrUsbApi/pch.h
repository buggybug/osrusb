/*++

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Copyright (c) 2019 Bohdan Yevtukh. All rights reserved.

Module Name:

    pch.h

Abstract:

    Include file for standard system include files, or project specific
    include files that are used frequently, but are changed infrequently.

--*/

#if !defined(PCH_H__31BF894B_AE76_4B4B_BDBB_A74713999638__INCLUDED_)
#define PCH_H__31BF894B_AE76_4B4B_BDBB_A74713999638__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* defined(_MSC_VER) && (_MSC_VER > 1000) */

#if !defined(STRICT)
#define STRICT
#endif /* !defined(STRICT) */

#include "targetver.h"

#define _ATL_FREE_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

//
// Some CString constructors will be explicit
//
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//
// Turns off ATL's hiding of some common and often safely ignored warning messages
//
#define _ATL_ALL_WARNINGS

#define _ATL_NO_HOSTING

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <tchar.h>

//+---------------------------------------------------------------------------
// ATL version check: minimum required version is 7.0
#if defined(_ATL_VER)
#if _ATL_VER < 0x0700
#error OSR USB API requires ATL version 7.0 or later
#endif /* _ATL_VER < 0x0700 */
#else /* ! defined(_ATL_VER) */
#error _ATL_VER is not defined (include atldef.h)
#endif /* defined(_ATL_VER) */
//----------------------------------------------------------------------------

//
// Macro letting the compiler know this is not a kernel driver
// (this will help surpress needless warnings)
//
__user_driver

#endif /* !defined(PCH_H__31BF894B_AE76_4B4B_BDBB_A74713999638__INCLUDED_) */
