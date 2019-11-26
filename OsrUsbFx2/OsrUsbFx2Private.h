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
// This structure represents the instance information associated with the
// OSR USB FX-2 LK device and holds any data the driver must have resident and in
// system space to carry out its I/O operations
//

typedef struct _OSRUSBFX2_DEVICE_EXTENSION {

    //
    // The device object created by a lower-level driver (bus driver object)
    //
    PDEVICE_OBJECT PhysicalDeviceObject;

} OSRUSBFX2_DEVICE_EXTENSION, *POSRUSBFX2_DEVICE_EXTENSION;

//
// Forward declaration of entry points for the driver's routines that are
// referenced in the DriverEntry function but implemented in other modules
//

DRIVER_ADD_DEVICE OsrUsbFx2PnPAddDevice;

#endif /* !defined (OSRUSBFX2PRIVATE_H__E8F9DC11_ABB6_475A_9266_4B535F7AA1E1__INCLUDED_) */
