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

Environment:

    User mode.

--*/

#if !defined(OSRUSBAPILOGGING_H__7A9E16F5_DE1F_4129_9D8F_33DE41ECD02B__INCLUDED_)
#define OSRUSBAPILOGGING_H__7A9E16F5_DE1F_4129_9D8F_33DE41ECD02B__INCLUDED_

#define OSRUSBAPI_LOGGING_NAME "OsrUsbApi"

#define WPP_CHECK_FOR_NULL_STRING  // To prevent exceptions due to NULL strings

//
// If software tracing is defined in the sources file WPP_DEFINE_CONTROL_GUID
// specifies the GUID used for this DLL
//
// WPP_DEFINE_BIT allows setting debug bit masks to selectively print, The
// names defined in the WPP_DEFINE_BIT call define the actual names that are
// used to control the level of tracing for the control guid specified
//

//
// Tracing GUID - 7C3EABC1-7833-4B66-87B6-AEBB25CA16FF
//
#define WPP_CONTROL_GUIDS \
    WPP_DEFINE_CONTROL_GUID(OSRUSBAPI, (7C3EABC1, 7833, 4B66, 87B6, AEBB25CA16FF), \
        WPP_DEFINE_BIT(OSR_TRACING_DEFAULT)  /* bit  0 = 0x00000001 */ \
        WPP_DEFINE_BIT(OSR_TRACING_DEBUG)    /* bit  1 = 0x00000002 */ \
        WPP_DEFINE_BIT(OSR_TRACING_BUGCHECK) /* bit  2 = 0x00000004 */ \
/* It is possible to have up to 32 defines; if more are required,      \
   another trace control GUID should be provided */                    \
    )

#define WPP_LEVEL_FLAGS_LOGGER(level, flags) WPP_LEVEL_LOGGER(flags)
#define WPP_LEVEL_FLAGS_ENABLED(level, flags) \
    (WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= level)

#include <OsrUsbLogging.h>

#endif /* !defined(OSRUSBAPILOGGING_H__7A9E16F5_DE1F_4129_9D8F_33DE41ECD02B__INCLUDED_) */
