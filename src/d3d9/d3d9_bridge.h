#pragma once

#include <windows.h>


namespace dxvk {

  class D3D9DeviceEx;

  class DECLSPEC_UUID("D3D9ACAB-42A9-4C1E-AA97-DEADFADED420") D3D9Bridge;

  class D3D9Bridge {


  public:
    D3D9Bridge(D3D9DeviceEx* pDevice) : m_device(pDevice) {}

    ULONG AddRef();
    ULONG Release();

    void SetD3D8Mode();

  private:
    D3D9DeviceEx* m_device;
  };

}