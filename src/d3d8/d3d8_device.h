#pragma once

// Implements IDirect3DDevice8

#include "d3d8_include.h"
#include "d3d8_texture.h"
#include "d3d8_buffer.h"
#include "d3d8_swapchain.h"
#include "d3d8_state_block.h"
#include "d3d8_d3d9_util.h"
#include "d3d8_caps.h"

#include "../d3d9/d3d9_bridge.h"

#include <array>
#include <vector>
#include <type_traits>
#include <unordered_map>

namespace dxvk {

  class D3D8InterfaceEx;
  class D3D8SwapChainEx;

  struct D3D8VertexShaderInfo;

  using D3D8DeviceBase = D3D8WrappedObject<d3d9::IDirect3DDevice9, IDirect3DDevice8>;
  class D3D8DeviceEx final : public D3D8DeviceBase {

    friend class D3D8SwapChainEx;
    friend class D3D8StateBlock;
  public:

    D3D8DeviceEx(
      D3D8InterfaceEx*              pParent,
      Com<d3d9::IDirect3DDevice9>&& pDevice,
      //D3D8Adapter*                    pAdapter,
      D3DDEVTYPE                    DeviceType,
      HWND                          hFocusWindow,
      DWORD                         BehaviorFlags,
      D3DPRESENT_PARAMETERS*        pParams);

    ~D3D8DeviceEx();

    /* Direct3D 8 Exclusive Methods */

    HRESULT STDMETHODCALLTYPE CopyRects(
            IDirect3DSurface8* pSourceSurface,
            CONST RECT* pSourceRectsArray,
            UINT cRects,
            IDirect3DSurface8* pDestinationSurface,
            CONST POINT* pDestPointsArray);
  
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) {
      return GetD3D9()->GetPixelShaderConstantF(Register, (float*)pConstantData, ConstantCount);
    }

    HRESULT STDMETHODCALLTYPE GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) {
      return GetD3D9()->GetVertexShaderConstantF(Register, (float*)pConstantData, ConstantCount);
    }
    
    HRESULT STDMETHODCALLTYPE GetPixelShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData);
    HRESULT STDMETHODCALLTYPE GetVertexShaderDeclaration(DWORD Handle, void* pData, DWORD* pSizeOfData);
    HRESULT STDMETHODCALLTYPE GetVertexShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData);

    HRESULT STDMETHODCALLTYPE GetInfo(DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize);

    HRESULT STDMETHODCALLTYPE TestCooperativeLevel();

    UINT    STDMETHODCALLTYPE GetAvailableTextureMem() { return GetD3D9()->GetAvailableTextureMem(); }

    HRESULT STDMETHODCALLTYPE ResourceManagerDiscardBytes(DWORD bytes) { 
      return GetD3D9()->EvictManagedResources();
    }

    HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D8** ppD3D8);

    HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS8* pCaps) {
      d3d9::D3DCAPS9 caps9;
      HRESULT res = GetD3D9()->GetDeviceCaps(&caps9);
      dxvk::ConvertCaps8(caps9, pCaps);
      return res;
    }

    HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* pMode) {
      // swap chain 0
      return GetD3D9()->GetDisplayMode(0, (d3d9::D3DDISPLAYMODE*)pMode);
    }

    HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters) {
      return GetD3D9()->GetCreationParameters((d3d9::D3DDEVICE_CREATION_PARAMETERS*)pParameters);
    }

    HRESULT STDMETHODCALLTYPE SetCursorProperties(
      UINT               XHotSpot,
      UINT               YHotSpot,
      IDirect3DSurface8* pCursorBitmap) {

      D3D8Surface* surf = static_cast<D3D8Surface*>(pCursorBitmap);
      return GetD3D9()->SetCursorProperties(XHotSpot, YHotSpot, D3D8Surface::GetD3D9Nullable(surf));
    }

    void    STDMETHODCALLTYPE SetCursorPosition(UINT XScreenSpace, UINT YScreenSpace, DWORD Flags) {
      GetD3D9()->SetCursorPosition(XScreenSpace, YScreenSpace, Flags);
    }

    // Microsoft d3d8.h in the DirectX 9 SDK uses a different function signature...
    void    STDMETHODCALLTYPE SetCursorPosition(int X, int Y, DWORD Flags) {
      GetD3D9()->SetCursorPosition(X, Y, Flags);
    }

    BOOL    STDMETHODCALLTYPE ShowCursor(BOOL bShow) { return GetD3D9()->ShowCursor(bShow); }

    HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(
        D3DPRESENT_PARAMETERS* pPresentationParameters,
        IDirect3DSwapChain8** ppSwapChain) {
      
      Com<d3d9::IDirect3DSwapChain9> pSwapChain9;
      d3d9::D3DPRESENT_PARAMETERS params = ConvertPresentParameters9(pPresentationParameters);
      HRESULT res = GetD3D9()->CreateAdditionalSwapChain(
        &params,
        &pSwapChain9
      );
      
      *ppSwapChain = ref(new D3D8SwapChain(this, std::move(pSwapChain9)));

      return res;
    }


    HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) {
      m_presentParams = *pPresentationParameters;
      ResetState();

      d3d9::D3DPRESENT_PARAMETERS params = ConvertPresentParameters9(pPresentationParameters);
      return GetD3D9()->Reset(&params);
    }

    HRESULT STDMETHODCALLTYPE Present(
      const RECT* pSourceRect,
      const RECT* pDestRect,
            HWND hDestWindowOverride,
      const RGNDATA* pDirtyRegion) {
      return GetD3D9()->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }

    HRESULT STDMETHODCALLTYPE GetBackBuffer(
            UINT iBackBuffer,
            D3DBACKBUFFER_TYPE Type,
            IDirect3DSurface8** ppBackBuffer) {
      InitReturnPtr(ppBackBuffer);
      
      if (iBackBuffer >= m_backBuffers.size() || m_backBuffers[iBackBuffer] == nullptr) {
        Com<d3d9::IDirect3DSurface9> pSurface9;
        HRESULT res = GetD3D9()->GetBackBuffer(0, iBackBuffer, (d3d9::D3DBACKBUFFER_TYPE)Type, &pSurface9);

        if (FAILED(res)) return res;
        
        m_backBuffers[iBackBuffer] = new D3D8Surface(this, std::move(pSurface9));
        *ppBackBuffer = m_backBuffers[iBackBuffer].ref();

        return res;
      }

      *ppBackBuffer = m_backBuffers[iBackBuffer].ref();
      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS* pRasterStatus) {
      return GetD3D9()->GetRasterStatus(0, (d3d9::D3DRASTER_STATUS*)pRasterStatus);
    }

    void STDMETHODCALLTYPE SetGammaRamp(DWORD Flags, const D3DGAMMARAMP* pRamp) {
      // For swap chain 0
      GetD3D9()->SetGammaRamp(0, Flags, reinterpret_cast<const d3d9::D3DGAMMARAMP*>(pRamp));
    }

    void STDMETHODCALLTYPE GetGammaRamp(D3DGAMMARAMP* pRamp) {
      // For swap chain 0
      GetD3D9()->GetGammaRamp(0, reinterpret_cast<d3d9::D3DGAMMARAMP*>(pRamp));
    }

    HRESULT STDMETHODCALLTYPE CreateTexture(
            UINT                Width,
            UINT                Height,
            UINT                Levels,
            DWORD               Usage,
            D3DFORMAT           Format,
            D3DPOOL             Pool,
            IDirect3DTexture8** ppTexture) {
      InitReturnPtr(ppTexture);

      Com<d3d9::IDirect3DTexture9> pTex9 = nullptr;
      HRESULT res = GetD3D9()->CreateTexture(
        Width,
        Height,
        Levels,
        Usage,
        d3d9::D3DFORMAT(Format),
        d3d9::D3DPOOL(Pool),
        &pTex9,
        NULL);

      if (FAILED(res))
        return res;

      *ppTexture = ref(new D3D8Texture2D(this, std::move(pTex9)));

      return res;
    }

    HRESULT STDMETHODCALLTYPE CreateVolumeTexture(
            UINT                      Width,
            UINT                      Height,
            UINT                      Depth,
            UINT                      Levels,
            DWORD                     Usage,
            D3DFORMAT                 Format,
            D3DPOOL                   Pool,
            IDirect3DVolumeTexture8** ppVolumeTexture) {
      Com<d3d9::IDirect3DVolumeTexture9> pVolume9 = nullptr;
      HRESULT res = GetD3D9()->CreateVolumeTexture(
        Width, Height, Depth, Levels,
        Usage,
        d3d9::D3DFORMAT(Format),
        d3d9::D3DPOOL(Pool),
        &pVolume9,
        NULL);

      *ppVolumeTexture = ref(new D3D8Texture3D(this, std::move(pVolume9)));

      return res;

    }

    HRESULT STDMETHODCALLTYPE CreateCubeTexture(
          UINT                      EdgeLength,
            UINT                    Levels,
            DWORD                   Usage,
            D3DFORMAT               Format,
            D3DPOOL                 Pool,
            IDirect3DCubeTexture8** ppCubeTexture) {
      Com<d3d9::IDirect3DCubeTexture9> pCube9 = nullptr;
      HRESULT res = GetD3D9()->CreateCubeTexture(
        EdgeLength,
        Levels,
        Usage,
        d3d9::D3DFORMAT(Format),
        d3d9::D3DPOOL(Pool),
        &pCube9,
        NULL);

      *ppCubeTexture = ref(new D3D8TextureCube(this, std::move(pCube9)));

      return res;
    }

    HRESULT STDMETHODCALLTYPE CreateVertexBuffer(
            UINT                     Length,
            DWORD                    Usage,
            DWORD                    FVF,
            D3DPOOL                  Pool,
            IDirect3DVertexBuffer8** ppVertexBuffer) {
      
      Com<d3d9::IDirect3DVertexBuffer9> pVertexBuffer9 = nullptr;
      HRESULT res = GetD3D9()->CreateVertexBuffer(Length, Usage, FVF, d3d9::D3DPOOL(Pool), &pVertexBuffer9, NULL);
      *ppVertexBuffer = ref(new D3D8VertexBuffer(this, std::move(pVertexBuffer9)));
      return res;
    }

    HRESULT STDMETHODCALLTYPE CreateIndexBuffer(
            UINT                    Length,
            DWORD                   Usage,
            D3DFORMAT               Format,
            D3DPOOL                 Pool,
            IDirect3DIndexBuffer8** ppIndexBuffer) {
      Com<d3d9::IDirect3DIndexBuffer9> pIndexBuffer9 = nullptr;
      HRESULT res = GetD3D9()->CreateIndexBuffer(Length, Usage, d3d9::D3DFORMAT(Format), d3d9::D3DPOOL(Pool), &pIndexBuffer9, NULL);
      *ppIndexBuffer = ref(new D3D8IndexBuffer(this, std::move(pIndexBuffer9)));
      return res;

    }

    HRESULT STDMETHODCALLTYPE CreateRenderTarget(
            UINT                Width,
            UINT                Height,
            D3DFORMAT           Format,
            D3DMULTISAMPLE_TYPE MultiSample,
            BOOL                Lockable,
            IDirect3DSurface8** ppSurface) {
      Com<d3d9::IDirect3DSurface9> pSurf9 = nullptr;
      HRESULT res = GetD3D9()->CreateRenderTarget(
        Width,
        Height,
        d3d9::D3DFORMAT(Format),
        d3d9::D3DMULTISAMPLE_TYPE(MultiSample),
        0,    // TODO: CreateRenderTarget MultisampleQuality
        Lockable,
        &pSurf9,
        NULL);

      *ppSurface = ref(new D3D8Surface(this, std::move(pSurf9)));

      return res;
    }

    HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(
            UINT                Width,
            UINT                Height,
            D3DFORMAT           Format,
            D3DMULTISAMPLE_TYPE MultiSample,
            IDirect3DSurface8** ppSurface) {
      Com<d3d9::IDirect3DSurface9> pSurf9 = nullptr;
      HRESULT res = GetD3D9()->CreateDepthStencilSurface(
        Width,
        Height,
        d3d9::D3DFORMAT(Format),
        d3d9::D3DMULTISAMPLE_TYPE(MultiSample),
        0,    // TODO: CreateDepthStencilSurface MultisampleQuality
        true, // TODO: CreateDepthStencilSurface Discard
        &pSurf9,
        NULL);

      *ppSurface = ref(new D3D8Surface(this, std::move(pSurf9)));

      return res;
    }

    HRESULT STDMETHODCALLTYPE UpdateTexture(
            IDirect3DBaseTexture8* pSourceTexture,
            IDirect3DBaseTexture8* pDestinationTexture) {
      D3D8Texture2D* src = static_cast<D3D8Texture2D*>(pSourceTexture);
      D3D8Texture2D* dst = static_cast<D3D8Texture2D*>(pDestinationTexture);

      return GetD3D9()->UpdateTexture(D3D8Texture2D::GetD3D9Nullable(src), D3D8Texture2D::GetD3D9Nullable(dst));
    }

    HRESULT STDMETHODCALLTYPE GetFrontBuffer(IDirect3DSurface8* pDestSurface) {
      if (unlikely(pDestSurface == nullptr))
        return D3DERR_INVALIDCALL;
      Com<D3D8Surface> surf = static_cast<D3D8Surface*>(pDestSurface);
      // This actually gets a copy of the front buffer and writes it to pDestSurface
      return GetD3D9()->GetFrontBufferData(0, D3D8Surface::GetD3D9Nullable(surf));
    }

    // CreateImageSurface -> CreateOffscreenPlainSurface
    HRESULT STDMETHODCALLTYPE CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) {

      Com<d3d9::IDirect3DSurface9> pSurf = nullptr;
      HRESULT res = GetD3D9()->CreateOffscreenPlainSurface(
        Width,
        Height,
        d3d9::D3DFORMAT(Format),
        // FIXME: D3DPOOL_SCRATCH is said to be dx8 compatible, but currently won't work with CopyRects
        d3d9::D3DPOOL_SYSTEMMEM,
        &pSurf,
        NULL);

      *ppSurface = ref(new D3D8Surface(this, std::move(pSurf)));

      return res;
    }

    HRESULT STDMETHODCALLTYPE SetRenderTarget(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) {
      HRESULT res;

      if (pRenderTarget != NULL) {
        D3D8Surface* surf = static_cast<D3D8Surface*>(pRenderTarget);

        D3DSURFACE_DESC rtDesc;
        surf->GetDesc(&rtDesc);

        if (unlikely(!(rtDesc.Usage & D3DUSAGE_RENDERTARGET)))
          return D3DERR_INVALIDCALL;

        if(likely(m_renderTarget.ptr() != surf)) {
          res = GetD3D9()->SetRenderTarget(0, surf->GetD3D9());

          if (FAILED(res)) return res;

          D3D8Surface* pRenderTargetSwap = nullptr;
          bool isRTSwap = m_renderTargetPrev.ptr() == surf;

          if(unlikely(isRTSwap))
            // keep a temporary ref on the prev RT to prevent its release
            pRenderTargetSwap = m_renderTargetPrev.ref();

          m_renderTargetPrev = m_renderTarget;
          m_renderTarget = surf;

          if(unlikely(isRTSwap && pRenderTargetSwap))
            pRenderTargetSwap->Release();
        }
      }

      // SetDepthStencilSurface is a separate call
      D3D8Surface* zStencil = static_cast<D3D8Surface*>(pNewZStencil);

      if(pNewZStencil != NULL) {
        D3DSURFACE_DESC zsDesc;
        zStencil->GetDesc(&zsDesc);

        if (unlikely(!(zsDesc.Usage & D3DUSAGE_DEPTHSTENCIL)))
          return D3DERR_INVALIDCALL;
      }

      if(likely(m_depthStencil.ptr() != zStencil)) {
        res = GetD3D9()->SetDepthStencilSurface(D3D8Surface::GetD3D9Nullable(zStencil));

        if (FAILED(res)) return res;

        D3D8Surface* pDepthStencilSwap = nullptr;
        bool isDSSwap = m_depthStencilPrev.ptr() == zStencil;

        if(unlikely(isDSSwap))
          // keep a temporary ref on the prev DS to prevent its release
          pDepthStencilSwap = m_depthStencilPrev.ref();

        m_depthStencilPrev = m_depthStencil;
        m_depthStencil = zStencil;

        if(unlikely(isDSSwap && pDepthStencilSwap))
          pDepthStencilSwap->Release();
      }

      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE GetRenderTarget(IDirect3DSurface8** ppRenderTarget) {
      InitReturnPtr(ppRenderTarget);

      if (unlikely(m_renderTarget == nullptr)) {
        Com<d3d9::IDirect3DSurface9> pRT9 = nullptr;
        HRESULT res = GetD3D9()->GetRenderTarget(0, &pRT9); // use RT index 0

        if(FAILED(res)) return res;

        m_renderTarget = new D3D8Surface(this, std::move(pRT9));

        *ppRenderTarget = m_renderTarget.ref();
        return res;
      }

      *ppRenderTarget = m_renderTarget.ref();
      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface) {
      InitReturnPtr(ppZStencilSurface);
      
      if (unlikely(m_depthStencil == nullptr)) {
        Com<d3d9::IDirect3DSurface9> pStencil9 = nullptr;
        HRESULT res = GetD3D9()->GetDepthStencilSurface(&pStencil9);

        if(FAILED(res)) return res;

        m_depthStencil = new D3D8Surface(this, std::move(pStencil9));

        *ppZStencilSurface = m_depthStencil.ref();
        return res;
      }

      *ppZStencilSurface = m_depthStencil.ref();
      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE BeginScene() { return GetD3D9()->BeginScene(); }

    HRESULT STDMETHODCALLTYPE EndScene() { return GetD3D9()->EndScene(); }

    HRESULT STDMETHODCALLTYPE Clear(
            DWORD    Count,
      const D3DRECT* pRects,
            DWORD    Flags,
            D3DCOLOR Color,
            float    Z,
            DWORD    Stencil) {
      return GetD3D9()->Clear(Count, pRects, Flags, Color, Z, Stencil);
    }

    HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) {
      return GetD3D9()->SetTransform(d3d9::D3DTRANSFORMSTATETYPE(State), pMatrix);
    }

    HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) {
      return GetD3D9()->GetTransform(d3d9::D3DTRANSFORMSTATETYPE(State), pMatrix);
    }

    HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE TransformState, const D3DMATRIX* pMatrix) {
      return GetD3D9()->MultiplyTransform(d3d9::D3DTRANSFORMSTATETYPE(TransformState), pMatrix);
    }

    HRESULT STDMETHODCALLTYPE SetViewport(const D3DVIEWPORT8* pViewport) {
      return GetD3D9()->SetViewport(reinterpret_cast<const d3d9::D3DVIEWPORT9*>(pViewport));
    }

    HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT8* pViewport) {
      return GetD3D9()->GetViewport(reinterpret_cast<d3d9::D3DVIEWPORT9*>(pViewport));
    }

    HRESULT STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL8* pMaterial) {
      return GetD3D9()->SetMaterial((const d3d9::D3DMATERIAL9*)pMaterial);
    }

    HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL8* pMaterial) {
      return GetD3D9()->GetMaterial((d3d9::D3DMATERIAL9*)pMaterial);
    }

    HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, const D3DLIGHT8* pLight) {
      return GetD3D9()->SetLight(Index, (const d3d9::D3DLIGHT9*)pLight);
    }

    HRESULT STDMETHODCALLTYPE GetLight(DWORD Index, D3DLIGHT8* pLight) {
      return GetD3D9()->GetLight(Index, (d3d9::D3DLIGHT9*)pLight);
    }

    HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable) {
      return GetD3D9()->LightEnable(Index, Enable);
    }

    HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL* pEnable) {
      return GetD3D9()->GetLightEnable(Index, pEnable);
    }

    HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, const float* pPlane) {
      return GetD3D9()->SetClipPlane(Index, pPlane);
    }

    HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD Index, float* pPlane) {
      return GetD3D9()->GetClipPlane(Index, pPlane);
    }

    HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

    HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
    
    HRESULT STDMETHODCALLTYPE CreateStateBlock(
            D3DSTATEBLOCKTYPE     Type,
            DWORD*                pToken) {

      Com<d3d9::IDirect3DStateBlock9> pStateBlock9;
      HRESULT res = GetD3D9()->CreateStateBlock(d3d9::D3DSTATEBLOCKTYPE(Type), &pStateBlock9);

      D3D8StateBlock* pStateBlock = new D3D8StateBlock(this, Type, pStateBlock9.ref());

      *pToken = DWORD(reinterpret_cast<uintptr_t>(pStateBlock));

      return res;
    }

    HRESULT STDMETHODCALLTYPE CaptureStateBlock(DWORD Token) {
      return reinterpret_cast<D3D8StateBlock*>(Token)->Capture();
    }

    HRESULT STDMETHODCALLTYPE ApplyStateBlock(DWORD Token) {
      return reinterpret_cast<D3D8StateBlock*>(Token)->Apply();
    }

    HRESULT STDMETHODCALLTYPE DeleteStateBlock(DWORD Token) {
      delete reinterpret_cast<D3D8StateBlock*>(Token);
      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE BeginStateBlock() {

      if (unlikely(m_recorder != nullptr))
        return D3DERR_INVALIDCALL;

      m_recorder = new D3D8StateBlock(this);

      return GetD3D9()->BeginStateBlock();
    }

    HRESULT STDMETHODCALLTYPE EndStateBlock(DWORD* pToken) {

      if (unlikely(pToken == nullptr || m_recorder == nullptr))
        return D3DERR_INVALIDCALL;

      Com<d3d9::IDirect3DStateBlock9> pStateBlock;
      HRESULT res = GetD3D9()->EndStateBlock(&pStateBlock);

      m_recorder->SetD3D9(std::move(pStateBlock));

      *pToken = DWORD(reinterpret_cast<uintptr_t>(m_recorder));

      m_recorder = nullptr;

      return res;
    }

    HRESULT STDMETHODCALLTYPE SetClipStatus(const D3DCLIPSTATUS8* pClipStatus) {
      return GetD3D9()->SetClipStatus(reinterpret_cast<const d3d9::D3DCLIPSTATUS9*>(pClipStatus));
    }

    HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS8* pClipStatus) {
      return GetD3D9()->GetClipStatus(reinterpret_cast<d3d9::D3DCLIPSTATUS9*>(pClipStatus));
    }

    HRESULT STDMETHODCALLTYPE GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture) {
      InitReturnPtr(ppTexture);

      *ppTexture = m_textures[Stage].ref();

      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture) {

      if (unlikely(Stage >= d8caps::MAX_TEXTURE_STAGES))
        return D3DERR_INVALIDCALL;

      if (unlikely(ShouldRecord()))
        return m_recorder->SetTexture(Stage, pTexture);

      D3D8Texture2D* tex = static_cast<D3D8Texture2D*>(pTexture);

      if(unlikely(m_textures[Stage].ptr() == tex))
        return D3D_OK;

      m_textures[Stage] = tex;

      return GetD3D9()->SetTexture(Stage, D3D8Texture2D::GetD3D9Nullable(tex));
    }

    HRESULT STDMETHODCALLTYPE GetTextureStageState(
            DWORD                    Stage,
            D3DTEXTURESTAGESTATETYPE Type,
            DWORD*                   pValue) {
      d3d9::D3DSAMPLERSTATETYPE stateType = GetSamplerStateType9(Type);

      if (stateType != -1) {
        // if the type has been remapped to a sampler state type:
        return GetD3D9()->GetSamplerState(Stage, stateType, pValue);
      }
      else {
        return GetD3D9()->GetTextureStageState(Stage, d3d9::D3DTEXTURESTAGESTATETYPE(Type), pValue);
      }
    }

    HRESULT STDMETHODCALLTYPE SetTextureStageState(
            DWORD                    Stage,
            D3DTEXTURESTAGESTATETYPE Type,
            DWORD                    Value) {

      d3d9::D3DSAMPLERSTATETYPE stateType = GetSamplerStateType9(Type);

      if (stateType != -1) {
        // if the type has been remapped to a sampler state type:
        return GetD3D9()->SetSamplerState(Stage, stateType, Value);
      } else {
        return GetD3D9()->SetTextureStageState(Stage, d3d9::D3DTEXTURESTAGESTATETYPE(Type), Value);
      }
    }

    HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* pNumPasses) {
      return GetD3D9()->ValidateDevice(pNumPasses);
    }

    // Palettes not supported by d9vk, but we pass the values through anyway.

    HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY* pEntries) {
      return GetD3D9()->SetPaletteEntries(PaletteNumber, pEntries);
    }

    HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) {
      return GetD3D9()->GetPaletteEntries(PaletteNumber, pEntries);
    }

    HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber) {
      return GetD3D9()->SetCurrentTexturePalette(PaletteNumber);
    }

    HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT* PaletteNumber) {
      return GetD3D9()->GetCurrentTexturePalette(PaletteNumber);
    }

    HRESULT STDMETHODCALLTYPE DrawPrimitive(
            D3DPRIMITIVETYPE PrimitiveType,
            UINT             StartVertex,
            UINT             PrimitiveCount) {
      return GetD3D9()->DrawPrimitive(d3d9::D3DPRIMITIVETYPE(PrimitiveType), StartVertex, PrimitiveCount);
    }

    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(
            D3DPRIMITIVETYPE PrimitiveType,
            UINT             MinVertexIndex,
            UINT             NumVertices,
            UINT             StartIndex,
            UINT             PrimitiveCount) {
      return GetD3D9()->DrawIndexedPrimitive(
        d3d9::D3DPRIMITIVETYPE(PrimitiveType),
        m_baseVertexIndex, // set by SetIndices
        MinVertexIndex,
        NumVertices,
        StartIndex,
        PrimitiveCount);
    }

    HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(
            D3DPRIMITIVETYPE PrimitiveType,
            UINT             PrimitiveCount,
      const void*            pVertexStreamZeroData,
            UINT             VertexStreamZeroStride) {
      return GetD3D9()->DrawPrimitiveUP(d3d9::D3DPRIMITIVETYPE(PrimitiveType), PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    }

    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(
            D3DPRIMITIVETYPE PrimitiveType,
            UINT             MinVertexIndex,
            UINT             NumVertices,
            UINT             PrimitiveCount,
      const void*            pIndexData,
            D3DFORMAT        IndexDataFormat,
      const void*            pVertexStreamZeroData,
            UINT             VertexStreamZeroStride) {
      return GetD3D9()->DrawIndexedPrimitiveUP(
        d3d9::D3DPRIMITIVETYPE(PrimitiveType),
        MinVertexIndex,
        NumVertices,
        PrimitiveCount,
        pIndexData,
        d3d9::D3DFORMAT(IndexDataFormat),
        pVertexStreamZeroData,
        VertexStreamZeroStride);
    }

    HRESULT STDMETHODCALLTYPE ProcessVertices(
        UINT                         SrcStartIndex,
        UINT                         DestIndex,
        UINT                         VertexCount,
        IDirect3DVertexBuffer8*      pDestBuffer,
        DWORD                        Flags) {
      if (unlikely(!pDestBuffer))
        return D3DERR_INVALIDCALL;
      D3D8VertexBuffer* buffer = static_cast<D3D8VertexBuffer*>(pDestBuffer);
      return GetD3D9()->ProcessVertices(
        SrcStartIndex,
        DestIndex,
        VertexCount,
        buffer->GetD3D9(),
        nullptr,
        Flags
      );
    }


    HRESULT STDMETHODCALLTYPE CreateVertexShader(
      const DWORD*  pDeclaration,
      const DWORD*  pFunction,
            DWORD*  pHandle,
            DWORD   Usage);

    HRESULT STDMETHODCALLTYPE SetVertexShader(DWORD Handle);

    HRESULT STDMETHODCALLTYPE GetVertexShader(DWORD* pHandle);

    HRESULT STDMETHODCALLTYPE DeleteVertexShader(DWORD Handle);

    HRESULT STDMETHODCALLTYPE SetVertexShaderConstant(
            DWORD StartRegister,
      const void* pConstantData,
            DWORD ConstantCount) {
      // ConstantCount is actually the same as Vector4fCount
      return GetD3D9()->SetVertexShaderConstantF(StartRegister, reinterpret_cast<const float*>(pConstantData), ConstantCount);
    }

    HRESULT STDMETHODCALLTYPE SetStreamSource(
            UINT                    StreamNumber,
            IDirect3DVertexBuffer8* pStreamData,
            UINT                    Stride) {
      if (unlikely(StreamNumber >= d8caps::MAX_STREAMS))
        return D3DERR_INVALIDCALL;

      D3D8VertexBuffer* buffer = static_cast<D3D8VertexBuffer*>(pStreamData);

      m_streams[StreamNumber] = D3D8VBO {buffer, Stride};

      return GetD3D9()->SetStreamSource(StreamNumber, D3D8VertexBuffer::GetD3D9Nullable(buffer), 0, Stride);
    }

    HRESULT STDMETHODCALLTYPE GetStreamSource(
            UINT                     StreamNumber,
            IDirect3DVertexBuffer8** ppStreamData,
            UINT*                    pStride) {
      InitReturnPtr(ppStreamData);

      if (likely(pStride != nullptr))
        *pStride = 0;

      if (unlikely(ppStreamData == nullptr || pStride == nullptr))
        return D3DERR_INVALIDCALL;

      if (unlikely(StreamNumber >= d8caps::MAX_STREAMS))
        return D3DERR_INVALIDCALL;
      
      const D3D8VBO& vbo = m_streams[StreamNumber];

      *ppStreamData = vbo.buffer.ref();
      *pStride      = vbo.stride;
      
      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex) {

      if (unlikely(ShouldRecord()))
        return m_recorder->SetIndices(pIndexData, BaseVertexIndex);

      // used by DrawIndexedPrimitive
      m_baseVertexIndex = static_cast<INT>(BaseVertexIndex);

      D3D8IndexBuffer* buffer = static_cast<D3D8IndexBuffer*>(pIndexData);

      m_indices = buffer;

      return GetD3D9()->SetIndices(D3D8IndexBuffer::GetD3D9Nullable(buffer));
    }

    HRESULT STDMETHODCALLTYPE GetIndices(
            IDirect3DIndexBuffer8** ppIndexData,
            UINT* pBaseVertexIndex) {
      InitReturnPtr(ppIndexData);

      *ppIndexData      = m_indices.ptr();
      *pBaseVertexIndex = m_baseVertexIndex;

      return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE CreatePixelShader(
      const DWORD* pFunction, 
            DWORD* pHandle);

    HRESULT STDMETHODCALLTYPE SetPixelShader(DWORD Handle);

    HRESULT STDMETHODCALLTYPE GetPixelShader(DWORD* pHandle);

    HRESULT STDMETHODCALLTYPE DeletePixelShader(THIS_ DWORD Handle);

    HRESULT STDMETHODCALLTYPE SetPixelShaderConstant(
            DWORD StartRegister,
      const void* pConstantData,
            DWORD ConstantCount) {
      // ConstantCount is actually the same as Vector4fCount
      return GetD3D9()->SetPixelShaderConstantF(StartRegister, reinterpret_cast<const float*>(pConstantData), ConstantCount);
    }

    // Patches not supported by d9vk but pass the values through anyway.

    HRESULT STDMETHODCALLTYPE DrawRectPatch(
            UINT               Handle,
      const float*             pNumSegs,
      const D3DRECTPATCH_INFO* pRectPatchInfo) {
      return GetD3D9()->DrawRectPatch(Handle, pNumSegs, reinterpret_cast<const d3d9::D3DRECTPATCH_INFO*>(pRectPatchInfo));
    }

    HRESULT STDMETHODCALLTYPE DrawTriPatch(
            UINT              Handle,
      const float*            pNumSegs,
      const D3DTRIPATCH_INFO* pTriPatchInfo) {
      return GetD3D9()->DrawTriPatch(Handle, pNumSegs, reinterpret_cast<const d3d9::D3DTRIPATCH_INFO*>(pTriPatchInfo));
    }

    HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle) {
      return GetD3D9()->DeletePatch(Handle);
    }

  public: // Internal Methods //

    inline bool ShouldRecord() { return m_recorder != nullptr; }

    inline void ResetState() {
      // Resetting implicitly ends scenes started by BeginScene
      m_bridge->EndScene();
      // Purge cached objects
      // TODO: Some functions may need to be called here (e.g. SetTexture, etc.)
      // in case Reset can be recorded by state blocks and other things.
      m_backBuffers.clear();
      m_textures.fill(nullptr);
      m_streams.fill(D3D8VBO());
      m_indices = nullptr;
      for (UINT i = 0; i < m_presentParams.BackBufferCount; i++) {
        m_backBuffers.push_back(nullptr);
      }
      m_renderTarget = nullptr;
      m_renderTargetPrev = nullptr;
      m_depthStencil = nullptr;
      m_depthStencilPrev = nullptr;
    }

    friend d3d9::IDirect3DPixelShader9* getPixelShaderPtr(D3D8DeviceEx* device, DWORD Handle);
    friend D3D8VertexShaderInfo*        getVertexShaderInfo(D3D8DeviceEx* device, DWORD Handle);

  private:

    IDxvkD3D8Bridge*      m_bridge;

    Com<D3D8InterfaceEx>  m_parent;

    D3DPRESENT_PARAMETERS m_presentParams;

    D3D8StateBlock* m_recorder = nullptr;

    struct D3D8VBO {
      Com<D3D8VertexBuffer, false>   buffer = nullptr;
      UINT                           stride = 0;
    };
    
    // Remember to fill() these in the constructor!
    std::array<Com<D3D8Texture2D, false>, d8caps::MAX_TEXTURE_STAGES>  m_textures;
    std::array<D3D8VBO, d8caps::MAX_STREAMS>                           m_streams;

    Com<D3D8IndexBuffer, false>        m_indices;
    INT                                m_baseVertexIndex = 0;

    // TODO: Which of these should be a private ref
    std::vector<Com<D3D8Surface, false>> m_backBuffers;

    Com<D3D8Surface, false>     m_renderTarget;
    Com<D3D8Surface, false>     m_renderTargetPrev;
    Com<D3D8Surface, false>     m_depthStencil;
    Com<D3D8Surface, false>     m_depthStencilPrev;

    std::vector<D3D8VertexShaderInfo>           m_vertexShaders;
    std::vector<d3d9::IDirect3DPixelShader9*>   m_pixelShaders;
    DWORD                       m_currentVertexShader  = 0;  // can be FVF or vs index, can have DXVK_D3D8_SHADER_BIT
    DWORD                       m_currentPixelShader   = 0;  // will have DXVK_D3D8_SHADER_BIT

    D3DDEVTYPE            m_deviceType;
    HWND                  m_window;

    DWORD                 m_behaviorFlags;

  };

}
