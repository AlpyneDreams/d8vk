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

#include "wrapper\ddraw.h"
#include "dd7_util.h"

namespace dxvk {

  HRESULT DD7Surface::QueryInterface(REFIID riid, LPVOID *ppvObj) {
    return ProxyInterface->QueryInterface(riid, ppvObj);
  }

  ULONG DD7Surface::AddRef() {
    return ProxyInterface->AddRef();
  }

  ULONG DD7Surface::Release() {
    return ProxyInterface->Release();
  }

  HRESULT DD7Surface::AddAttachedSurface(LPDIRECTDRAWSURFACE7 a) {
    return ProxyInterface->AddAttachedSurface(a);
  }

  HRESULT DD7Surface::AddOverlayDirtyRect(LPRECT a) {
    return ProxyInterface->AddOverlayDirtyRect(a);
  }

  HRESULT DD7Surface::Blt(LPRECT a, LPDIRECTDRAWSURFACE7 b, LPRECT c, DWORD d, LPDDBLTFX e) {
    if (IsPrimary())
      return DD_OK;
    return ProxyInterface->Blt(a, b, c, d, e);
  }

  HRESULT DD7Surface::BltBatch(LPDDBLTBATCH a, DWORD b, DWORD c) {
    return ProxyInterface->BltBatch(a, b, c);
  }

  HRESULT DD7Surface::BltFast(DWORD a, DWORD b, LPDIRECTDRAWSURFACE7 c, LPRECT d, DWORD e) {
    return ProxyInterface->BltFast(a, b, c, d, e);
  }

  HRESULT DD7Surface::DeleteAttachedSurface(DWORD a, LPDIRECTDRAWSURFACE7 b) {
    return ProxyInterface->DeleteAttachedSurface(a, b);
  }

  HRESULT DD7Surface::EnumAttachedSurfaces(LPVOID a, LPDDENUMSURFACESCALLBACK7 b) {
    return ProxyInterface->EnumAttachedSurfaces2(a, b);
  }

  HRESULT DD7Surface::EnumOverlayZOrders(DWORD a, LPVOID b, LPDDENUMSURFACESCALLBACK7 c) {
    return ProxyInterface->EnumOverlayZOrders2(a, b, c);
  }

  HRESULT DD7Surface::Flip(LPDIRECTDRAWSURFACE7 a, DWORD b) {
    return ProxyInterface->Flip(a, b);
  }

  HRESULT DD7Surface::GetAttachedSurface(LPDDSCAPS2 a, LPDIRECTDRAWSURFACE7 FAR* b) {
    return ProxyInterface->GetAttachedSurface2(a, b);
  }

  HRESULT DD7Surface::GetBltStatus(DWORD a) {
    return ProxyInterface->GetBltStatus(a);
  }

  HRESULT DD7Surface::GetCaps(LPDDSCAPS2 a) {
    return ProxyInterface->GetCaps2(a);
  }

  HRESULT DD7Surface::GetClipper(LPDIRECTDRAWCLIPPER FAR* a) {
    return ProxyInterface->GetClipper(a);
  }

  HRESULT DD7Surface::GetColorKey(DWORD a, LPDDCOLORKEY b) {
    return ProxyInterface->GetColorKey(a, b);
  }

  HRESULT DD7Surface::GetDC(HDC FAR* a) {
    return ProxyInterface->GetDC(a);
  }

  HRESULT DD7Surface::GetFlipStatus(DWORD a) {
    return ProxyInterface->GetFlipStatus(a);
  }

  HRESULT DD7Surface::GetOverlayPosition(LPLONG a, LPLONG b) {
    return ProxyInterface->GetOverlayPosition(a, b);
  }

  HRESULT DD7Surface::GetPalette(LPDIRECTDRAWPALETTE FAR* a) {
    return ProxyInterface->GetPalette(a);
  }

  HRESULT DD7Surface::GetPixelFormat(LPDDPIXELFORMAT a) {
    return ProxyInterface->GetPixelFormat(a);
  }

  HRESULT DD7Surface::GetSurfaceDesc(LPDDSURFACEDESC2 a) {
    return ProxyInterface->GetSurfaceDesc2(a);
  }

  HRESULT DD7Surface::Initialize(LPDIRECTDRAW a, LPDDSURFACEDESC2 b) {
    return ProxyInterface->Initialize2(a, b);
  }

  HRESULT DD7Surface::IsLost() {
    return ProxyInterface->IsLost();
  }

  HRESULT DD7Surface::Lock(LPRECT a, LPDDSURFACEDESC2 b, DWORD c, HANDLE d) {
    return ProxyInterface->Lock2(a, b, c, d);
  }

  HRESULT DD7Surface::ReleaseDC(HDC dc) {
    return ProxyInterface->ReleaseDC(dc);
  }

  HRESULT DD7Surface::Restore() {
    return ProxyInterface->Restore();
  }

  HRESULT DD7Surface::SetClipper(LPDIRECTDRAWCLIPPER clipper) {
    if (clipper != nullptr && IsPrimary()) {
      Logger::info("Primary surface attached to clipper:");
      PrintSurfaceStructure(this, 1);
      s_primaryClipper = clipper;
    }
    return ProxyInterface->SetClipper(clipper);
  }

  HRESULT DD7Surface::SetColorKey(DWORD a, LPDDCOLORKEY b) {
    return ProxyInterface->SetColorKey(a, b);
  }

  HRESULT DD7Surface::SetOverlayPosition(LONG a, LONG b) {
    return ProxyInterface->SetOverlayPosition(a, b);
  }

  HRESULT DD7Surface::SetPalette(LPDIRECTDRAWPALETTE a) {
    return ProxyInterface->SetPalette(a);
  }

  HRESULT DD7Surface::Unlock(LPRECT a) {
    return ProxyInterface->Unlock(a);
  }

  HRESULT DD7Surface::UpdateOverlay(LPRECT a, LPDIRECTDRAWSURFACE7 b, LPRECT c, DWORD d, LPDDOVERLAYFX e) {
    return ProxyInterface->UpdateOverlay(a, b, c, d, e);
  }

  HRESULT DD7Surface::UpdateOverlayDisplay(DWORD a) {
    return ProxyInterface->UpdateOverlayDisplay(a);
  }

  HRESULT DD7Surface::UpdateOverlayZOrder(DWORD a, LPDIRECTDRAWSURFACE7 b) {
    return ProxyInterface->UpdateOverlayZOrder(a, b);
  }

  HRESULT DD7Surface::GetDDInterface(LPVOID FAR* a) {
    return ProxyInterface->GetDDInterface(a);
  }

  HRESULT DD7Surface::PageLock(DWORD a) {
    return ProxyInterface->PageLock(a);
  }

  HRESULT DD7Surface::PageUnlock(DWORD a) {
    return ProxyInterface->PageUnlock(a);
  }

  HRESULT DD7Surface::SetSurfaceDesc(LPDDSURFACEDESC2 a, DWORD b) {
    return ProxyInterface->SetSurfaceDesc2(a, b);
  }

  HRESULT DD7Surface::SetPrivateData(REFGUID a, LPVOID b, DWORD c, DWORD d) {
    return ProxyInterface->SetPrivateData(a, b, c, d);
  }

  HRESULT DD7Surface::GetPrivateData(REFGUID a, LPVOID b, LPDWORD c) {
    return ProxyInterface->GetPrivateData(a, b, c);
  }

  HRESULT DD7Surface::FreePrivateData(REFGUID a) {
    return ProxyInterface->FreePrivateData(a);
  }

  HRESULT DD7Surface::GetUniquenessValue(LPDWORD a) {
    return ProxyInterface->GetUniquenessValue(a);
  }

  HRESULT DD7Surface::ChangeUniquenessValue() {
    return ProxyInterface->ChangeUniquenessValue();
  }

  HRESULT DD7Surface::SetPriority(DWORD a) {
    return ProxyInterface->SetPriority(a);
  }

  HRESULT DD7Surface::GetPriority(LPDWORD a) {
    return ProxyInterface->GetPriority(a);
  }

  HRESULT DD7Surface::SetLOD(DWORD a) {
    return ProxyInterface->SetLOD(a);
  }

  HRESULT DD7Surface::GetLOD(LPDWORD a) {
    return ProxyInterface->GetLOD(a);
  }

}
