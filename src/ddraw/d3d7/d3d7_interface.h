#pragma once

namespace dxvk {

  class D3D7Interface : public IDirect3D7, public AddressLookupTableObject {
    
  private:
    std::unique_ptr<m_IDirect3DX> ProxyInterface;
    IDirect3D7 *RealInterface;
    REFIID WrapperID = IID_IDirect3D7;

  public:
    D3D7Interface(IDirect3D7 *aOriginal) : RealInterface(aOriginal) {
      ProxyInterface = std::make_unique<m_IDirect3DX>(RealInterface, 7, this);
      ProxyAddressLookupTable.SaveAddress(this, RealInterface);
    }
    ~D3D7Interface() {
      ProxyAddressLookupTable.DeleteAddress(this);
    }

    DWORD GetDirectXVersion() {
      return 7;
    }
    REFIID GetWrapperType() {
      return WrapperID;
    }
    IDirect3D7 *GetProxyInterface() {
      return RealInterface;
    }

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj);
    STDMETHOD_(ULONG, AddRef)(THIS);
    STDMETHOD_(ULONG, Release)(THIS);

    /*** IDirect3D7 methods ***/
    STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK7, LPVOID);
    STDMETHOD(CreateDevice)(THIS_ REFCLSID, LPDIRECTDRAWSURFACE7, LPDIRECT3DDEVICE7 *);
    STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC, LPDIRECT3DVERTEXBUFFER7 *, DWORD);
    STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID, LPD3DENUMPIXELFORMATSCALLBACK, LPVOID);
    STDMETHOD(EvictManagedTextures)(THIS);
  };
}