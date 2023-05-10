#pragma once

#include "../ddraw/d3d_include.h"

#include "../ddraw/d3d_wrapped_object.h"

namespace dxvk {

  class D3D7Interface : public WrappedObject<d3d9::IDirect3D9, IDirect3D7>, public AddressLookupTableObject {

  private:
    std::unique_ptr<m_IDirect3DX> ProxyInterface;
    IDirect3D7 *RealInterface;
    REFIID WrapperID = IID_IDirect3D7;

  public:
    D3D7Interface(IDirect3D7 *aOriginal)
    : Base(std::move(d3d9::Direct3DCreate9(D3D_SDK_VERSION)))
    , RealInterface(aOriginal) {
      ProxyInterface = std::make_unique<m_IDirect3DX>(RealInterface, 7, this);
      ProxyAddressLookupTable.SaveAddress(this, RealInterface);
    }
    ~D3D7Interface() {
      ProxyAddressLookupTable.DeleteAddress(this);
    }

    /*** IDirect3D7 methods ***/
    STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK7, LPVOID);
    STDMETHOD(CreateDevice)(THIS_ REFCLSID, LPDIRECTDRAWSURFACE7, LPDIRECT3DDEVICE7 *);
    STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC, LPDIRECT3DVERTEXBUFFER7 *, DWORD);
    STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID, LPD3DENUMPIXELFORMATSCALLBACK, LPVOID);
    STDMETHOD(EvictManagedTextures)(THIS);
  };
}