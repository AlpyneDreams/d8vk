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

#include "../wrapper/ddraw.h"

namespace dxvk {

  HRESULT D3D7VertexBuffer::QueryInterface(REFIID riid, LPVOID *ppvObj) {
    return ProxyInterface->QueryInterface(riid, ppvObj);
  }

  ULONG D3D7VertexBuffer::AddRef() {
    return ProxyInterface->AddRef();
  }

  ULONG D3D7VertexBuffer::Release() {
    return ProxyInterface->Release();
  }

  HRESULT D3D7VertexBuffer::Lock(DWORD a, LPVOID *b, LPDWORD c) {
    return ProxyInterface->Lock(a, b, c);
  }

  HRESULT D3D7VertexBuffer::Unlock() {
    return ProxyInterface->Unlock();
  }

  HRESULT D3D7VertexBuffer::ProcessVertices(DWORD a, DWORD b, DWORD c, LPDIRECT3DVERTEXBUFFER7 d, DWORD e, LPDIRECT3DDEVICE7 f, DWORD g) {
    return ProxyInterface->ProcessVertices(a, b, c, d, e, f, g);
  }

  HRESULT D3D7VertexBuffer::GetVertexBufferDesc(LPD3DVERTEXBUFFERDESC a) {
    return ProxyInterface->GetVertexBufferDesc(a);
  }

  HRESULT D3D7VertexBuffer::Optimize(LPDIRECT3DDEVICE7 a, DWORD b) {
    return ProxyInterface->Optimize(a, b);
  }

  HRESULT D3D7VertexBuffer::ProcessVerticesStrided(DWORD a, DWORD b, DWORD c, LPD3DDRAWPRIMITIVESTRIDEDDATA d, DWORD e, LPDIRECT3DDEVICE7 f, DWORD g) {
    return ProxyInterface->ProcessVerticesStrided(a, b, c, d, e, f, g);
  }

}