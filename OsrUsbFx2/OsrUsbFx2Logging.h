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

    OsrUsbApiLogging.h

Abstract:

    Header file for the debug tracing related function defintions and macros.
    Logs can be dumped from windbg with the following command:

    !rcdrkd.rcdrlogdump osrusbfx2

Environment:

    Kernel mode only.

--*/

#if !defined (OSRUSBFX2LOGGING_H__769FE7D0_B449_4FA1_BE39_B5A1B525AFED__INCLUDED_)
#define OSRUSBFX2LOGGING_H__769FE7D0_B449_4FA1_BE39_B5A1B525AFED__INCLUDED_

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* defined(_MSC_VER) && (_MSC_VER > 1000) */

#define OSR_USB_FX_2_LOGGING_NAME "OsrUsbFx2"

#define WPP_CHECK_FOR_NULL_STRING  // To prevent exceptions due to NULL strings

//
// Tracing prologue and epilogue routines
//

__drv_requiresIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
VOID
OSR_LOG_INIT (
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
VOID
OSR_LOG_CLEANUP (
    __in PDRIVER_OBJECT DriverObject
    );

//
// If software tracing is defined in the sources file WPP_DEFINE_CONTROL_GUID
// specifies the GUID used for this driver
//
// WPP_DEFINE_BIT allows setting debug bit masks to selectively print, The
// names defined in the WPP_DEFINE_BIT call define the actual names that are
// used to control the level of tracing for the control guid specified
//

//
// Tracing GUID - 82B88A5B-C478-4054-97C5-CB755261E46E
//
#define WPP_CONTROL_GUIDS \
    WPP_DEFINE_CONTROL_GUID(OSRUSBFX2, (82B88A5B, C478, 4054, 97C5, CB755261E46E), \
        WPP_DEFINE_BIT(OSR_TRACING_DEFAULT)  /* bit  0 = 0x00000001 */ \
        WPP_DEFINE_BIT(OSR_TRACING_DEBUG)    /* bit  1 = 0x00000002 */ \
        WPP_DEFINE_BIT(OSR_TRACING_BUGCHECK) /* bit  2 = 0x00000004 */ \
/* It is possible to have up to 32 defines; if more are required,      \
   another trace control GUID should be provided */                    \
    )

//
// Prior Windows 10 WDK versions do not fully support WPP recorder feature because
// of a missing library (WppRecorder.lib), so just define logger flags as we did
// in a user space case
//
#if !defined (OSR_ENABLE_WPP_RECORDER)
#define WPP_LEVEL_FLAGS_LOGGER(level, flags) WPP_LEVEL_LOGGER(flags)
#define WPP_LEVEL_FLAGS_ENABLED(level, flags) \
    (WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= level)
#endif /* !defined (OSR_ENABLE_WPP_RECORDER) */

#include <OsrUsbLogging.h>

#endif /* !defined (OSRUSBFX2LOGGING_H__769FE7D0_B449_4FA1_BE39_B5A1B525AFED__INCLUDED_) */
