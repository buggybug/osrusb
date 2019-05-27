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

    dllmain.cpp

Abstract:

    This module contains the implementation of the DLL entry point and its
    exported functions for providing COM support.

    None of this functions are supposed to be called directly. Use the Win32
    and/or COM routines instead.

--*/

#include "pch.h"
#include "OsrUsbApiModule.h"

//
// DLL in-process server module instance
//
// As of the ATL 7.0, the CComModule (and its instance _Module) is obsolete
// and has been replaced with the CAtlBaseModule (_AtlBaseModule instance).
// The last one does not implement COM server module, so an additional object
// must be defined in order to support the DLL in-process server
//
// The base class CAtlDllModuleT provides all DLL COM server related routines
// (see COsrUsbApiModule)
//
COsrUsbApiModule g_OsrUsbApiModule;

//+---------------------------------------------------------------------------
//
// DllMain
//
// An entry point into a dynamic-link library (DLL). When the system starts or
// terminates a process or thread, it calls the entry-point function for each
// loaded DLL using the first thread of the process. The system also calls the
// entry-point function for a DLL when it is loaded or unloaded using the
// LoadLibrary and FreeLibrary functions.
//
// Parameters:
//      hInstance  - A handle to the DLL module. The value is the base address
//                   of the DLL.
//      dwReason   - The reason code that indicates why the DLL entry-point
//                   function is being called.
//      pContext   - If dwReason is DLL_PROCESS_ATTACH, pContext is NULL for
//                   dynamic loads and non-NULL for static loads.
//
// Return Values:
//      When the system calls the DllMain function with the DLL_PROCESS_ATTACH
//      value, the function returns TRUE if it succeeds or FALSE if
//      initialization fails.
//
//      When the system calls the DllMain function with any value other than
//      DLL_PROCESS_ATTACH, the return value is ignored.
//
//----------------------------------------------------------------------------
extern "C" BOOL WINAPI DllMain(
    __in HINSTANCE hInstance,
    __in DWORD dwReason,
    __in_opt PCONTEXT pContext) throw()
{
    BOOL bStatus;

    //
    // Base address of the DLL module is unused here. It can be obtained later
    // through a call to the ATL::_AtlBaseModule.GetModuleInstance
    //
    UNREFERENCED_PARAMETER(hInstance);

    //
    // Perform all initialization and cleanup operations. Pay attention that
    // the DLL module can be considered valid after a successful call to the
    // internal DllMain function on process attach and before a call to the
    // DllMain function on process detach
    //

    //
    // Do necessary cleanup only if the DLL is being unloaded dynamically
    //
    if (DLL_PROCESS_DETACH == dwReason && NULL == pContext)
    {
    }

    bStatus = g_OsrUsbApiModule.DllMain(dwReason, pContext);

    //
    // Perform an additional set of initialization steps if the DLL has been
    // successfuly loaded
    //
    if (DLL_PROCESS_ATTACH == dwReason && bStatus)
    {
    }

    return bStatus;
}

//+---------------------------------------------------------------------------
//
// DllCanUnloadNow
//
// Used to determine whether the DLL can be unloaded by OLE.
//
// Parameters:
//      void - (unused argument).
//
// Return Values:
//      S_OK - DLL can be unloaded.
//      S_FALSE - DLL cannot be unloaded now.
//
//----------------------------------------------------------------------------
__useHeader STDAPI DllCanUnloadNow(void) throw()
{
    HRESULT hr = g_OsrUsbApiModule.DllCanUnloadNow();
    return hr;
}

//+---------------------------------------------------------------------------
//
// DllGetClassObject
//
// Returns a class factory to create an object of the requested type.
//
// Parameters:
//      rclsid - The CLSID that will associate the correct data and code.
//      riid   - A reference to the identifier of the interface that the
//               caller is to use to communicate with the class object.
//      ppv    - The address of a pointer variable that receives the interface
//               pointer requested in riid. Upon successful return, ppv
//               contains the requested interface pointer. If an error occurs,
//               the interface pointer is NULL.
//
// Return Values:
//      Various HRESULT codes.
//
//      S_OK - The object was retrieved successfully.
//      CLASS_E_CLASSNOTAVAILABLE - The DLL does not support the class.
//
//----------------------------------------------------------------------------
__useHeader STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) throw()
{
    HRESULT hr = g_OsrUsbApiModule.DllGetClassObject(rclsid, riid, ppv);
    return hr;
}

//+---------------------------------------------------------------------------
//
// DllRegisterServer
//
// Adds entries to the system registry for all classes supported in this
// server module.
//
// Parameters:
//      void - (unused argument).
//
// Return Values:
//      Various HRESULT codes.
//
//      S_OK - The registry entries were created successfully.
//      SELFREG_E_TYPELIB - The server was unable to complete the registration
//                          of all the type libraries used by its classes.
//      SELFREG_E_CLASS - The server was unable to complete the registration
//                        of all the object classes.
//
//----------------------------------------------------------------------------
__useHeader STDAPI DllRegisterServer(void) throw()
{
    //
    // Registers object, typelib and all interfaces in typelib
    //
    HRESULT hr = g_OsrUsbApiModule.DllRegisterServer();
    return hr;
}

//+---------------------------------------------------------------------------
//
// DllUnregisterServer
//
// Removes entries from the system registry created through the
// DllRegisterServer.
//
// Parameters:
//      void - (unused argument).
//
// Return Values:
//      Various HRESULT codes.
//
//      S_OK - The registry entries were deleted successfully.
//      S_FALSE - Unregistration of this server's known entries was
//                successful, but other entries still exist for this server's
//                classes.
//      SELFREG_E_TYPELIB - The server was unable to remove the entries of all
//                          the type libraries used by its classes.
//      SELFREG_E_CLASS - The server was unable to remove the entries of all
//                        the object classes.
//
//----------------------------------------------------------------------------
__useHeader STDAPI DllUnregisterServer(void) throw()
{
    HRESULT hr = g_OsrUsbApiModule.DllUnregisterServer();
    return hr;
}
