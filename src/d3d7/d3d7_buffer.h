#pragma once

#include "../ddraw/d3d_wrapped_object.h"
#include "../ddraw/d3d_util.h"

namespace dxvk {

  class D3D7VertexBuffer : public WrappedObject<d3d9::IDirect3DVertexBuffer9, IDirect3DVertexBuffer7>, public AddressLookupTableObject {

  private:
    std::unique_ptr<m_IDirect3DVertexBufferX> ProxyInterface;
    IDirect3DVertexBuffer7 *RealInterface;
    REFIID WrapperID = IID_IDirect3DVertexBuffer7;

  public:
    D3D7VertexBuffer(
        IDirect3DVertexBuffer7 *aOriginal,
        d3d9::IDirect3DVertexBuffer9* aVB9 = nullptr,
        DWORD Size = 0,
        DWORD aFVF = 0)
    : Base(aVB9)
    , RealInterface(aOriginal)
    , m_size(Size)
    , m_fvf(aFVF)
    , m_stride(GetFVFSize(aFVF)) {
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

    DWORD GetFVF() const { return m_fvf; }
    DWORD GetStride() const { return m_stride; }

    /*** IDirect3DVertexBuffer methods ***/
    STDMETHOD(Lock)(THIS_ DWORD, LPVOID *, LPDWORD);
    STDMETHOD(Unlock)(THIS);
    STDMETHOD(ProcessVertices)(THIS_ DWORD, DWORD, DWORD, LPDIRECT3DVERTEXBUFFER7, DWORD, LPDIRECT3DDEVICE7, DWORD);
    STDMETHOD(GetVertexBufferDesc)(THIS_ LPD3DVERTEXBUFFERDESC);
    STDMETHOD(Optimize)(THIS_ LPDIRECT3DDEVICE7, DWORD);
    /*** IDirect3DVertexBuffer7 methods ***/
    STDMETHOD(ProcessVerticesStrided)(THIS_ DWORD, DWORD, DWORD, LPD3DDRAWPRIMITIVESTRIDEDDATA, DWORD, LPDIRECT3DDEVICE7, DWORD);
  private:
    DWORD m_size;
    DWORD m_fvf;
    DWORD m_stride;
  };

}