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

HRESULT m_IDirect3DMaterialX::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return ProxyQueryInterface(ProxyInterface, riid, ppvObj, WrapperID, WrapperInterface);
}

ULONG m_IDirect3DMaterialX::AddRef()
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DMaterialX::Release()
{
	ULONG ref = ProxyInterface->Release();

	if (ref == 0)
	{
		WrapperInterface->DeleteMe();
	}

	return ref;
}

HRESULT m_IDirect3DMaterialX::Initialize(LPDIRECT3D lplpD3D)
{
	if (lplpD3D)
	{
		lplpD3D = static_cast<m_IDirect3D *>(lplpD3D)->GetProxyInterface();
	}

	return GetProxyInterfaceV1()->Initialize(lplpD3D);
}

HRESULT m_IDirect3DMaterialX::SetMaterial(LPD3DMATERIAL lpMat)
{
	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->SetMaterial(lpMat);
	case 2:
		return GetProxyInterfaceV2()->SetMaterial(lpMat);
	case 3:
		return GetProxyInterfaceV3()->SetMaterial(lpMat);
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DMaterialX::GetMaterial(LPD3DMATERIAL lpMat)
{
	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->GetMaterial(lpMat);
	case 2:
		return GetProxyInterfaceV2()->GetMaterial(lpMat);
	case 3:
		return GetProxyInterfaceV3()->GetMaterial(lpMat);
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DMaterialX::GetHandle(LPDIRECT3DDEVICE3 lpDirect3DDevice, LPD3DMATERIALHANDLE lpHandle)
{
	if (lpDirect3DDevice)
	{
		lpDirect3DDevice = static_cast<m_IDirect3DDevice3 *>(lpDirect3DDevice)->GetProxyInterface();
	}

	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->GetHandle((LPDIRECT3DDEVICE)lpDirect3DDevice, lpHandle);
	case 2:
		return GetProxyInterfaceV2()->GetHandle((LPDIRECT3DDEVICE2)lpDirect3DDevice, lpHandle);
	case 3:
		return GetProxyInterfaceV3()->GetHandle(lpDirect3DDevice, lpHandle);
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DMaterialX::Reserve()
{
	return GetProxyInterfaceV1()->Reserve();
}

HRESULT m_IDirect3DMaterialX::Unreserve()
{
	return GetProxyInterfaceV1()->Unreserve();
}
