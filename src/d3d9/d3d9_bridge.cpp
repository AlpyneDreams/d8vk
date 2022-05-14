
#include "d3d9_bridge.h"
#include "d3d9_device.h"
#include "d3d9_interface.h"
#include "d3d9_swapchain.h"

namespace dxvk {

  ULONG D3D9Bridge::AddRef() {
    return m_device->AddRef();
  }

  ULONG D3D9Bridge::Release() {
    return m_device->Release();
  }

  void D3D9Bridge::SetD3D8Mode() {
    m_device->m_implicitSwapchain->SetApiName("D3D8");
  }


  ULONG D3D9InterfaceBridge::AddRef() {
    return m_interface->AddRef();
  }

  ULONG D3D9InterfaceBridge::Release() {
    return m_interface->Release();
  }
}