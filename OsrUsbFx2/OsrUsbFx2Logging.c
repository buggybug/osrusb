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

    Kernel mode only.

--*/

#include "pch.h"
#include "OsrUsbFx2Logging.h"
#include <rtcapi.h>

#if defined (EVENT_TRACING)
//
// The trace message header (.tmh) file must be included in a source file
// before any WPP macro calls and after defining a WPP_CONTROL_GUIDS
// macro (defined in OsrUsbFx2Logging.h)
//
#include "OsrUsbFx2Logging.tmh"
#endif /* defined (EVENT_TRACING) */

#if defined (_MSC_VER) && (_MSC_VER >= 1300)
#pragma intrinsic(_ReturnAddress)
#endif /* defined (_MSC_VER) && (_MSC_VER >= 1300) */

//
// Define the sections that allow for discarding (i.e. paging) some of the code
//

#if defined (ALLOC_PRAGMA)
#pragma alloc_text(INIT, OSR_LOG_INIT)
#pragma alloc_text(PAGE, OSR_LOG_CLEANUP)
#endif /* defined (ALLOC_PRAGMA) */

//
// Debug tracing helper function declarations
//

__checkReturn
__drv_sameIRQL
BOOLEAN
OsrIsDebuggerPresent (
    __in BOOLEAN Refresh
    );

//
// Debug tracing functions implementation
//

__useHeader
VOID
OSR_LOG_INIT (
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    Registers the provider GUID and initializes the structures that are needed
    for software tracing in a kernel-mode driver.

Arguments:

    DriverObject - A pointer to the driver object that represents the driver.

    RegistryPath - A pointer to a Unicode string that specifies the path to the
    driver's registry key.

Return Value:

    None.

--*/
{
    PAGED_CODE ();

    WPP_INIT_TRACING (DriverObject, RegistryPath);
}

__useHeader
VOID
OSR_LOG_CLEANUP (
    PDRIVER_OBJECT DriverObject
    )
/*++

Routine Description:

    Unregister the provider that was registered by calling OSR_LOG_INIT and
    deactivates software tracing in a kernel-mode driver.

Arguments:

    DriverObject - A pointer to the driver object that represents the driver.

Return Value:

    None.

--*/
{
    PAGED_CODE ();

    WPP_CLEANUP (DriverObject);
}

__useHeader
BOOLEAN
_OsrLogBugCheck (
    ULONG Level
    )
/*++

Routine Description:

    Performs fatal (unrecoverable) errors handling in the kernel space and
    brings down the system in a controlled manner. Should not be called directly
    but from the WPP macros as registered in the config.

Arguments:

    Level - The level of the event. The level indicates the severity or
    verbosity of the event. The WPP configuration (OsrUsbLogging.h) defines a
    list of all possible values that can be passed, e.g. TRACE_LEVEL_CRITICAL.

Return Value:

    None.

--*/
{
    volatile CONST PVOID returnAddress = _ReturnAddress ();

    //
    // This routine is called from the OSR_CRTITICAL_ASSERT, i.e. there is no
    // other way to recover from the failure but to bring down the system.
    //
#if defined (_PREFAST_)
    //
    // The PREFast suppress specifier disables the specified warning for the
    // next line including the preprocessor commands, which makes impossible to
    // use it inside of the preprocessor block, so we use an explicit push-pop
    // sequence.
    //
#pragma prefast(push)
#pragma prefast(disable:__WARNING_USE_OTHER_FUNCTION, "Unrecoverable error.")
#endif /* defined (_RREFAST_) */

    KeBugCheckEx (
        BUGCODE_ID_DRIVER,
        (ULONG_PTR) returnAddress,
        Level,
        0,
        0
        );

#if defined (_PREFAST_)
#pragma prefast(pop)
#endif /* defined (_RREFAST_) */

#if defined (_MSC_VER) && (_MSC_VER > 1200)
    //
    // Unreachable code, optimize out
    //
    __assume (0);
#else
    return TRUE;
#endif /* defined (_MSC_VER) && (_MSC_VER > 1200) */
}

__useHeader
BOOLEAN
_OsrLogDebug (
    ULONG Level
    )
/*++

Routine Description:

    Performs debug events handling in the kernel space by logging a debug message
    and breaking an execution if a kernel debugger is currently attached. Should
    not be called directly but from the WPP macros as registered in the config.

Arguments:

    Level - The level of the event. The level indicates the severity or verbosity
    of the event. The WPP configuration (OsrUsbLogging.h) defines a list of all
    possible values that can be passed, e.g. TRACE_LEVEL_ERROR.

Return Value:

    TRUE if the debug event is handled,
    FALSE otherwise.

--*/
{
    //
    // Tracing level description table, each level is directly mapped by its value
    //
    static PCSTR levelDescription[] = {
        "[%s]",                         // TRACE_LEVEL_NONE
        "critical error",               // TRACE_LEVEL_CRITICAL
        "error",                        // TRACE_LEVEL_ERROR
        "warning",                      // TRACE_LEVEL_WARNING
        "information",                  // TRACE_LEVEL_INFORMATION
        "trace"                         // TRACE_LEVEL_VERBOSE
    };

    volatile CONST PVOID returnAddress = _ReturnAddress ();

    //
    // Find a corresponding debug event description string in the level
    // description array
    //
    CONST PCSTR levelString = levelDescription[
        Level < ARRAYSIZE(levelDescription) ? Level : 0];

    CHAR responseBuffer[2];
    BOOLEAN continueLoop = TRUE;

    //
    // Don't waste time if there is no kernel debugger currently attached
    //
    // The kernel debugger presence is checked during a first call only, in order
    // to do it every time the TRUE value should be passed istead
    //
    if (!OsrIsDebuggerPresent (FALSE)) {
        return TRUE;
    }

    //
    // Send a string (debug event description) to the kernel debugger
    //
    DbgPrintEx (
        DPFLTR_DEFAULT_ID,
        DPFLTR_ERROR_LEVEL,
        OSR_USB_FX_2_LOGGING_NAME " %s event occured at 0x%p.\n",
        levelString,
        returnAddress
        );

    //
    // Ask if the user wants to break an execution to inspect the current state or
    // ignore a debug event and continue 
    //
    while (continueLoop) {
        DbgPrompt (
            "Debug event occured. Do you want to break (y[Y] - break, n[N] - ignore)? ",
            responseBuffer,
            ARRAYSIZE(responseBuffer)
            );

        //
        // The DbgPrompt won't return until the response buffer is completely
        // filled or a new line character is passed, so the length check is
        // optional
        //
        // Perform a case insensitive comparison of a first character, a second
        // one is supposed to be a new line character and can be ignored anyway
        //
        if ('Y' == (responseBuffer[0] & ~0x20)) {
            DbgBreakPoint ();
            continueLoop = FALSE;
        }
        else if ('N' == (responseBuffer[0] & ~0x20)) {
            continueLoop = FALSE;
        }
    }

    return TRUE;
}

__useHeader
BOOLEAN
OsrIsDebuggerPresent (
    BOOLEAN Refresh
    )
/*++

Routine Description:

    Checks whether a kernel debugger is currently attached.

Arguments:

    Refresh - If set (TRUE) the global kernel debugger presence flag will be
    invalidated, otherwise (FALSE) the cached value will be used.

Return Value:

    TRUE if debugger is present,
    FALSE otherwise.

--*/
{
#if (NTDDI_VERSION >= NTDDI_WS03)
    static BOOLEAN debuggerPresenceInitialized = FALSE;

    //
    // Thread unsafe, yet disregardable. May refresh debugger status global
    // variable several times. Much more efficient than atomic test and set.
    // KdRefreshDebuggerNotPresent returns !KD_DEBUGGER_NOT_PRESENT value,
    // but in order to reduce the number of conditional branches just use the
    // KD_DEBUGGER_NOT_PRESENT variable directly.
    //
    if (!debuggerPresenceInitialized) {
        KdRefreshDebuggerNotPresent ();
        debuggerPresenceInitialized = TRUE;
    }
    else if (Refresh) {
        KdRefreshDebuggerNotPresent ();
    }
#endif /* NTDDI_VERSION >= NTDDI_WS03 */
    UNREFERENCED_PARAMETER (Refresh);

    return KD_DEBUGGER_ENABLED && !KD_DEBUGGER_NOT_PRESENT;
}
