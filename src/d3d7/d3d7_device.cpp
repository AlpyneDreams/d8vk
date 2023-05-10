#include "../ddraw/wrapper/ddraw.h"

#include "../ddraw/d3d_util.h"
#include "../d3d9/d3d9_bridge.h"

namespace dxvk {

  void D3D7Device::Setup() {
    GetD3D9()->GetRenderTarget(0, &m_initialRT);
    GetD3D9()->GetDepthStencilSurface(&m_initialDS);

    m_ibSize = 8 * 1024 * 1024;
    GetD3D9()->CreateIndexBuffer(m_ibSize, D3DUSAGE_DYNAMIC, d3d9::D3DFMT_INDEX16, d3d9::D3DPOOL_DEFAULT, &m_IB, nullptr);

    GetD3D9Bridge<D3D9Bridge>(GetD3D9())->SetAPIName("D3D7");
  }

  void D3D7Device::UploadIndices(void* indices, DWORD indexCount) {
    size_t size = indexCount * sizeof(WORD);

    if (size > m_ibSize) {
      Logger::err("DrawIndexedPrimitive: index buffer too small. Please make me bigger!");
    }

    void* pData = nullptr;
    m_IB->Lock(0, size, &pData, D3DLOCK_DISCARD);
    memcpy(pData, indices, size);
    m_IB->Unlock();
  }

  HRESULT D3D7Device::GetCaps(LPD3DDEVICEDESC7 a) {
    return ProxyInterface->GetCaps(a);
  }

  HRESULT D3D7Device::EnumTextureFormats(LPD3DENUMPIXELFORMATSCALLBACK a, LPVOID b) {
    return ProxyInterface->EnumTextureFormats(a, b);
  }

  HRESULT D3D7Device::BeginScene() {
	  GetD3D9()->BeginScene();
    return ProxyInterface->BeginScene();
  }

  HRESULT D3D7Device::EndScene() {
	  GetD3D9()->EndScene();
    // TODO: This should go in surface->Blit() probably
    GetD3D9()->Present(NULL, NULL, NULL, NULL);
    return ProxyInterface->EndScene();
  }

  HRESULT D3D7Device::GetDirect3D(LPDIRECT3D7 *a) {
    return ProxyInterface->GetDirect3D(a);
  }

  HRESULT D3D7Device::SetRenderTarget(LPDIRECTDRAWSURFACE7 rt, DWORD flags) {
    DD7Surface* surf = static_cast<DD7Surface*>(rt);

    // Reset to initial rendertarget.
    if (surf == m_rt) {
      GetD3D9()->SetRenderTarget(0, m_initialRT.ptr());
      GetD3D9()->SetDepthStencilSurface(m_initialDS.ptr());
      return ProxyInterface->SetRenderTarget(rt, flags);
    }

    if (FAILED(InitTexture(surf, true)))
      return D3DERR_INVALIDCALL;

    GetD3D9()->SetRenderTarget(0, surf->GetSurface());
    return ProxyInterface->SetRenderTarget(rt, flags);
  }

  HRESULT D3D7Device::GetRenderTarget(LPDIRECTDRAWSURFACE7 *rt) {
    return ProxyInterface->GetRenderTarget(rt);
  }

#pragma region Identical Shims

  HRESULT D3D7Device::Clear(DWORD a, LPD3DRECT b, DWORD c, D3DCOLOR d, D3DVALUE e, DWORD f) {
    GetD3D9()->Clear(a, b, c, d, e, f);
	  return ProxyInterface->Clear(a, b, c, d, e, f);
  }

  HRESULT D3D7Device::SetTransform(D3DTRANSFORMSTATETYPE type, LPD3DMATRIX matrix) {
    GetD3D9()->SetTransform(ConvertTransformState(type), matrix);
	  return ProxyInterface->SetTransform(type, matrix);
  }

  HRESULT D3D7Device::GetTransform(D3DTRANSFORMSTATETYPE type, LPD3DMATRIX matrix) {
	  GetD3D9()->GetTransform(ConvertTransformState(type), matrix);
    return ProxyInterface->GetTransform(type, matrix);
  }

  HRESULT D3D7Device::SetViewport(LPD3DVIEWPORT7 a) {
    GetD3D9()->SetViewport((d3d9::D3DVIEWPORT9*)a);
	  return ProxyInterface->SetViewport(a);
  }

  HRESULT D3D7Device::MultiplyTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b) {
	  GetD3D9()->MultiplyTransform((d3d9::D3DTRANSFORMSTATETYPE)a, b);
    return ProxyInterface->MultiplyTransform(a, b);
  }

  HRESULT D3D7Device::GetViewport(LPD3DVIEWPORT7 a) {
	  GetD3D9()->GetViewport((d3d9::D3DVIEWPORT9*)a);
    return ProxyInterface->GetViewport(a);
  }

  HRESULT D3D7Device::SetMaterial(LPD3DMATERIAL7 a) {
	  GetD3D9()->SetMaterial((d3d9::D3DMATERIAL9*)a);
    return ProxyInterface->SetMaterial(a);
  }

  HRESULT D3D7Device::GetMaterial(LPD3DMATERIAL7 a) {
	  GetD3D9()->GetMaterial((d3d9::D3DMATERIAL9*)a);
    return ProxyInterface->GetMaterial(a);
  }

  HRESULT D3D7Device::SetLight(DWORD a, LPD3DLIGHT7 b) {
    GetD3D9()->SetLight(a, (d3d9::D3DLIGHT9*)b);
	  return ProxyInterface->SetLight(a, b);
  }

  HRESULT D3D7Device::GetLight(DWORD a, LPD3DLIGHT7 b) {
    GetD3D9()->GetLight(a, (d3d9::D3DLIGHT9*)b);
	  return ProxyInterface->GetLight(a, b);
  }

  HRESULT D3D7Device::SetRenderState(D3DRENDERSTATETYPE a, DWORD b) {
    GetD3D9()->SetRenderState((d3d9::D3DRENDERSTATETYPE)a, b);
	  return ProxyInterface->SetRenderState(a, b);
  }

  HRESULT D3D7Device::GetRenderState(D3DRENDERSTATETYPE a, LPDWORD b) {
    GetD3D9()->GetRenderState((d3d9::D3DRENDERSTATETYPE)a, b);
	  return ProxyInterface->GetRenderState(a, b);
  }

  HRESULT D3D7Device::BeginStateBlock() {
    GetD3D9()->BeginStateBlock();
	  return ProxyInterface->BeginStateBlock();
  }

  HRESULT D3D7Device::EndStateBlock(LPDWORD a) {
    GetD3D9()->EndStateBlock((d3d9::IDirect3DStateBlock9**)a);
	  return ProxyInterface->EndStateBlock(a);
  }

  HRESULT D3D7Device::LightEnable(DWORD n, BOOL enabled) {
    GetD3D9()->LightEnable(n, enabled);
    return ProxyInterface->LightEnable(n, enabled);
  }

  HRESULT D3D7Device::GetLightEnable(DWORD n, BOOL *enabled) {
    GetD3D9()->GetLightEnable(n, enabled);
    return ProxyInterface->GetLightEnable(n, enabled);
  }

  HRESULT D3D7Device::SetClipPlane(DWORD a, D3DVALUE *b) {
    GetD3D9()->SetClipPlane(a, b);
    return ProxyInterface->SetClipPlane(a, b);
  }

  HRESULT D3D7Device::GetClipPlane(DWORD a, D3DVALUE *b) {
    GetD3D9()->GetClipPlane(a, b);
    return ProxyInterface->GetClipPlane(a, b);
  }

  HRESULT D3D7Device::GetInfo(DWORD a, LPVOID b, DWORD c) {
    return ProxyInterface->GetInfo(a, b, c);
  }

#pragma endregion

  HRESULT D3D7Device::PreLoad(LPDIRECTDRAWSURFACE7 Surface) {
    DD7Surface* surf = static_cast<DD7Surface*>(Surface);
    surf->GetSurface()->PreLoad();
    return ProxyInterface->PreLoad(Surface);
  }

  HRESULT D3D7Device::DrawPrimitive(
	    D3DPRIMITIVETYPE  dptPrimitiveType,
	    DWORD             dwVertexTypeDesc,
	    LPVOID            lpVertices,
	    DWORD             dwVertexCount,
	    DWORD             dwFlags) {
    GetD3D9()->SetFVF(dwVertexTypeDesc);
    GetD3D9()->DrawPrimitiveUP(
      d3d9::D3DPRIMITIVETYPE(dptPrimitiveType),
      dwVertexCount / GetPrimitiveSize(dptPrimitiveType),
      lpVertices,
      GetFVFSize(dwVertexTypeDesc));
	  return ProxyInterface->DrawPrimitive(dptPrimitiveType, dwVertexTypeDesc, lpVertices, dwVertexCount, dwFlags);
  }

  HRESULT D3D7Device::DrawIndexedPrimitive(
      D3DPRIMITIVETYPE  dptPrimitiveType,
      DWORD             dwVertexTypeDesc,
      LPVOID            lpvVertices,
      DWORD             dwVertexCount,
      LPWORD            lpwIndices,
      DWORD             dwIndexCount,
      DWORD             dwFlags) {
    GetD3D9()->SetFVF(dwVertexTypeDesc);
    GetD3D9()->DrawIndexedPrimitiveUP(
      d3d9::D3DPRIMITIVETYPE(dptPrimitiveType),
      0,
      dwVertexCount,
      dwIndexCount / GetPrimitiveSize(dptPrimitiveType),
      lpwIndices,
      d3d9::D3DFMT_INDEX16,
      lpvVertices,
      GetFVFSize(dwVertexTypeDesc));

    return ProxyInterface->DrawIndexedPrimitive(
      dptPrimitiveType,
      dwVertexTypeDesc,
      lpvVertices,
      dwVertexCount,
      lpwIndices,
      dwIndexCount,
      dwFlags);
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

  HRESULT D3D7Device::DrawPrimitiveVB(
      D3DPRIMITIVETYPE        PrimitiveType,
      LPDIRECT3DVERTEXBUFFER7 VB,
      DWORD                   StartVertex,
      DWORD                   NumVertices,
      DWORD                   Flags) {

    D3D7VertexBuffer* vb = static_cast<D3D7VertexBuffer*>(VB);
    GetD3D9()->SetFVF(vb->GetFVF());
    GetD3D9()->SetStreamSource(0, vb->GetD3D9(), 0, vb->GetStride());
    GetD3D9()->DrawPrimitive(
      d3d9::D3DPRIMITIVETYPE(PrimitiveType),
      StartVertex,
      NumVertices / GetPrimitiveSize(PrimitiveType));
    return ProxyInterface->DrawPrimitiveVB(PrimitiveType, VB, StartVertex, NumVertices, Flags);
  }

  HRESULT D3D7Device::DrawIndexedPrimitiveVB(
      D3DPRIMITIVETYPE        PrimitiveType,
      LPDIRECT3DVERTEXBUFFER7 VB,
      DWORD                   StartVertex,
      DWORD                   NumVertices,
      LPWORD                  Indices,
      DWORD                   IndexCount,
      DWORD                   Flags) {
    D3D7VertexBuffer* vb = static_cast<D3D7VertexBuffer*>(VB);
    UploadIndices(Indices, IndexCount);
    GetD3D9()->SetFVF(vb->GetFVF());
    GetD3D9()->SetStreamSource(0, vb->GetD3D9(), 0, vb->GetStride());
    GetD3D9()->SetIndices(m_IB.ptr());
    GetD3D9()->DrawIndexedPrimitive(
      d3d9::D3DPRIMITIVETYPE(PrimitiveType),
      0,
      0,
      NumVertices,
      StartVertex,
      IndexCount / GetPrimitiveSize(PrimitiveType));
    return ProxyInterface->DrawIndexedPrimitiveVB(PrimitiveType, VB, StartVertex, NumVertices, Indices, IndexCount, Flags);
  }

  HRESULT D3D7Device::ComputeSphereVisibility(LPD3DVECTOR a, LPD3DVALUE b, DWORD c, DWORD d, LPDWORD e) {
    return ProxyInterface->ComputeSphereVisibility(a, b, c, d, e);
  }

  HRESULT D3D7Device::GetTexture(DWORD index, LPDIRECTDRAWSURFACE7* tex) {
    return ProxyInterface->GetTexture(index, tex);
  }

  inline HRESULT D3D7Device::InitTexture(DD7Surface* surf, bool renderTarget) {

    if (!surf)
      return D3D_OK;

    // If the DDraw surface doesn't have a D3D9 texture attached, create one. 
    if (unlikely(renderTarget ? !surf->GetSurface() : !surf->GetTexture())) {
      // TODO: Perhaps do this in DD7Surface::ReleaseDC or DD7Interface::CreateSurface?
      // Some kind of IsUsing3D method...?
      DDSURFACEDESC2 desc;
      desc.dwSize = sizeof(DDSURFACEDESC2);
      surf->GetSurfaceDesc(&desc);
      DWORD mips = desc.dwMipMapCount + 1;

      bool complex = desc.ddsCaps.dwCaps & DDSCAPS_COMPLEX;

      HRESULT res;

      if (renderTarget) {
        Logger::info("Initializing RT: ");
        PrintSurfaceStructure(surf, 1);
      }

      if (desc.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP) {
        Com<d3d9::IDirect3DCubeTexture9> tex = nullptr;

        // TODO: Pool for non-RT cubemaps.
        res = GetD3D9()->CreateCubeTexture(
          desc.dwWidth, mips, renderTarget ? D3DUSAGE_RENDERTARGET : 0,
          ConvertFormat(desc.ddpfPixelFormat), d3d9::D3DPOOL_DEFAULT, &tex, nullptr);

        if (FAILED(res)) return res;

        if (!renderTarget) {
          // TODO: Upload cubemap faces.
        }
        
        // Attach face 0 to this surface.
        Com<d3d9::IDirect3DSurface9> face = nullptr;
        tex->GetCubeMapSurface((d3d9::D3DCUBEMAP_FACES)0, 0, &face);
        surf->SetSurface(std::move(face));

        // Attach sides 1-5 to each attached surface.
        surf->EnumAttachedSurfaces(tex.ptr(),
          [](IDirectDrawSurface7* subsurf, DDSURFACEDESC2* desc, void* ctx) WINAPI -> HRESULT {
            d3d9::IDirect3DCubeTexture9* cube = (d3d9::IDirect3DCubeTexture9*)ctx;

            // Skip zbuffer.
            if (desc->ddsCaps.dwCaps & DDSCAPS_ZBUFFER)
              return DDENUMRET_OK;

            Com<d3d9::IDirect3DSurface9> face = nullptr;
            cube->GetCubeMapSurface(GetCubemapFace(desc), 0, &face);

            DD7Surface* dd7surf = (DD7Surface*)subsurf;
            dd7surf->SetSurface(std::move(face));

            return DDENUMRET_OK;
          }
        );

        surf->SetTexture(std::move(tex));
        return D3D_OK;
      }

      else if (renderTarget) {
        Com<d3d9::IDirect3DSurface9> rt = nullptr;

        if (desc.ddsCaps.dwCaps & DDSCAPS_BACKBUFFER) {
          res = GetD3D9()->GetBackBuffer(0, 0, d3d9::D3DBACKBUFFER_TYPE_MONO, &rt);
          Logger::info("Created backbuffer surface.");
        }

        else if (desc.ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN) {
          res = GetD3D9()->CreateOffscreenPlainSurface(
            desc.dwWidth, desc.dwHeight, ConvertFormat(desc.ddpfPixelFormat),
            d3d9::D3DPOOL_DEFAULT, &rt, nullptr);
          Logger::info("Created offscreen plain surface.");
        }

        else if (desc.ddsCaps.dwCaps & DDSCAPS_COMPLEX) {
          
          res = GetD3D9()->CreateRenderTarget(
            desc.dwWidth, desc.dwHeight, ConvertFormat(desc.ddpfPixelFormat),
            d3d9::D3DMULTISAMPLE_NONE, 0, FALSE, &rt, nullptr);

          Logger::warn("Unknown complex surface RT. Creating generic RT?");
        }
        
        else {
          res = GetD3D9()->CreateRenderTarget(
            desc.dwWidth, desc.dwHeight, ConvertFormat(desc.ddpfPixelFormat),
            d3d9::D3DMULTISAMPLE_NONE, 0, FALSE, &rt, nullptr);
          Logger::info("Created generic RT.");
        }

        if (FAILED(res)) return res;

        surf->SetSurface(std::move(rt));
      } else {
        Com<d3d9::IDirect3DTexture9> tex = nullptr;
        res = GetD3D9()->CreateTexture(
          desc.dwWidth, desc.dwHeight, mips, 0,
          ConvertFormat(desc.ddpfPixelFormat), d3d9::D3DPOOL_MANAGED, &tex, nullptr);

        if (FAILED(res)) return res;

        // Get ddraw surface DC
        HDC dc7;
        res = surf->GetDC(&dc7);
        if (FAILED(res)) return res;

        // Blit to each mip level
        for (DWORD i = 0; i < mips; i++) {
          Com<d3d9::IDirect3DSurface9> level = nullptr;
          tex->GetSurfaceLevel(i, &level);

          HDC levelDC;
          res = level->GetDC(&levelDC);
          if (FAILED(res)) return res;

          bool b = BitBlt(levelDC, 0, 0, desc.dwWidth, desc.dwHeight, dc7, 0, 0, SRCCOPY);

          level->ReleaseDC(levelDC);
        }

        surf->ReleaseDC(dc7);
        surf->SetTexture(std::move(tex));
      }
    }
    return D3D_OK;
  }

  HRESULT D3D7Device::SetTexture(DWORD index, LPDIRECTDRAWSURFACE7 tex7) {
    DD7Surface* surf = static_cast<DD7Surface*>(tex7);
    if (FAILED(InitTexture(surf)))
      return D3DERR_INVALIDCALL;

    GetD3D9()->SetTexture(index, surf->GetTexture());
    return ProxyInterface->SetTexture(index, tex7);
  }

  HRESULT D3D7Device::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, LPDWORD pValue) {
    d3d9::D3DSAMPLERSTATETYPE stateType = ConvertSamplerStateType(Type);

    if (stateType != -1) {
      // if the type has been remapped to a sampler state type:
      return GetD3D9()->GetSamplerState(Stage, stateType, pValue);
    } else {
      return GetD3D9()->GetTextureStageState(Stage, d3d9::D3DTEXTURESTAGESTATETYPE(Type), pValue);
    }
  }

  HRESULT D3D7Device::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
    d3d9::D3DSAMPLERSTATETYPE stateType = ConvertSamplerStateType(Type);

    if (stateType != -1) {
      // if the type has been remapped to a sampler state type:
      return GetD3D9()->SetSamplerState(Stage, stateType, Value);
    } else {
      return GetD3D9()->SetTextureStageState(Stage, d3d9::D3DTEXTURESTAGESTATETYPE(Type), Value);
    }
  }

  HRESULT D3D7Device::ValidateDevice(LPDWORD a) {
    GetD3D9()->ValidateDevice(a);
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
}