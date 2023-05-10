#pragma once

#include "d3d_include.h"
#include "../util/com/com_object.h"

namespace dxvk {

  // TODO: Do D3D7 and down clamp their refcounts?
  template <typename T>
  using D3DObject = ComObjectClamp<T>;

  template <typename D3D9Type, typename D3DType>
  class WrappedObject : public D3DObject<D3DType> {

  public:
    using Base = WrappedObject<D3D9Type, D3DType>;
    using D3D9 = D3D9Type;
    using D3D = D3DType;

    WrappedObject(Com<D3D9>&& object)
      : m_d3d9(std::move(object)) {
    }

    D3D9* GetD3D9() {
      return m_d3d9.ptr();
    }

    // For cases where the object may be null.
    static D3D9* GetD3D9Nullable(WrappedObject* self) {
      if (unlikely(self == NULL)) {
        return NULL;
      }
      return self->m_d3d9.ptr();
    }

    template <typename T>
    static D3D9* GetD3D9Nullable(Com<T>& self) {
      return GetD3D9Nullable(self.ptr());
    }

    void SetD3D9(Com<D3D9>&& object) {
      m_d3d9 = std::move(object);
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) final {
      if (ppvObject == nullptr)
        return E_POINTER;

      *ppvObject = nullptr;

      if (riid == __uuidof(IUnknown)) {
      // || riid == __uuidof(D3D)) { // TODO: Need GUIDs
        *ppvObject = this;
        return D3D_OK;
      }

      Logger::warn("D3D8WrappedObject::QueryInterface: Unknown interface query");
      Logger::warn(str::format(riid));

      return E_NOINTERFACE;
    }


  private:

    Com<D3D9> m_d3d9;

  };

}