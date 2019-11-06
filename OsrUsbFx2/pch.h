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

Environment:

    Kernel mode only.

--*/

#if !defined (PCH_H__CAEFB85A_05C0_4A37_9878_33C358BC686B__INCLUDED_)
#define PCH_H__CAEFB85A_05C0_4A37_9878_33C358BC686B__INCLUDED_

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* defined(_MSC_VER) && (_MSC_VER > 1000) */

#include <ntddk.h>

#if defined (EVENT_TRACING) && defined(_MSC_VER) && (_MSC_VER >= 1900)
#include <WppRecorder.h>
#define OSR_ENABLE_WPP_RECORDER  1
#endif /* defined (EVENT_TRACING) && defined(_MSC_VER) && (_MSC_VER >= 1900) */

//
// Macro letting the compiler know this is a kernel driver
//
__kernel_driver

#endif /* !defined (PCH_H__CAEFB85A_05C0_4A37_9878_33C358BC686B__INCLUDED_) */
