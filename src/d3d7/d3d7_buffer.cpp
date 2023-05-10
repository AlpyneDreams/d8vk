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

#include "../ddraw/wrapper/ddraw.h"

namespace dxvk {

  HRESULT D3D7VertexBuffer::Lock(DWORD Flags, LPVOID *Data, LPDWORD Size) {
    if (Size != nullptr)
      *Size = m_size;

    static_assert(D3DLOCK_READONLY    == DDLOCK_READONLY);
    static_assert(D3DLOCK_DISCARD     == DDLOCK_DISCARDCONTENTS);
    static_assert(D3DLOCK_NOOVERWRITE == DDLOCK_NOOVERWRITE);
    static_assert(D3DLOCK_NOSYSLOCK   == DDLOCK_NOSYSLOCK);

    if (!(Flags & DDLOCK_WAIT))
      Flags |= D3DLOCK_DONOTWAIT;

    return GetD3D9()->Lock(0, 0, Data, Flags);
  }

  HRESULT D3D7VertexBuffer::Unlock() {
    return GetD3D9()->Unlock();
  }

  HRESULT D3D7VertexBuffer::ProcessVertices(
      DWORD VertexOp,
      DWORD DestIndex,
      DWORD Count,
      LPDIRECT3DVERTEXBUFFER7 VB,
      DWORD SrcIndex,
      LPDIRECT3DDEVICE7 Device,
      DWORD Flags) {
    D3D7Device* device = static_cast<D3D7Device*>(Device);
    D3D7VertexBuffer* src = static_cast<D3D7VertexBuffer*>(VB);
    // TODO: VertexOp
    device->GetD3D9()->SetStreamSource(0, src->GetD3D9(), 0, m_stride);
    return device->GetD3D9()->ProcessVertices(
      SrcIndex,
      DestIndex,
      Count,
      GetD3D9(),
      nullptr,
      Flags);
  }

  HRESULT D3D7VertexBuffer::GetVertexBufferDesc(LPD3DVERTEXBUFFERDESC pDesc) {
    ProxyInterface->GetVertexBufferDesc(pDesc);
    pDesc->dwSize         = sizeof(D3DVERTEXBUFFERDESC);
    pDesc->dwCaps         = 0;        // TODO: VertexBuffer dwCaps
    pDesc->dwFVF          = m_fvf;
    pDesc->dwNumVertices  = m_size / m_stride;
    return D3D_OK;
  } 

  HRESULT D3D7VertexBuffer::Optimize(LPDIRECT3DDEVICE7 Device, DWORD Flags) {
    // Sure thing, buddy
    return D3D_OK;
  }

  HRESULT D3D7VertexBuffer::ProcessVerticesStrided(
      DWORD VertexOp,
      DWORD DestIndex,
      DWORD Count,
      LPD3DDRAWPRIMITIVESTRIDEDDATA Data,
      DWORD SrcIndex,
      LPDIRECT3DDEVICE7 Device,
      DWORD Flags) {
    // TODO: ProcessVerticesStrided
    return ProxyInterface->ProcessVerticesStrided(VertexOp, DestIndex, Count, Data, SrcIndex, Device, Flags);
  }

}