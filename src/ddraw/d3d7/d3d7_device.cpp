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

  HRESULT D3D7Device::QueryInterface(REFIID riid, LPVOID *ppvObj) {
    return ProxyInterface->QueryInterface(riid, ppvObj);
  }

  ULONG D3D7Device::AddRef() {
    return ProxyInterface->AddRef();
  }

  ULONG D3D7Device::Release() {
    return ProxyInterface->Release();
  }

  HRESULT D3D7Device::GetCaps(LPD3DDEVICEDESC7 a) {
    return ProxyInterface->GetCaps(a);
  }

  HRESULT D3D7Device::EnumTextureFormats(LPD3DENUMPIXELFORMATSCALLBACK a, LPVOID b) {
    return ProxyInterface->EnumTextureFormats(a, b);
  }

  HRESULT D3D7Device::BeginScene() {
    return ProxyInterface->BeginScene();
  }

  HRESULT D3D7Device::EndScene() {
    return ProxyInterface->EndScene();
  }

  HRESULT D3D7Device::GetDirect3D(LPDIRECT3D7 *a) {
    return ProxyInterface->GetDirect3D(a);
  }

  HRESULT D3D7Device::SetRenderTarget(LPDIRECTDRAWSURFACE7 a, DWORD b) {
    return ProxyInterface->SetRenderTarget(a, b);
  }

  HRESULT D3D7Device::GetRenderTarget(LPDIRECTDRAWSURFACE7 *a) {
    return ProxyInterface->GetRenderTarget(a);
  }

  HRESULT D3D7Device::Clear(DWORD a, LPD3DRECT b, DWORD c, D3DCOLOR d, D3DVALUE e, DWORD f) {
    return ProxyInterface->Clear(a, b, c, d, e, f);
  }

  HRESULT D3D7Device::SetTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b) {
    return ProxyInterface->SetTransform(a, b);
  }

  HRESULT D3D7Device::GetTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b) {
    return ProxyInterface->GetTransform(a, b);
  }

  HRESULT D3D7Device::SetViewport(LPD3DVIEWPORT7 a) {
    return ProxyInterface->SetViewport(a);
  }

  HRESULT D3D7Device::MultiplyTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b) {
    return ProxyInterface->MultiplyTransform(a, b);
  }

  HRESULT D3D7Device::GetViewport(LPD3DVIEWPORT7 a) {
    return ProxyInterface->GetViewport(a);
  }

  HRESULT D3D7Device::SetMaterial(LPD3DMATERIAL7 a) {
    return ProxyInterface->SetMaterial(a);
  }

  HRESULT D3D7Device::GetMaterial(LPD3DMATERIAL7 a) {
    return ProxyInterface->GetMaterial(a);
  }

  HRESULT D3D7Device::SetLight(DWORD a, LPD3DLIGHT7 b) {
    return ProxyInterface->SetLight(a, b);
  }

  HRESULT D3D7Device::GetLight(DWORD a, LPD3DLIGHT7 b) {
    return ProxyInterface->GetLight(a, b);
  }

  HRESULT D3D7Device::SetRenderState(D3DRENDERSTATETYPE a, DWORD b) {
    return ProxyInterface->SetRenderState(a, b);
  }

  HRESULT D3D7Device::GetRenderState(D3DRENDERSTATETYPE a, LPDWORD b) {
    return ProxyInterface->GetRenderState(a, b);
  }

  HRESULT D3D7Device::BeginStateBlock() {
    return ProxyInterface->BeginStateBlock();
  }

  HRESULT D3D7Device::EndStateBlock(LPDWORD a) {
    return ProxyInterface->EndStateBlock(a);
  }

  HRESULT D3D7Device::PreLoad(LPDIRECTDRAWSURFACE7 a) {
    return ProxyInterface->PreLoad(a);
  }

  HRESULT D3D7Device::DrawPrimitive(D3DPRIMITIVETYPE a, DWORD b, LPVOID c, DWORD d, DWORD e) {
    return ProxyInterface->DrawPrimitive(a, b, c, d, e);
  }

  HRESULT D3D7Device::DrawIndexedPrimitive(D3DPRIMITIVETYPE a, DWORD b, LPVOID c, DWORD d, LPWORD e, DWORD f, DWORD g) {
    return ProxyInterface->DrawIndexedPrimitive(a, b, c, d, e, f, g);
  }

  HRESULT D3D7Device::SetClipStatus(LPD3DCLIPSTATUS a) {
    return ProxyInterface->SetClipStatus(a);
  }

  HRESULT D3D7Device::GetClipStatus(LPD3DCLIPSTATUS a) {
    return ProxyInterface->GetClipStatus(a);
  }

  HRESULT D3D7Device::DrawPrimitiveStrided(D3DPRIMITIVETYPE a, DWORD b, LPD3DDRAWPRIMITIVESTRIDEDDATA c, DWORD d,
                                           DWORD e) {
    return ProxyInterface->DrawPrimitiveStrided(a, b, c, d, e);
  }

  HRESULT D3D7Device::DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE a, DWORD b, LPD3DDRAWPRIMITIVESTRIDEDDATA c, DWORD d,
                                                  LPWORD e, DWORD f, DWORD g) {
    return ProxyInterface->DrawIndexedPrimitiveStrided(a, b, c, d, e, f, g);
  }

  HRESULT D3D7Device::DrawPrimitiveVB(D3DPRIMITIVETYPE a, LPDIRECT3DVERTEXBUFFER7 b, DWORD c, DWORD d, DWORD e) {
    return ProxyInterface->DrawPrimitiveVB(a, b, c, d, e);
  }

  HRESULT D3D7Device::DrawIndexedPrimitiveVB(D3DPRIMITIVETYPE a, LPDIRECT3DVERTEXBUFFER7 b, DWORD c, DWORD d, LPWORD e,
                                             DWORD f, DWORD g) {
    return ProxyInterface->DrawIndexedPrimitiveVB(a, b, c, d, e, f, g);
  }

  HRESULT D3D7Device::ComputeSphereVisibility(LPD3DVECTOR a, LPD3DVALUE b, DWORD c, DWORD d, LPDWORD e) {
    return ProxyInterface->ComputeSphereVisibility(a, b, c, d, e);
  }

  HRESULT D3D7Device::GetTexture(DWORD a, LPDIRECTDRAWSURFACE7 *b) {
    return ProxyInterface->GetTexture(a, b);
  }

  HRESULT D3D7Device::SetTexture(DWORD a, LPDIRECTDRAWSURFACE7 b) {
    return ProxyInterface->SetTexture(a, b);
  }

  HRESULT D3D7Device::GetTextureStageState(DWORD a, D3DTEXTURESTAGESTATETYPE b, LPDWORD c) {
    return ProxyInterface->GetTextureStageState(a, b, c);
  }

  HRESULT D3D7Device::SetTextureStageState(DWORD a, D3DTEXTURESTAGESTATETYPE b, DWORD c) {
    return ProxyInterface->SetTextureStageState(a, b, c);
  }

  HRESULT D3D7Device::ValidateDevice(LPDWORD a) {
    return ProxyInterface->ValidateDevice(a);
  }

  HRESULT D3D7Device::ApplyStateBlock(DWORD a) {
    return ProxyInterface->ApplyStateBlock(a);
  }

  HRESULT D3D7Device::CaptureStateBlock(DWORD a) {
    return ProxyInterface->CaptureStateBlock(a);
  }

  HRESULT D3D7Device::DeleteStateBlock(DWORD a) {
    return ProxyInterface->DeleteStateBlock(a);
  }

  HRESULT D3D7Device::CreateStateBlock(D3DSTATEBLOCKTYPE a, LPDWORD b) {
    return ProxyInterface->CreateStateBlock(a, b);
  }

  HRESULT D3D7Device::Load(LPDIRECTDRAWSURFACE7 a, LPPOINT b, LPDIRECTDRAWSURFACE7 c, LPRECT d, DWORD e) {
    return ProxyInterface->Load(a, b, c, d, e);
  }

  HRESULT D3D7Device::LightEnable(DWORD a, BOOL b) {
    return ProxyInterface->LightEnable(a, b);
  }

  HRESULT D3D7Device::GetLightEnable(DWORD a, BOOL *b) {
    return ProxyInterface->GetLightEnable(a, b);
  }

  HRESULT D3D7Device::SetClipPlane(DWORD a, D3DVALUE *b) {
    return ProxyInterface->SetClipPlane(a, b);
  }

  HRESULT D3D7Device::GetClipPlane(DWORD a, D3DVALUE *b) {
    return ProxyInterface->GetClipPlane(a, b);
  }

  HRESULT D3D7Device::GetInfo(DWORD a, LPVOID b, DWORD c) {
    return ProxyInterface->GetInfo(a, b, c);
  }

}