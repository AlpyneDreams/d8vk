#pragma once

#include <windows.h>


namespace dxvk {

  class D3D9DeviceEx;

  class DECLSPEC_UUID("D3D9ACAB-42A9-4C1E-AA97-DEADFADED420") D3D9Bridge;

  class D3D9Bridge {

  public:
    D3D9Bridge(D3D9DeviceEx* pDevice) : m_device(pDevice) {}
    virtual ~D3D9Bridge() = default;

    ULONG AddRef();
    ULONG Release();

    virtual void SetD3D8Mode();

  private:
    D3D9DeviceEx* m_device;
  };

}

#if !defined(_MSC_VER)
__CRT_UUID_DECL(dxvk::D3D9Bridge, 0xD3D9ACAB, 0x42A9, 0x4C1E, 0xAA, 0x97, 0xDE, 0xAD, 0xFA, 0xDE, 0xD4, 0x20);
#endif
