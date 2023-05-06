#include "d3d8_interface.h"

namespace dxvk {

  static struct LoggerD3D8 {
    LoggerD3D8() {
      Logger::setLogFile("d3d8.log");
    }
  } s_instance;

  HRESULT CreateD3D8(
          UINT         SDKVersion,
          IDirect3D8** ppDirect3D8) {
    if (!ppDirect3D8)
      return D3DERR_INVALIDCALL;

    *ppDirect3D8 = ref(new D3D8InterfaceEx(SDKVersion));
    return D3D_OK;
  }
}

extern "C" {  
  DLLEXPORT IDirect3D8* __stdcall Direct3DCreate8(UINT nSDKVersion) {
    dxvk::Logger::trace("Direct3DCreate8 called");

    IDirect3D8* pDirect3D = nullptr;
    dxvk::CreateD3D8(false, &pDirect3D);

    return pDirect3D;
  }
}
