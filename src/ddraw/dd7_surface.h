#pragma once

#include "d3d_include.h"
#include "d3d_wrapped_object.h"

namespace dxvk {

  class DD7Surface : public IDirectDrawSurface7, public AddressLookupTableObject {
  private:
    std::unique_ptr<m_IDirectDrawSurfaceX> ProxyInterface;
    IDirectDrawSurface7* RealInterface;
    REFIID WrapperID = IID_IDirectDrawSurface7;

  public:
    DD7Surface(IDirectDrawSurface7* aOriginal) : RealInterface(aOriginal) {
      ProxyInterface = std::make_unique<m_IDirectDrawSurfaceX>(RealInterface, 7, this);
      ProxyAddressLookupTable.SaveAddress(this, RealInterface);
    }
    ~DD7Surface() {
      ProxyAddressLookupTable.DeleteAddress(this);
    }

    DWORD GetDirectXVersion() {
      return 7;
    }
    IDirectDrawSurface7* GetProxyInterface() {
      return RealInterface;
    }

    d3d9::IDirect3DSurface9* GetSurface() {
      if (!this)
        return nullptr;
      if (m_surface.ptr())
        return m_surface.ptr();
      /*if (m_texture.ptr()) {
        if (d3d9::IDirect3DCubeTexture9* cube = dynamic_cast<d3d9::IDirect3DCubeTexture9*>(m_texture.ptr())) {
          if (d3d9::IDirect3DSurface9* surface; SUCCEEDED(cube->GetCubeMapSurface(d3d9::D3DCUBEMAP_FACE_POSITIVE_X, 0, &surface)))
            return surface;
        } else if (d3d9::IDirect3DTexture9* texture = dynamic_cast<d3d9::IDirect3DTexture9*>(m_texture.ptr())) {
          if (d3d9::IDirect3DSurface9* surface; SUCCEEDED(texture->GetSurfaceLevel(0, &surface)))
            return surface;
        }
      }*/
      return nullptr;
    }

    void SetSurface(Com<d3d9::IDirect3DSurface9>&& surface) {
      m_surface = surface;
    }

    d3d9::IDirect3DBaseTexture9* GetTexture() {
      if (!this)
        return nullptr;
      return m_texture.ptr();
    }

    void SetTexture(Com<d3d9::IDirect3DBaseTexture9>&& texture) {
      m_texture = texture;
    }

    void SetTexture(Com<d3d9::IDirect3DTexture9>&& texture) {
      m_texture = texture.ref();
    }

    void SetTexture(Com<d3d9::IDirect3DCubeTexture9>&& texture) {
      m_texture = texture.ref();
    }

    /*** IUnknown methods ***/
    STDMETHOD_(ULONG, AddRef)(THIS);
    STDMETHOD_(ULONG, Release)(THIS);

    /*** IDirectDrawSurface methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj);
    STDMETHOD(AddAttachedSurface)(THIS_ LPDIRECTDRAWSURFACE7);
    STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT);
    STDMETHOD(Blt)(THIS_ LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX);
    STDMETHOD(BltBatch)(THIS_ LPDDBLTBATCH, DWORD, DWORD);
    STDMETHOD(BltFast)(THIS_ DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD);
    STDMETHOD(DeleteAttachedSurface)(THIS_ DWORD, LPDIRECTDRAWSURFACE7);
    STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID, LPDDENUMSURFACESCALLBACK7);
    STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7);
    STDMETHOD(Flip)(THIS_ LPDIRECTDRAWSURFACE7, DWORD);
    STDMETHOD(GetAttachedSurface)(THIS_ LPDDSCAPS2, LPDIRECTDRAWSURFACE7 FAR*);
    STDMETHOD(GetBltStatus)(THIS_ DWORD);
    STDMETHOD(GetCaps)(THIS_ LPDDSCAPS2);
    STDMETHOD(GetClipper)(THIS_ LPDIRECTDRAWCLIPPER FAR*);
    STDMETHOD(GetColorKey)(THIS_ DWORD, LPDDCOLORKEY);
    STDMETHOD(GetDC)(THIS_ HDC FAR*);
    STDMETHOD(GetFlipStatus)(THIS_ DWORD);
    STDMETHOD(GetOverlayPosition)(THIS_ LPLONG, LPLONG);
    STDMETHOD(GetPalette)(THIS_ LPDIRECTDRAWPALETTE FAR*);
    STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT);
    STDMETHOD(GetSurfaceDesc)(THIS_ LPDDSURFACEDESC2);
    STDMETHOD(Initialize)(THIS_ LPDIRECTDRAW, LPDDSURFACEDESC2);
    STDMETHOD(IsLost)(THIS);
    STDMETHOD(Lock)(THIS_ LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE);
    STDMETHOD(ReleaseDC)(THIS_ HDC);
    STDMETHOD(Restore)(THIS);
    STDMETHOD(SetClipper)(THIS_ LPDIRECTDRAWCLIPPER);
    STDMETHOD(SetColorKey)(THIS_ DWORD, LPDDCOLORKEY);
    STDMETHOD(SetOverlayPosition)(THIS_ LONG, LONG);
    STDMETHOD(SetPalette)(THIS_ LPDIRECTDRAWPALETTE);
    STDMETHOD(Unlock)(THIS_ LPRECT);
    STDMETHOD(UpdateOverlay)(THIS_ LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX);
    STDMETHOD(UpdateOverlayDisplay)(THIS_ DWORD);
    STDMETHOD(UpdateOverlayZOrder)(THIS_ DWORD, LPDIRECTDRAWSURFACE7);
    /*** Added in the v2 interface ***/
    STDMETHOD(GetDDInterface)(THIS_ LPVOID FAR*);
    STDMETHOD(PageLock)(THIS_ DWORD);
    STDMETHOD(PageUnlock)(THIS_ DWORD);
    /*** Added in the v3 interface ***/
    STDMETHOD(SetSurfaceDesc)(THIS_ LPDDSURFACEDESC2, DWORD);
    /*** Added in the v4 interface ***/
    STDMETHOD(SetPrivateData)(THIS_ REFGUID, LPVOID, DWORD, DWORD);
    STDMETHOD(GetPrivateData)(THIS_ REFGUID, LPVOID, LPDWORD);
    STDMETHOD(FreePrivateData)(THIS_ REFGUID);
    STDMETHOD(GetUniquenessValue)(THIS_ LPDWORD);
    STDMETHOD(ChangeUniquenessValue)(THIS);
    /*** Moved Texture7 methods here ***/
    STDMETHOD(SetPriority)(THIS_ DWORD);
    STDMETHOD(GetPriority)(THIS_ LPDWORD);
    STDMETHOD(SetLOD)(THIS_ DWORD);
    STDMETHOD(GetLOD)(THIS_ LPDWORD);

  private:
    Com<d3d9::IDirect3DSurface9> m_surface;
    Com<d3d9::IDirect3DBaseTexture9> m_texture;

    bool IsPrimary() {
      DDSURFACEDESC2 desc;
      desc.dwSize = sizeof(DDSURFACEDESC2);
      GetSurfaceDesc(&desc);
      return desc.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE;
    }

  public:
    static inline IDirectDrawClipper* s_primaryClipper;
  };

}