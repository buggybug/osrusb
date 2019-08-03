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

    OsrUsbApiModule.h

Abstract:

    This module contains the type definitions for the COsrUsbApiModule class.

--*/

#if !defined(OSRUSBAPIMODULE_H__61348E54_67B6_48E1_A0F9_4AB07F315C4E__INCLUDED_)
#define OSRUSBAPIMODULE_H__61348E54_67B6_48E1_A0F9_4AB07F315C4E__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* defined(_MSC_VER) && (_MSC_VER > 1000) */

//
// We do not include the resource.h here as it is already included in the
// precompiled header in order to prevent redefinition warnings
//

#include <OsrUsbApiDll.h>

//
// This class represents the module for a dynamic-link library (DLL) and
// provides support for registration
//

class COsrUsbApiModule : public ATL::CAtlDllModuleT<COsrUsbApiModule>
{
public:
    DECLARE_LIBID(LIBID_OsrUsbApiDllLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OSRUSBAPI_SERVER_REGISTRY, "{568B0689-D132-4541-A40E-3991E9E3214D}")
};

#endif /* !defined(OSRUSBAPIMODULE_H__61348E54_67B6_48E1_A0F9_4AB07F315C4E__INCLUDED_) */
