#pragma once

#include <windows.h>
#include "../util/config/config.h"
#include "../util/util_error.h"
#include "../util/log/log.h"

#include "../vulkan/vulkan_loader.h"

#include <bitset>

// NOTE: You must include "d3d9_include.h" or "d3d8_include.h" before this header.

/**
 * The D3D9 bridge allows D3D8 to access DXVK internals.
 * For Vulkan interop without needing DXVK internals, see d3d9_interop.h.
 */

namespace dxvk {

  class D3D9DeviceEx;
  class D3D9InterfaceEx;

#if defined(_MSC_VER)
  class DECLSPEC_UUID("D3D9ACAB-42A9-4C1E-AA97-DEADFADED420") D3D9Bridge;
  class DECLSPEC_UUID("D3D9ACAB-A407-773E-18E9-CAFEBEEF2000") D3D9InterfaceBridge;
#endif

  class D3D9Bridge {

  // D3D8 keeps D3D9 objects contained in a namespace.
  #ifdef DXVK_D3D9_NAMESPACE
    using IDirect3DSurface9 = d3d9::IDirect3DSurface9;
  #endif

  public:
    using Type = D3D9DeviceEx;

    D3D9Bridge(D3D9DeviceEx* pDevice) : m_device(pDevice) {}
    virtual ~D3D9Bridge() = default;

    ULONG AddRef();
    ULONG Release();

    virtual void SetAPIName(const char* name);
    virtual HRESULT UpdateTextureFromBuffer(
        IDirect3DSurface9*        pDestSurface,
        IDirect3DSurface9*        pSrcSurface,
        const RECT*               pSrcRect,
        const POINT*              pDestPoint);


    // Indicate that D3D9 shouldn't complain about an unhandled
    // D3DRENDERSTATE because it's implemented elsewhere.
    template <typename... Args>
    inline void AddSupportedRenderStates(Args... state) {
      ((m_supportedRenderStates[state] = true), ...);
    }

    inline void RenderStateNotSupported(uint8_t renderState) {
      m_supportedRenderStates[renderState] = false;
    }

    constexpr bool IsRenderStateSupported(uint8_t renderState) const {
      return m_supportedRenderStates[renderState];
    }

  private:
    D3D9DeviceEx*           m_device;
    std::bitset<UINT8_MAX>  m_supportedRenderStates;
  };

  class D3D9InterfaceBridge {

  public:
    using Type = D3D9InterfaceEx;

    D3D9InterfaceBridge(D3D9InterfaceEx* pObject) : m_interface(pObject) {}
    virtual ~D3D9InterfaceBridge() = default;

    ULONG AddRef();
    ULONG Release();

    virtual const Config& GetConfig() const;
    
  protected:
    D3D9InterfaceEx* m_interface;
  };

  // Get a bridge interface to D3D9 (D3D9Bridge, D3D9InterfaceBridge).
  // Pass this the relevant D3D9 object.
  template <typename B, typename T = typename B::Type>
  B* GetD3D9Bridge(T* object) {
    B* pointer;
    if (FAILED(object->QueryInterface(__uuidof(B), (void**)&pointer))) {
      Logger::err("GetD3D9Bridge: ERROR! Failed to get D3D9 Bridge. d3d9.dll might be DXVK, but not a version built for D8VK!");
      Logger::err("Please install the d3d9.dll that came with d3d8.dll");
      throw DxvkError("GetD3D9Bridge: ERROR! Failed to get D3D9 Bridge. d3d9.dll might be DXVK, but not a version built for D8VK!");
    }
    return pointer;
  }

}

#if !defined(_MSC_VER)
__CRT_UUID_DECL(dxvk::D3D9Bridge, 0xD3D9ACAB, 0x42A9, 0x4C1E, 0xAA, 0x97, 0xDE, 0xAD, 0xFA, 0xDE, 0xD4, 0x20);
__CRT_UUID_DECL(dxvk::D3D9InterfaceBridge, 0xD3D9ACAB, 0xA407, 0x773E, 0x18, 0xE9, 0xCA, 0xFE, 0xBE, 0xEF, 0x20, 0x00);
#endif
