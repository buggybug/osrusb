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

    OsrUsbApiLogging.cpp

Abstract:

    This module contains the implementation of the debug tracing related
    functions.

Environment:

    User mode.

--*/

#include "pch.h"
#include "OsrUsbApiLogging.h"
#include <rtcapi.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma intrinsic(_ReturnAddress)
#endif /* defined(_MSC_VER) && (_MSC_VER >= 1300) */

//
// Tracing level description table, each level is directly mapped by its value
//
PCTSTR g_rgszLevelDescription[] = {
    _T("[%s]"),                     // TRACE_LEVEL_NONE
    _T("critical error"),           // TRACE_LEVEL_CRITICAL
    _T("error"),                    // TRACE_LEVEL_ERROR
    _T("warning"),                  // TRACE_LEVEL_WARNING
    _T("information"),              // TRACE_LEVEL_INFORMATION
    _T("trace")};                   // TRACE_LEVEL_VERBOSE

//+---------------------------------------------------------------------------
//
// _OsrLogBugCheck
//
// Performs fatal (noncontinuable) errors handling in the user space and
// raises SEH exception. Should not be called directly but from the WPP macros
// as registered in the config.
//
// Parameters:
//      Level - The level of the event. The level indicates the severity or
//              verbosity of the event. The WPP configuration (OsrUsbLogging.h)
//              defines a list of all possible values that can be passed,
//              e.g. TRACE_LEVEL_CRITICAL.
//
//----------------------------------------------------------------------------
__useHeader BOOLEAN _OsrLogBugCheck(ULONG Level)
{
    //
    // TODO: Convert the event level to the Windows error code to provide a
    //       more detailed diagnostic information
    //
    DBG_UNREFERENCED_PARAMETER(Level);

    ::RaiseException(static_cast<DWORD>(E_FAIL), EXCEPTION_NONCONTINUABLE, 0UL, NULL);

#if defined(_MSC_VER) && (_MSC_VER > 1200)
    //
    // Unreachable code, optimize out
    //
    __assume(0);
#else
    return TRUE;
#endif /* defined(_MSC_VER) && (_MSC_VER > 1200) */
}

//+---------------------------------------------------------------------------
//
// _OsrLogDebug
//
// Performs debug events handling in the user space by logging a debug message
// and breaking an execution. Should not be called directly but from the WPP
// macros as registered in the config.
//
// Parameters:
//      Level - The level of the event. The level indicates the severity or
//              verbosity of the event. The WPP configuration (OsrUsbLogging.h)
//              defines a list of all possible values that can be passed,
//              e.g. TRACE_LEVEL_ERROR.
//
// Return Values:
//      TRUE - The debug event is handled.
//      FALSE - The debug event is not handled.
//
//----------------------------------------------------------------------------
__useHeader BOOLEAN _OsrLogDebug(ULONG Level)
{
    volatile const PVOID pReturnAddress = ::_ReturnAddress();

    //
    // Find a corresponding debug event description string in the level
    // description array
    //
    const PCTSTR pszLevelDescription = g_rgszLevelDescription[
        Level < ARRAYSIZE(g_rgszLevelDescription) ? Level : 0];

    TCHAR szBuffer[64];

    //
    // Try to build a debug string ontop a stack buffer
    //
    const int cchWritten = ::_sntprintf_s(
        szBuffer,
        ARRAYSIZE(szBuffer) - 1,
        _T(OSRUSBAPI_LOGGING_NAME) _T(" %s event occured at %p.\n"),
        pszLevelDescription,
        pReturnAddress);

    if (cchWritten > 0)
    {
        ::OutputDebugString(szBuffer);
    }

    ::DbgRaiseAssertionFailure();

    //
    // All debug events are handled
    //
    return TRUE;
}
