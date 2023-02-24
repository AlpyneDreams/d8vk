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

#include "..\wrapper\ddraw.h"

namespace dxvk {

  HRESULT D3D7Interface::QueryInterface(REFIID riid, LPVOID *ppvObj) {
    return ProxyInterface->QueryInterface(riid, ppvObj);
  }

  ULONG D3D7Interface::AddRef() {
    return ProxyInterface->AddRef();
  }

  ULONG D3D7Interface::Release() {
    return ProxyInterface->Release();
  }

  HRESULT D3D7Interface::EnumDevices(LPD3DENUMDEVICESCALLBACK7 a, LPVOID b) {
    return ProxyInterface->EnumDevices7(a, b);
  }

  HRESULT D3D7Interface::CreateDevice(REFCLSID a, LPDIRECTDRAWSURFACE7 b, LPDIRECT3DDEVICE7 *c) {
    return ProxyInterface->CreateDevice(a, b, c, nullptr);
  }

  HRESULT D3D7Interface::CreateVertexBuffer(LPD3DVERTEXBUFFERDESC a, LPDIRECT3DVERTEXBUFFER7 *b, DWORD c) {
    return ProxyInterface->CreateVertexBuffer(a, b, c, nullptr);
  }

  HRESULT D3D7Interface::EnumZBufferFormats(REFCLSID a, LPD3DENUMPIXELFORMATSCALLBACK b, LPVOID c) {
    return ProxyInterface->EnumZBufferFormats(a, b, c);
  }

  HRESULT D3D7Interface::EvictManagedTextures() {
    return ProxyInterface->EvictManagedTextures();
  }

}