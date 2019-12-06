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

    OsrUsbFx2Device.c

Abstract:

    This module contains routines to handle Plug and Play (PnP) requests for
    the OSR USB FX-2 device.

Environment:

    Kernel mode only.

--*/

#include "pch.h"
#include "OsrUsbFx2Logging.h"
#include "OsrUsbFx2Private.h"
#include <OsrUsbFx2Public.h>

#if defined (EVENT_TRACING)
//
// The trace message header (.tmh) file must be included in a source file
// before any WPP macro calls and after defining a WPP_CONTROL_GUIDS
// macro (defined in OsrUsbFx2Logging.h). During the compilation, WPP scans
// the source files for OSR_LOG...() calls and builds a .tmh file which stores
// a unique data GUID for each message, the text resource string for each
// message, and the data types of the variables passed in for each message.
// This file is automatically generated and used during post-processing.
//
#include "OsrUsbFx2Device.tmh"
#endif /* defined (EVENT_TRACING) */

__checkReturn
__drv_maxIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
NTSTATUS
OsrUsbFx2_CreateDeviceObject (
    __in PDRIVER_OBJECT DriverObject,
    __in PDEVICE_OBJECT PhysicalDeviceObject,
    __out
    __drv_out_deref(
        __drv_allocatesMem(Mem)
        __on_failure(__null))
    PDEVICE_OBJECT *DeviceObject
    );

//
// Define the sections that allow for discarding (i.e. paging) some of the code
//

#if defined (ALLOC_PRAGMA)
#pragma alloc_text(PAGE, OsrUsbFx2_PnPAddDevice)
#pragma alloc_text(PAGE, OsrUsbFx2_CreateDeviceObject)
#endif /* defined (ALLOC_PRAGMA) */

__useHeader
NTSTATUS
OsrUsbFx2_CreateDeviceObject (
    PDRIVER_OBJECT DriverObject,
    PDEVICE_OBJECT PhysicalDeviceObject,
    PDEVICE_OBJECT *DeviceObject
    )
/*++

Routine Description:

    This routine creates the device object representing physical device for
    which the driver carries out I/O requests. It also attaches the device
    object to the device stack, so the device stack will contain a device object
    for each driver associated with the device.

Arguments:

    DriverObject - Caller-supplied pointer to a DRIVER_OBJECT structure. This is
    the driver's driver object.

    PhysicalDeviceObject - Caller-supplied pointer to a DEVICE_OBJECT structure
    representing a physical device object (PDO) created by a lower-level driver.

    DeviceObject - Caller-supplied pointer to a variable that receives a pointer
    to the newly created functional device object (FDO).

Return Value:

    Various NTSTATUS codes.

--*/
{
    POSRUSBFX2_DEVICE_EXTENSION deviceExtension;
    PDEVICE_OBJECT deviceObject;
    PDEVICE_OBJECT topDeviceObject;
    UNICODE_STRING symbolicLinkName;
    POWER_STATE powerState;
    NTSTATUS status;

    PAGED_CODE ();

    //
    // NULL indicates that no device object was created
    //
    *DeviceObject = (PDEVICE_OBJECT) NULL;

    //
    // Create the functional device object (FDO) and allocate device extension,
    // i.e. the driver-defined internal structure, to store the OSR USB FX-2 LK
    // device driver specific data
    //
    // WDM filter and function drivers do not name their device objects (legacy
    // option that is only used by non-PnP devices/control devices)
    //

    status = IoCreateDevice (
        DriverObject,
        sizeof(OSRUSBFX2_DEVICE_EXTENSION),
        NULL,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &deviceObject
        );

    if (!NT_SUCCESS (status)) {
        OSR_LOG_ERROR ("Failed to create the device object: %!STATUS!.", status);
        return status;
    }

    //
    // Attach the device object to the device stack and store the returned
    // device object pointer, which points to the device object of the next-lower
    // driver for the device, is a required parameter to IoCallDriver and
    // PoCallDriver when passing IRPs down the device stack
    //

    topDeviceObject = IoAttachDeviceToDeviceStack (deviceObject, PhysicalDeviceObject);

    if (NULL == topDeviceObject) {
        OSR_LOG_ERROR (
            "Failed to attach the device object to the highest device object in the chain."
            );
        IoDeleteDevice (deviceObject);
        return STATUS_NO_SUCH_DEVICE;
    }

    //
    // Register device functionality and create a symbolic link that applications
    // or system components can use to open the device
    //

    status = IoRegisterDeviceInterface (
        PhysicalDeviceObject,
        (LPGUID) &GUID_DEVINTERFACE_OSRUSBFX2,
        NULL,
        &symbolicLinkName
        );

    if (!NT_SUCCESS (status)) {
        OSR_LOG_ERROR ("Failed to register a device interface class: %!STATUS!", status);
        IoDetachDevice (topDeviceObject);
        IoDeleteDevice (deviceObject);
        return status;
    }

    //
    // Initialize the device object
    //

#if NTDDI_VERSION >= NTDDI_VISTA
    //
    // Indicate that the driver is pageable, it allows the power manager to
    // call such driver at IRQL = PASSIVE_LEVEL
    //
    deviceObject->Flags |= DO_POWER_PAGABLE;
#else /* NTDDI_VERSION < NTDDI_VISTA */
    //
    // Prior to the Windows Vista, the operating system requires that all device
    // objects within a device stack have the same power-related flags set
    //
    deviceObject->Flags |= (topDeviceObject->Flags &
        (DO_POWER_INRUSH | DO_POWER_PAGABLE /* | DO_POWER_NOOP */));

    //
    // TODO: Move power manager related routines to the pageable code section
    //       (PAGExxxx) and call MmLockPagableCodeSection function here to lock
    //       them (entire code section) in memory if DO_POWER_PAGABLE is set
    //
#endif /* NTDDI_VERSION >= NTDDI_VISTA */

    //
    // Typically, the function driver is responsible for the device power policy
    //
    // Set the initial power state of the device
    //
    powerState.DeviceState = PowerDeviceD3;
    PoSetPowerState (deviceObject, DevicePowerState, powerState);

    //
    // Initialize the device extension
    //

    deviceExtension = deviceObject->DeviceExtension;

    deviceExtension->PhysicalDeviceObject = PhysicalDeviceObject;

    //
    // The device object was successfully created and initialized, so assign
    // an output pointer
    //
    *DeviceObject = deviceObject;

    return STATUS_SUCCESS;
}

__useHeader
NTSTATUS
OsrUsbFx2_PnPAddDevice (
    PDRIVER_OBJECT DriverObject,
    PDEVICE_OBJECT PhysicalDeviceObject
    )
/*++

Routine Description:

    OsrUsbFx2_PnPAddDevice creates functional device objects (FDO) for OSR USB
    FX-2 LK devices enumerated by the Plug and Play (PnP) manager.

Arguments:

    DriverObject - Caller-supplied pointer to a DRIVER_OBJECT structure. This is
    the driver's driver object.

    PhysicalDeviceObject - Caller-supplied pointer to a DEVICE_OBJECT structure
    representing a physical device object (PDO) created by a lower-level driver.

Return Value:

    Various NTSTATUS codes.

--*/
{
    PDEVICE_OBJECT deviceObject;
    NTSTATUS status;

    PAGED_CODE ();

    OSR_LOG_TRACE (
        "Entering %!FUNC!(0x%p, 0x%p).",
        DriverObject,
        PhysicalDeviceObject
        );

    //
    // Create the functional device object (FDO)
    //
    status = OsrUsbFx2_CreateDeviceObject (
        DriverObject,
        PhysicalDeviceObject,
        &deviceObject
        );

    if (NT_SUCCESS (status)) {
        //
        // The purpose of DO_DEVICE_INITIALIZING is to prevent other components
        // from sending I/O to a device before the driver has finished
        // initializing the device object
        //
        // N.B. Do not clear this flag until the driver has set the device power
        //      state and the power DO flags
        //
        deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

        OSR_LOG_INFORMATION ("Successfully created the device object: 0x%p.", deviceObject);
    }

    OSR_LOG_TRACE ("Leaving %!FUNC!: %!STATUS!.", status);

    return status;
}
