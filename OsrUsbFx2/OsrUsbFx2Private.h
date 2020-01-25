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

    OsrUsbFx2Private.h

Abstract:

    This module contains the private data structures and procedure prototypes
    for the OSR USB FX-2 LK device driver.

Environment:

    Kernel mode only.

--*/

#if !defined (OSRUSBFX2PRIVATE_H__E8F9DC11_ABB6_475A_9266_4B535F7AA1E1__INCLUDED_)
#define OSRUSBFX2PRIVATE_H__E8F9DC11_ABB6_475A_9266_4B535F7AA1E1__INCLUDED_

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* defined (_MSC_VER) && (_MSC_VER > 1000) */

//
// Pool tag used for the OSR USB FX-2 allocations
//
#define OSRUSBFX2_POOL_TAG '2xFO'

#if (NTDDI_VERSION >= NTDDI_WS03)

typedef
BOOLEAN
KD_REFRESH_DEBUGGER_NOT_PRESENT (
    VOID
    );

//
// The OSR_SYSTEM_ROUTINE_TABLE structure represents the Windows kernel version
// dependent system routine table, i.e. table of the DDI function pointers that
// may not be supported by earlier versions of the Windows
//

typedef struct _OSR_SYSTEM_ROUTINE_TABLE {

    KD_REFRESH_DEBUGGER_NOT_PRESENT *KdRefreshDebuggerNotPresent;

} OSR_SYSTEM_ROUTINE_TABLE, *POSR_SYSTEM_ROUTINE_TABLE;
#endif /* NTDDI_VERSION >= NTDDI_WS03 */

//
// The OSRUSBFX2_DRIVER_GLOBALS structure stores the driver-wide global data that
// can be referenced from all driver modules after DriverEntry returns.
// It's assumed that only driver entry and unload routines can modify it
//

typedef struct _OSRUSBFX2_DRIVER_GLOBALS {

    //
    // A counted Unicode string specifying the path to the driver's Services Key
    // in the registry
    //
    UNICODE_STRING RegistryPath;

#if (NTDDI_VERSION >= NTDDI_WS03)
    //
    // The DDI version dependent function pointers table
    //
    OSR_SYSTEM_ROUTINE_TABLE SystemRoutineTable;
#endif /* NTDDI_VERSION >= NTDDI_WS03 */

} OSRUSBFX2_DRIVER_GLOBALS, *POSRUSBFX2_DRIVER_GLOBALS;

extern OSRUSBFX2_DRIVER_GLOBALS OsrUsbFx2_DriverGlobals;

//++
//
// POSRUSBFX2_DRIVER_GLOBALS
// OSRUSBFX2_GET_DRIVER_GLOBALS (
//     VOID
//     )
//
// Routine Description:
//
//     This macro retrieves a pointer to the driver's specific globals structure.
//
// Arguments:
//
//     None.
//
// Return Value:
//
//     A pointer to the driver's globals structure.
//
//--

#define OSRUSBFX2_GET_DRIVER_GLOBALS() (&OsrUsbFx2_DriverGlobals)

//
// This structure represents the instance information associated with the
// OSR USB FX-2 LK device and holds any data the driver must have resident and in
// system space to carry out its I/O operations
//

typedef struct _OSRUSBFX2_DEVICE_EXTENSION {

    //
    // N.B. Do not provide a pointer to the function device object (FDO),
    //      because the device extension structure is always obtained from
    //      the device object and never used as a standalone structure
    //

    //
    // The device object created by a lower-level driver (bus driver object)
    //
    PDEVICE_OBJECT PhysicalDeviceObject;

} OSRUSBFX2_DEVICE_EXTENSION, *POSRUSBFX2_DEVICE_EXTENSION;

//
// Forward declaration of entry points for the driver's routines that are
// referenced in the DriverEntry function but implemented in other modules
//

DRIVER_ADD_DEVICE OsrUsbFx2_PnPAddDevice;

#endif /* !defined (OSRUSBFX2PRIVATE_H__E8F9DC11_ABB6_475A_9266_4B535F7AA1E1__INCLUDED_) */
