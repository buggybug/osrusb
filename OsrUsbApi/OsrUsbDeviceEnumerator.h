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

    OsrUsbDeviceEnumerator.h

Abstract:

    This module contains the type definitions for the CUsbDeviceEnumerator
    class.

--*/

#if !defined(OSRUSBDEVICEENUMERATOR_H__2622B424_6F44_4CC8_AB63_EB1B1C93AD31__INCLUDED_)
#define OSRUSBDEVICEENUMERATOR_H__2622B424_6F44_4CC8_AB63_EB1B1C93AD31__INCLUDED_

//
// We do not include the resource.h here as it is already included in the
// precompiled header in order to prevent redefinition warnings
//

#include <atlbase.h>
#include <atlcom.h>
#include <OsrUsbApiDll.h>

namespace OSR {

//
// This class provides methods for enumerating IOsrUsbDevice objects
//

class ATL_NO_VTABLE CUsbDeviceEnumerator :
    public ATL::CComObjectRootEx<ATL::CComMultiThreadModel>,
    public ATL::CComCoClass<CUsbDeviceEnumerator, &CLSID_OsrUsbDeviceEnumerator>,
    public IOsrUsbDeviceEnumerator
{
public:
    CUsbDeviceEnumerator() throw();

DECLARE_REGISTRY_RESOURCEID(IDR_OSRUSBAPI_ENUMERATOR_REGISTRY)
DECLARE_NOT_AGGREGATABLE(CUsbDeviceEnumerator)

BEGIN_COM_MAP(CUsbDeviceEnumerator)
    COM_INTERFACE_ENTRY(IOsrUsbDeviceEnumerator)
END_COM_MAP()

//
// IOsrUsbDeviceEnumerator implementation
//
public:
    IFACEMETHOD(EnumerateDevices)();
    IFACEMETHOD(GetDeviceCount)(__out DWORD* pdwDeviceCount);
    IFACEMETHOD(GetDeviceByIndex)(__in DWORD dwDeviceId, __out void** ppDevice);

protected:
    ~CUsbDeviceEnumerator() throw();

private:
//
// Private to avoid accidental use
//
    CUsbDeviceEnumerator(const CUsbDeviceEnumerator&) throw();
    CUsbDeviceEnumerator& operator=(const CUsbDeviceEnumerator&) throw();
};

//
// Provide support for the registration, initialization, and creation of a
// class so it can be externally creatable via CoCreateInstance
//
OBJECT_ENTRY_AUTO(__uuidof(OsrUsbDeviceEnumerator), CUsbDeviceEnumerator)

}; /* namespace OSR */

#endif /* !defined(OSRUSBDEVICEENUMERATOR_H__2622B424_6F44_4CC8_AB63_EB1B1C93AD31__INCLUDED_) */
