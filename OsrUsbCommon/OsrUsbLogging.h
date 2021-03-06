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

    OsrUsbLogging.h

Abstract:

    This module contains the required definitions to enable WppRecorder.

--*/

#if !defined(OSRUSBLOGGING_H__D9582830_6E6B_460C_9B59_6BF879C43F07__INCLUDED_)
#define OSRUSBLOGGING_H__D9582830_6E6B_460C_9B59_6BF879C43F07__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* defined(_MSC_VER) && (_MSC_VER > 1000) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

//
// BugCheck/Debug helpers used by tracing macros.
// Note, that these are not intended to be called from anywhere else
//

BOOLEAN
_OsrLogBugCheck(
    __in ULONG Level
    );

BOOLEAN
_OsrLogDebug(
    __in ULONG Level
    );

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

//
// This comment block is scanned by the trace preprocessor to define cumstom
// tracing functions
//
// begin_wpp config
//
// FUNC OSR_LOG_CRITICAL_ERROR{LEVEL=TRACE_LEVEL_CRITICAL, FLAGS=OSR_TRACING_BUGCHECK}(MSG, ...);
// USEPREFIX (OSR_LOG_CRITICAL_ERROR, "%!STDPREFIX! [%s @ %u] CRITICAL ERROR :", __FILE__, __LINE__);
//
// FUNC OSR_LOG_ASSERTION{LEVEL=TRACE_LEVEL_ERROR, FLAGS=OSR_TRACING_DEBUG}(MSG, ...);
// USEPREFIX (OSR_LOG_ASSERTION, "%!STDPREFIX! [%s @ %u] ASSERTION :", __FILE__, __LINE__);
//
// FUNC OSR_LOG_ERROR{LEVEL=TRACE_LEVEL_ERROR, FLAGS=OSR_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (OSR_LOG_ERROR, "%!STDPREFIX! [%s @ %u] ERROR :", __FILE__, __LINE__);
//
// FUNC OSR_LOG_LOW_MEMORY{LEVEL=TRACE_LEVEL_ERROR, FLAGS=OSR_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (OSR_LOG_LOW_MEMORY, "%!STDPREFIX! [%s @ %u] LOW MEMORY :", __FILE__, __LINE__);
//
// FUNC OSR_LOG_WARNING{LEVEL=TRACE_LEVEL_WARNING, FLAGS=OSR_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (OSR_LOG_WARNING, "%!STDPREFIX! [%s @ %u] WARNING :", __FILE__, __LINE__);
//
// FUNC OSR_LOG_INFORMATION{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=OSR_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (OSR_LOG_INFORMATION, "%!STDPREFIX! [%s @ %u] INFO :", __FILE__, __LINE__);
//
// FUNC OSR_LOG_TRACE{LEVEL=TRACE_LEVEL_VERBOSE, FLAGS=OSR_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (OSR_LOG_TRACE, "%!STDPREFIX! [%s @ %u] TRACE :", __FILE__, __LINE__);
//
// FUNC OSR_TRACE_EVENTS(LEVEL, FLAGS, MSG, ...);
// USEPREFIX (OSR_TRACE_EVENTS, "%!STDPREFIX! [%s @ %u] TRACE :", __FILE__, __LINE__);
//
// FUNC OSR_CRITICAL_ASSERT{LEVEL=TRACE_LEVEL_CRITICAL, FLAGS=OSR_TRACING_BUGCHECK}(OSR_CRIT_ASSERT_EXP);
// USEPREFIX (OSR_CRITICAL_ASSERT, "%!STDPREFIX! [%s @ %u] CRITICAL ASSERTION: %s", __FILE__, __LINE__, #OSR_CRIT_ASSERT_EXP);
//
// FUNC OSR_ASSERT{LEVEL=TRACE_LEVEL_ERROR, FLAGS=OSR_TRACING_DEBUG}(OSR_ASSERT_EXP);
// USEPREFIX (OSR_ASSERT, "%!STDPREFIX! [%s @ %u] ASSERTION: %s", __FILE__, __LINE__, #OSR_ASSERT_EXP);
//
// end_wpp

//
// OSR_LOG... customization
//

#define WPP_LEVEL_FLAGS_POST(LEVEL, FLAGS) \
    ,(((WPP_BIT_ ## FLAGS) == WPP_BIT_OSR_TRACING_BUGCHECK) ? \
            _OsrLogBugCheck(LEVEL) : \
            (((WPP_BIT_ ## FLAGS) == WPP_BIT_OSR_TRACING_DEBUG) ? \
                _OsrLogDebug(LEVEL) : TRUE))

//
// OSR_CRTITICAL_ASSERT customization
//

#define WPP_RECORDER_LEVEL_FLAGS_OSR_CRIT_ASSERT_EXP_FILTER(LEVEL, FLAGS, OSR_CRIT_ASSERT_EXP) \
    (!(OSR_CRIT_ASSERT_EXP))

#define WPP_RECORDER_LEVEL_FLAGS_OSR_CRIT_ASSERT_EXP_ARGS(LEVEL, FLAGS, OSR_CRIT_ASSERT_EXP) \
    WPP_CONTROL(WPP_BIT_ ## FLAGS).AutoLogContext, LEVEL, WPP_BIT_ ## FLAGS

#define WPP_LEVEL_FLAGS_OSR_CRIT_ASSERT_EXP_POST(LEVEL, FLAGS, OSR_CRIT_ASSERT_EXP) \
    ,((!(OSR_CRIT_ASSERT_EXP)) ? _OsrLogBugCheck(LEVEL) : TRUE)

//
// OSR_ASSERT customization
//

#define WPP_RECORDER_LEVEL_FLAGS_OSR_ASSERT_EXP_FILTER(LEVEL, FLAGS, OSR_ASSERT_EXP) \
    (!(OSR_ASSERT_EXP))

#define WPP_RECORDER_LEVEL_FLAGS_OSR_ASSERT_EXP_ARGS(LEVEL, FLAGS, OSR_ASSERT_EXP) \
    WPP_CONTROL(WPP_BIT_ ## FLAGS).AutoLogContext, LEVEL, WPP_BIT_ ## FLAGS

#define WPP_LEVEL_FLAGS_OSR_ASSERT_EXP_POST(LEVEL, FLAGS, OSR_ASSERT_EXP) \
    ,((!(OSR_ASSERT_EXP)) ? _OsrLogDebug(LEVEL) : TRUE)

#endif /* !defined(OSRUSBLOGGING_H__D9582830_6E6B_460C_9B59_6BF879C43F07__INCLUDED_) */
