
#include "ext-common.h"
// #include "guids.h"
// #include "shell-ext.h"
// #include "class-factory.h"

volatile LONG       g_cRefThisDll = 0;              ///< reference count of this DLL.
HINSTANCE           g_hmodThisDll = NULL;           ///< handle to this DLL itself.
int                 g_cAprInit = 0;
DWORD               g_langid;
DWORD               g_langTimeout = 0;
HINSTANCE           g_hResInst = NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /* lpReserved */)
{
    // NOTE: Do *NOT* call apr_initialize() or apr_terminate() here in DllMain(),
    // because those functions call LoadLibrary() indirectly through malloc().
    // And LoadLibrary() inside DllMain() is not allowed and can lead to unexpected
    // behavior and even may create dependency loops in the dll load order.
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (g_hmodThisDll == NULL)
        {
            // g_csGlobalCOMGuard.Init();
        }

        // Extension DLL one-time initialization
        g_hmodThisDll = hInstance;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        // do not clean up memory here:
        // if an application doesn't release all COM objects
        // but still unloads the dll, cleaning up ourselves
        // will lead to crashes.
        // better to leak some memory than to crash other apps.
        // sometimes an application doesn't release all COM objects
        // but still unloads the dll.
        // in that case, we do it ourselves

        // g_csGlobalCOMGuard.Term();
    }
    return 1;   // ok
}

STDAPI DllCanUnloadNow(void)
{
    return (g_cRefThisDll == 0 ? S_OK : S_FALSE);
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
{
    if (ppvOut == 0 )
        return E_POINTER;
    *ppvOut = NULL;

    // FileState state = FileStateInvalid;
    // if (IsEqualIID(rclsid, CLSID_TortoiseSVN_UPTODATE))
    //     state = FileStateVersioned;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_MODIFIED))
    //     state = FileStateModified;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_CONFLICTING))
    //     state = FileStateConflict;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_UNCONTROLLED))
    //     state = FileStateUncontrolled;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_DROPHANDLER))
    //     state = FileStateDropHandler;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_DELETED))
    //     state = FileStateDeleted;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_READONLY))
    //     state = FileStateReadOnly;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_LOCKED))
    //     state = FileStateLockedOverlay;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_ADDED))
    //     state = FileStateAddedOverlay;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_IGNORED))
    //     state = FileStateIgnoredOverlay;
    // else if (IsEqualIID(rclsid, CLSID_TortoiseSVN_UNVERSIONED))
    //     state = FileStateUnversionedOverlay;

    // if (state != FileStateInvalid)
    // {
    //     apr_initialize();
    //     svn_dso_initialize2();
    //     g_SVNAdminDir.Init();
    //     g_cAprInit++;

    //     CShellExtClassFactory *pcf = new (std::nothrow) CShellExtClassFactory(state);
    //     if (pcf == NULL)
    //         return E_OUTOFMEMORY;
    //     // refcount currently set to 0
    //     const HRESULT hr = pcf->QueryInterface(riid, ppvOut);
    //     if(FAILED(hr))
    //         delete pcf;
    //     return hr;
    // }

    return CLASS_E_CLASSNOTAVAILABLE;

}
