#pragma once

#include <windows.h>
#include "../util/config/config.h"
#include "../util/util_error.h"

namespace dxvk {

  class D3D9DeviceEx;
  class D3D9InterfaceEx;

  class DECLSPEC_UUID("D3D9ACAB-42A9-4C1E-AA97-DEADFADED420") D3D9Bridge;
  class DECLSPEC_UUID("D3D9ACAB-A407-773E-18E9-CAFEBEEF2000") D3D9InterfaceBridge;

  class D3D9Bridge {

  public:
    using Type = D3D9DeviceEx;

    D3D9Bridge(D3D9DeviceEx* pDevice) : m_device(pDevice) {}
    virtual ~D3D9Bridge() = default;

    ULONG AddRef();
    ULONG Release();

    virtual void SetD3D8Mode();

  private:
    D3D9DeviceEx* m_device;
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
      throw DxvkError("GetD3D9Bridge: ERROR! Failed to get D3D9 Bridge. d3d9.dll might not be DXVK!");
    }
    return pointer;
  }

}

#if !defined(_MSC_VER)
__CRT_UUID_DECL(dxvk::D3D9Bridge, 0xD3D9ACAB, 0x42A9, 0x4C1E, 0xAA, 0x97, 0xDE, 0xAD, 0xFA, 0xDE, 0xD4, 0x20);
__CRT_UUID_DECL(dxvk::D3D9InterfaceBridge, 0xD3D9ACAB, 0xA407, 0x773E, 0x18, 0xE9, 0xCA, 0xFE, 0xBE, 0xEF, 0x20, 0x00);
#endif
