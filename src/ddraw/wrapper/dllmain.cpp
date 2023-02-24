/**
* Copyright (C) 2020 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "ddraw.h"

#define D3DERR_COMMAND_UNPARSED              MAKE_DDHRESULT(3000)

std::ofstream Log::LOG("ddraw.log");
AddressLookupTable<void> ProxyAddressLookupTable = AddressLookupTable<void>();

AcquireDDThreadLockProc m_pAcquireDDThreadLock;
CompleteCreateSysmemSurfaceProc m_pCompleteCreateSysmemSurface;
D3DParseUnknownCommandProc m_pD3DParseUnknownCommand;
DDGetAttachedSurfaceLclProc m_pDDGetAttachedSurfaceLcl;
DDInternalLockProc m_pDDInternalLock;
DDInternalUnlockProc m_pDDInternalUnlock;
DSoundHelpProc m_pDSoundHelp;
DirectDrawCreateProc m_pDirectDrawCreate;
DirectDrawCreateClipperProc m_pDirectDrawCreateClipper;
DirectDrawCreateExProc m_pDirectDrawCreateEx;
DirectDrawEnumerateAProc m_pDirectDrawEnumerateA;
DirectDrawEnumerateExAProc m_pDirectDrawEnumerateExA;
DirectDrawEnumerateExWProc m_pDirectDrawEnumerateExW;
DirectDrawEnumerateWProc m_pDirectDrawEnumerateW;
DllCanUnloadNowProc m_pDllCanUnloadNow;
DllGetClassObjectProc m_pDllGetClassObject;
GetDDSurfaceLocalProc m_pGetDDSurfaceLocal;
GetOLEThunkDataProc m_pGetOLEThunkData;
GetSurfaceFromDCProc m_pGetSurfaceFromDC;
RegisterSpecialCaseProc m_pRegisterSpecialCase;
ReleaseDDThreadLockProc m_pReleaseDDThreadLock;
SetAppCompatDataProc m_pSetAppCompatData;

bool _stdcall DllMain(HANDLE, DWORD dwReason, LPVOID)
{
	static HMODULE ddrawdll = nullptr;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Load dll
		char path[MAX_PATH];
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\ddraw.dll");
		Log() << "Loading " << path;
		ddrawdll = LoadLibraryA(path);

		// Get function addresses
		m_pAcquireDDThreadLock = (AcquireDDThreadLockProc)GetProcAddress(ddrawdll, "AcquireDDThreadLock");
		m_pCompleteCreateSysmemSurface = (CompleteCreateSysmemSurfaceProc)GetProcAddress(ddrawdll, "CompleteCreateSysmemSurface");
		m_pD3DParseUnknownCommand = (D3DParseUnknownCommandProc)GetProcAddress(ddrawdll, "D3DParseUnknownCommand");
		m_pDDGetAttachedSurfaceLcl = (DDGetAttachedSurfaceLclProc)GetProcAddress(ddrawdll, "DDGetAttachedSurfaceLcl");
		m_pDDInternalLock = (DDInternalLockProc)GetProcAddress(ddrawdll, "DDInternalLock");
		m_pDDInternalUnlock = (DDInternalUnlockProc)GetProcAddress(ddrawdll, "DDInternalUnlock");
		m_pDSoundHelp = (DSoundHelpProc)GetProcAddress(ddrawdll, "DSoundHelp");
		m_pDirectDrawCreate = (DirectDrawCreateProc)GetProcAddress(ddrawdll, "DirectDrawCreate");
		m_pDirectDrawCreateClipper = (DirectDrawCreateClipperProc)GetProcAddress(ddrawdll, "DirectDrawCreateClipper");
		m_pDirectDrawCreateEx = (DirectDrawCreateExProc)GetProcAddress(ddrawdll, "DirectDrawCreateEx");
		m_pDirectDrawEnumerateA = (DirectDrawEnumerateAProc)GetProcAddress(ddrawdll, "DirectDrawEnumerateA");
		m_pDirectDrawEnumerateExA = (DirectDrawEnumerateExAProc)GetProcAddress(ddrawdll, "DirectDrawEnumerateExA");
		m_pDirectDrawEnumerateExW = (DirectDrawEnumerateExWProc)GetProcAddress(ddrawdll, "DirectDrawEnumerateExW");
		m_pDirectDrawEnumerateW = (DirectDrawEnumerateWProc)GetProcAddress(ddrawdll, "DirectDrawEnumerateW");
		m_pDllCanUnloadNow = (DllCanUnloadNowProc)GetProcAddress(ddrawdll, "DllCanUnloadNow");
		m_pDllGetClassObject = (DllGetClassObjectProc)GetProcAddress(ddrawdll, "DllGetClassObject");
		m_pGetDDSurfaceLocal = (GetDDSurfaceLocalProc)GetProcAddress(ddrawdll, "GetDDSurfaceLocal");
		m_pGetOLEThunkData = (GetOLEThunkDataProc)GetProcAddress(ddrawdll, "GetOLEThunkData");
		m_pGetSurfaceFromDC = (GetSurfaceFromDCProc)GetProcAddress(ddrawdll, "GetSurfaceFromDC");
		m_pRegisterSpecialCase = (RegisterSpecialCaseProc)GetProcAddress(ddrawdll, "RegisterSpecialCase");
		m_pReleaseDDThreadLock = (ReleaseDDThreadLockProc)GetProcAddress(ddrawdll, "ReleaseDDThreadLock");
		m_pSetAppCompatData = (SetAppCompatDataProc)GetProcAddress(ddrawdll, "SetAppCompatData");
		break;

	case DLL_PROCESS_DETACH:
		FreeLibrary(ddrawdll);
		break;
	}
	return true;
}

HRESULT WINAPI AcquireDDThreadLock()
{
	if (!m_pAcquireDDThreadLock)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pAcquireDDThreadLock();
}

DWORD WINAPI CompleteCreateSysmemSurface(DWORD arg)
{
	if (!m_pCompleteCreateSysmemSurface)
	{
		return NULL;
	}

	return m_pCompleteCreateSysmemSurface(arg);
}

HRESULT WINAPI D3DParseUnknownCommand(LPVOID lpCmd, LPVOID *lpRetCmd)
{
	if (!m_pD3DParseUnknownCommand)
	{
		return D3DERR_COMMAND_UNPARSED;
	}

	return m_pD3DParseUnknownCommand(lpCmd, lpRetCmd);
}

HRESULT WINAPI DDGetAttachedSurfaceLcl(DWORD arg1, DWORD arg2, DWORD arg3)
{
	if (!m_pDDGetAttachedSurfaceLcl)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pDDGetAttachedSurfaceLcl(arg1, arg2, arg3);
}

DWORD WINAPI DDInternalLock(DWORD arg1, DWORD arg2)
{
	if (!m_pDDInternalLock)
	{
		return 0xFFFFFFFF;
	}

	return m_pDDInternalLock(arg1, arg2);
}

DWORD WINAPI DDInternalUnlock(DWORD arg)
{
	if (!m_pDDInternalUnlock)
	{
		return 0xFFFFFFFF;
	}

	return m_pDDInternalUnlock(arg);
}

HRESULT WINAPI DSoundHelp(DWORD arg1, DWORD arg2, DWORD arg3)
{
	if (!m_pDSoundHelp)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pDSoundHelp(arg1, arg2, arg3);
}

HRESULT WINAPI DirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter)
{
	if (!m_pDirectDrawCreate)
	{
		return E_FAIL;
	}

	HRESULT hr = m_pDirectDrawCreate(lpGUID, lplpDD, pUnkOuter);

	if (SUCCEEDED(hr) && lplpDD)
	{
		*lplpDD = new m_IDirectDraw(*lplpDD);
	}

	return hr;
}

HRESULT WINAPI DirectDrawCreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER *lplpDDClipper, LPUNKNOWN pUnkOuter)
{
	if (!m_pDirectDrawCreateClipper)
	{
		return E_FAIL;
	}

	HRESULT hr = m_pDirectDrawCreateClipper(dwFlags, lplpDDClipper, pUnkOuter);

	if (SUCCEEDED(hr) && lplpDDClipper)
	{
		*lplpDDClipper = new m_IDirectDrawClipper(*lplpDDClipper);
	}

	return hr;
}

HRESULT WINAPI DirectDrawCreateEx(GUID FAR *lpGUID, LPVOID *lplpDD, REFIID riid, IUnknown FAR *pUnkOuter)
{
	if (!m_pDirectDrawCreateEx)
	{
		return E_FAIL;
	}

	HRESULT hr = m_pDirectDrawCreateEx(lpGUID, lplpDD, riid, pUnkOuter);

	if (SUCCEEDED(hr) && lplpDD)
	{
		genericQueryInterface(riid, lplpDD);
	}

	return hr;
}

HRESULT WINAPI DirectDrawEnumerateA(LPDDENUMCALLBACKA lpCallback, LPVOID lpContext)
{
	if (!m_pDirectDrawEnumerateA)
	{
		return E_FAIL;
	}

	return m_pDirectDrawEnumerateA(lpCallback, lpContext);
}

HRESULT WINAPI DirectDrawEnumerateExA(LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags)
{
	if (!m_pDirectDrawEnumerateExA)
	{
		return E_FAIL;
	}

	return m_pDirectDrawEnumerateExA(lpCallback, lpContext, dwFlags);
}

HRESULT WINAPI DirectDrawEnumerateExW(LPDDENUMCALLBACKEXW lpCallback, LPVOID lpContext, DWORD dwFlags)
{
	if (!m_pDirectDrawEnumerateExW)
	{
		return E_FAIL;
	}

	return m_pDirectDrawEnumerateExW(lpCallback, lpContext, dwFlags);
}

HRESULT WINAPI DirectDrawEnumerateW(LPDDENUMCALLBACKW lpCallback, LPVOID lpContext)
{
	if (!m_pDirectDrawEnumerateW)
	{
		return E_FAIL;
	}

	return m_pDirectDrawEnumerateW(lpCallback, lpContext);
}

HRESULT WINAPI DllCanUnloadNow()
{
	if (!m_pDllCanUnloadNow)
	{
		return E_FAIL;
	}

	return m_pDllCanUnloadNow();
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
	if (!m_pDllGetClassObject)
	{
		return E_FAIL;
	}

	HRESULT hr = m_pDllGetClassObject(rclsid, riid, ppv);

	if (SUCCEEDED(hr))
	{
		genericQueryInterface(riid, ppv);
	}

	return hr;
}

HRESULT WINAPI GetDDSurfaceLocal(DWORD arg1, DWORD arg2, DWORD arg3)
{
	if (!m_pGetDDSurfaceLocal)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pGetDDSurfaceLocal(arg1, arg2, arg3);
}

DWORD WINAPI GetOLEThunkData(DWORD index)
{
	if (!m_pGetOLEThunkData)
	{
		return NULL;
	}

	return m_pGetOLEThunkData(index);
}

extern "C" HRESULT WINAPI GetSurfaceFromDC(HDC hdc, LPDIRECTDRAWSURFACE7 *lpDDS, DWORD arg)
{
	if (!m_pGetSurfaceFromDC)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pGetSurfaceFromDC(hdc, lpDDS, arg);
}

HRESULT WINAPI RegisterSpecialCase(DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4)
{
	if (!m_pRegisterSpecialCase)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pRegisterSpecialCase(arg1, arg2, arg3, arg4);
}

HRESULT WINAPI ReleaseDDThreadLock()
{
	if (!m_pReleaseDDThreadLock)
	{
		return DDERR_UNSUPPORTED;
	}

	return m_pReleaseDDThreadLock();
}

HRESULT WINAPI SetAppCompatData(DWORD Type, DWORD Value)
{
	if (!m_pSetAppCompatData)
	{
		return E_FAIL;
	}

	return m_pSetAppCompatData(Type, Value);
}
