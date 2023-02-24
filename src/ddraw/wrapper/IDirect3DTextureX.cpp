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

HRESULT m_IDirect3DTextureX::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return ProxyQueryInterface(ProxyInterface, riid, ppvObj, WrapperID, WrapperInterface);
}

ULONG m_IDirect3DTextureX::AddRef()
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DTextureX::Release()
{
	ULONG ref = ProxyInterface->Release();

	if (ref == 0)
	{
		WrapperInterface->DeleteMe();
	}

	return ref;
}

HRESULT m_IDirect3DTextureX::Initialize(LPDIRECT3DDEVICE lpDirect3DDevice, LPDIRECTDRAWSURFACE lplpDDSurface)
{
	if (lpDirect3DDevice)
	{
		lpDirect3DDevice = static_cast<m_IDirect3DDevice *>(lpDirect3DDevice)->GetProxyInterface();
	}
	if (lplpDDSurface)
	{
		lplpDDSurface = static_cast<m_IDirectDrawSurface *>(lplpDDSurface)->GetProxyInterface();
	}

	return GetProxyInterfaceV1()->Initialize(lpDirect3DDevice, lplpDDSurface);
}

HRESULT m_IDirect3DTextureX::GetHandle(LPDIRECT3DDEVICE2 lpDirect3DDevice2, LPD3DTEXTUREHANDLE lpHandle)
{
	if (lpDirect3DDevice2)
	{
		lpDirect3DDevice2 = static_cast<m_IDirect3DDevice2 *>(lpDirect3DDevice2)->GetProxyInterface();
	}

	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->GetHandle((LPDIRECT3DDEVICE)lpDirect3DDevice2, lpHandle);
	case 2:
		return GetProxyInterfaceV2()->GetHandle(lpDirect3DDevice2, lpHandle);
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DTextureX::PaletteChanged(DWORD dwStart, DWORD dwCount)
{
	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->PaletteChanged(dwStart, dwCount);
	case 2:
		return GetProxyInterfaceV2()->PaletteChanged(dwStart, dwCount);
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DTextureX::Load(LPDIRECT3DTEXTURE2 lpD3DTexture2)
{
	if (lpD3DTexture2)
	{
		lpD3DTexture2 = static_cast<m_IDirect3DTexture2 *>(lpD3DTexture2)->GetProxyInterface();
	}

	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->Load((LPDIRECT3DTEXTURE)lpD3DTexture2);
	case 2:
		return GetProxyInterfaceV2()->Load(lpD3DTexture2);
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DTextureX::Unload()
{
	return GetProxyInterfaceV1()->Unload();
}
