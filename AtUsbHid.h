#ifndef _ATUSBHID_H_
#define _ATUSBHID_H_

// Error codes.
#define ERROR_USB_DEVICE_NOT_FOUND          0xE0000001
#define ERROR_USB_DEVICE_NO_CAPABILITIES    0xE0000002

// name of the DLL to be loaded
#define AT_USB_HID_DLL (_T("AtUsbHid"))

// Implement the DLL export/import mechanism and allow a C-written program
// to use our DLL.
#ifdef ATUSBHID_EXPORTS
#define ATUSBHID_API extern "C" __declspec(dllexport)
#else
#define ATUSBHID_API extern "C" __declspec(dllimport)
#endif


// This macro function calls the C runtime's _beginthreadex function. 
// The C runtime library doesn't want to have any reliance on Windows' data 
// types such as HANDLE. This means that a Windows programmer needs to cast
// values when using _beginthreadex. Since this is terribly inconvenient, 
// this macro has been created to perform the casting.
typedef unsigned(__stdcall *PTHREAD_START)(void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                      \
         (void *)        (psa),                     \
         (unsigned)      (cbStack),                 \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *)        (pvParam),                 \
         (unsigned)      (fdwCreate),               \
(unsigned *)(pdwThreadId)))

#if 0

// Allow applications not built with Microsoft Visual C++ to link with our DLL.
#define STDCALL __stdcall

// These macros make calling our DLL functions through pointers easier.
#define DECLARE_FUNCTION_POINTER(FUNC)  PF_##FUNC lp##FUNC=NULL;  
#define LOAD_FUNCTION_POINTER(DLL,FUNC) lp##FUNC = (PF_##FUNC)GetProcAddress(DLL, #FUNC);
#define ADDR_CHECK(FUNC) if (lp##FUNC == NULL) {_ftprintf_s(stderr, _T("%s\n"), _T("Error: Cannot get address of function.")); return FALSE;}
#define DYNCALL(FUNC) lp##FUNC

#else

#ifndef NEXTUNE_DLL_MACROS
#define NEXTUNE_DLL_MACROS

// Allow applications not built with Microsoft Visual C++ to link with our DLL.
#define STDCALL __stdcall

// These macros make calling our DLL functions through pointers easier.
#define DECLARE_FUNCTION_POINTER(FUNC)	static PF_##FUNC lp##FUNC=NULL;  
#define LOAD_FUNCTION_POINTER(DLL,FUNC)	lp##FUNC = (PF_##FUNC)GetProcAddress(DLL, #FUNC);
#define ADDR_CHECK(FUNC)				if (lp##FUNC == NULL) {_ftprintf_s(stderr, _T("%s\n"), _T("Error: Cannot get address of function.")); return FALSE;}
#define DYNCALL(FUNC)					lp##FUNC

// 110426, Ken Added, Used in multi files
#define LOAD_FUNC(DLL,FUNC)		 		PF_##FUNC lp##FUNC = (PF_##FUNC)GetProcAddress(DLL, #FUNC);		\
										if(lp##FUNC == NULL)											\
										{																\
											TCHAR tszErr[MAXCHAR] = {0};								\
											TCHAR tszFunc[MAXCHAR] = {0};								\
											mbstowcs_s(NULL, tszFunc, #FUNC, MAXCHAR);					\
											_stprintf_s(tszErr, _T("Cannot get address of %s() function. Please check the related DLL exists."), tszFunc);		\
											AfxMessageBox(tszErr);										\
																										\
											ExitProcess(-1);											\
										}																
									

// LOAD_FUNC() macro must be called before RUN_FUNC macro call.
#define RUN_FUNC(FUNC)					(lp##FUNC)

#endif /* NEXTUNE_DLL_MACROS */

#endif /* 0 */


///////////////////////////////////////////////////////////////////////////////
typedef BOOLEAN (STDCALL *PF_findHidDevice)(const UINT VendorID, const UINT ProductID);
typedef void    (STDCALL *PF_closeDevice)(void);
typedef BOOLEAN (STDCALL *PF_writeData)(UCHAR* buffer);
typedef BOOLEAN (STDCALL *PF_readData)(UCHAR* buffer);
typedef int     (STDCALL *PF_hidRegisterDeviceNotification)(HWND hWnd);
typedef void    (STDCALL *PF_hidUnregisterDeviceNotification)(HWND hWnd);
typedef int     (STDCALL *PF_isMyDeviceNotification)(DWORD dwData);
typedef BOOLEAN (STDCALL *PF_setFeature)(UCHAR* buffer);
typedef int     (STDCALL *PF_getFeatureReportLength)();
typedef int     (STDCALL *PF_getInputReportLength)();
typedef int     (STDCALL *PF_getOutputReportLength)();

///////////////////////////////////////////////////////////////////////////////

// Exported functions prototypes.

///////////////////////////////////////////////////////////////////////////////
ATUSBHID_API BOOLEAN STDCALL findHidDevice(const UINT VendorID, const UINT ProductID);

//  Closes the USB device and all handles before exiting the application.
ATUSBHID_API void    STDCALL closeDevice(void);

ATUSBHID_API BOOLEAN STDCALL writeData(UCHAR* buf);

ATUSBHID_API BOOLEAN STDCALL readData(UCHAR* buffer);

ATUSBHID_API int     STDCALL hidRegisterDeviceNotification(HWND hWnd);

ATUSBHID_API void    STDCALL hidUnregisterDeviceNotification(HWND hWnd);

ATUSBHID_API int     STDCALL isMyDeviceNotification(DWORD dwData);

ATUSBHID_API BOOLEAN STDCALL setFeature(UCHAR *buffer);

ATUSBHID_API int     STDCALL getFeatureReportLength(void);

ATUSBHID_API int     STDCALL getOutputReportLength(void);

ATUSBHID_API int     STDCALL getInputReportLength(void);

///////////////////////////////////////////////////////////////////////////////

#ifndef ATUSBHID_EXPORTS


DECLARE_FUNCTION_POINTER(findHidDevice);
DECLARE_FUNCTION_POINTER(closeDevice);
DECLARE_FUNCTION_POINTER(writeData);
DECLARE_FUNCTION_POINTER(readData);
DECLARE_FUNCTION_POINTER(hidRegisterDeviceNotification);
DECLARE_FUNCTION_POINTER(hidUnregisterDeviceNotification);
DECLARE_FUNCTION_POINTER(isMyDeviceNotification);
DECLARE_FUNCTION_POINTER(setFeature);
DECLARE_FUNCTION_POINTER(getFeatureReportLength)
DECLARE_FUNCTION_POINTER(getOutputReportLength)
DECLARE_FUNCTION_POINTER(getInputReportLength)

// this function call all function available in the DLL *
static bool loadFuncPointers(HINSTANCE hLib)
{
    LOAD_FUNCTION_POINTER(hLib, findHidDevice);
    ADDR_CHECK(findHidDevice);

	LOAD_FUNCTION_POINTER(hLib, closeDevice);
    ADDR_CHECK(closeDevice);

	LOAD_FUNCTION_POINTER(hLib, writeData);
    ADDR_CHECK(writeData);

	LOAD_FUNCTION_POINTER(hLib, readData);
    ADDR_CHECK(readData);

	LOAD_FUNCTION_POINTER(hLib, hidRegisterDeviceNotification);
	ADDR_CHECK(hidRegisterDeviceNotification);

	LOAD_FUNCTION_POINTER(hLib, hidUnregisterDeviceNotification);
	ADDR_CHECK(hidUnregisterDeviceNotification);

	LOAD_FUNCTION_POINTER(hLib, isMyDeviceNotification);
	ADDR_CHECK(isMyDeviceNotification);

	LOAD_FUNCTION_POINTER(hLib, setFeature);
	ADDR_CHECK(setFeature);

	LOAD_FUNCTION_POINTER(hLib, getOutputReportLength);
	ADDR_CHECK(getOutputReportLength);

	LOAD_FUNCTION_POINTER(hLib, getInputReportLength);
	ADDR_CHECK(getInputReportLength);

	LOAD_FUNCTION_POINTER(hLib, getFeatureReportLength);
	ADDR_CHECK(getFeatureReportLength);

    return true;
}

#endif

#endif  // _ATUSBHID_H_
