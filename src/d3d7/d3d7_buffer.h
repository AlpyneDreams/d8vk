#pragma once

#include "../ddraw/d3d_wrapped_object.h"

namespace dxvk {

  class D3D7VertexBuffer : public D3DObject<IDirect3DVertexBuffer7>, public AddressLookupTableObject {

  private:
    std::unique_ptr<m_IDirect3DVertexBufferX> ProxyInterface;
    IDirect3DVertexBuffer7 *RealInterface;
    REFIID WrapperID = IID_IDirect3DVertexBuffer7;

  public:
    D3D7VertexBuffer(IDirect3DVertexBuffer7 *aOriginal) : RealInterface(aOriginal) {
      ProxyInterface = std::make_unique<m_IDirect3DVertexBufferX>(RealInterface, 7, this);
      ProxyAddressLookupTable.SaveAddress(this, RealInterface);
    }
    ~D3D7VertexBuffer() {
      ProxyAddressLookupTable.DeleteAddress(this);
    }

	// FIXME: Remove this.
    IDirect3DVertexBuffer7 *GetProxyInterface() {
      return RealInterface;
    }

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj);

    /*** IDirect3DVertexBuffer methods ***/
    STDMETHOD(Lock)(THIS_ DWORD, LPVOID *, LPDWORD);
    STDMETHOD(Unlock)(THIS);
    STDMETHOD(ProcessVertices)(THIS_ DWORD, DWORD, DWORD, LPDIRECT3DVERTEXBUFFER7, DWORD, LPDIRECT3DDEVICE7, DWORD);
    STDMETHOD(GetVertexBufferDesc)(THIS_ LPD3DVERTEXBUFFERDESC);
    STDMETHOD(Optimize)(THIS_ LPDIRECT3DDEVICE7, DWORD);
    /*** IDirect3DVertexBuffer7 methods ***/
    STDMETHOD(ProcessVerticesStrided)(THIS_ DWORD, DWORD, DWORD, LPD3DDRAWPRIMITIVESTRIDEDDATA, DWORD, LPDIRECT3DDEVICE7, DWORD);
  };

}