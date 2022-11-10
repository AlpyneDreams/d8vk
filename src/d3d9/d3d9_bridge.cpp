
#include "d3d9_device.h"
#include "d3d9_interface.h"
#include "d3d9_bridge.h"
#include "d3d9_swapchain.h"
#include "d3d9_surface.h"

namespace dxvk {

  ULONG D3D9Bridge::AddRef() {
    return m_device->AddRef();
  }

  ULONG D3D9Bridge::Release() {
    return m_device->Release();
  }

  void D3D9Bridge::SetAPIName(const char* name) {
    m_device->m_implicitSwapchain->SetApiName(name);
  }

  HRESULT D3D9Bridge::UpdateTextureFromBuffer(
        IDirect3DSurface9*  pDestSurface,
        IDirect3DSurface9*  pSrcSurface,
        const RECT*         pSrcRect,
        const POINT*        pDestPoint) {
    D3D9Surface* dst = static_cast<D3D9Surface*>(pDestSurface);
    D3D9Surface* src = static_cast<D3D9Surface*>(pSrcSurface);

    if (unlikely(dst == nullptr || src == nullptr))
      return D3DERR_INVALIDCALL;

    D3D9CommonTexture* srcTextureInfo = src->GetCommonTexture();
    D3D9CommonTexture* dstTextureInfo = dst->GetCommonTexture();

    VkOffset3D srcOffset = { 0u, 0u, 0u };
    VkOffset3D dstOffset = { 0u, 0u, 0u };
    VkExtent3D texLevelExtent = srcTextureInfo->GetExtentMip(src->GetSubresource());
    VkExtent3D extent = texLevelExtent;

    srcOffset = { pSrcRect->left,
                  pSrcRect->top,
                  0u };

    extent = { uint32_t(pSrcRect->right - pSrcRect->left), uint32_t(pSrcRect->bottom - pSrcRect->top), 1 };

    // TODO: Validate extents like in D3D9DeviceEx::UpdateSurface

    dstOffset = { pDestPoint->x,
                  pDestPoint->y,
                  0u };


    m_device->UpdateTextureFromBuffer(
      srcTextureInfo, dstTextureInfo,
      src->GetSubresource(), dst->GetSubresource(),
      srcOffset, extent, dstOffset
    );

    return D3D_OK;
  }


  ULONG D3D9InterfaceBridge::AddRef() {
    return m_interface->AddRef();
  }

  ULONG D3D9InterfaceBridge::Release() {
    return m_interface->Release();
  }

  const Config& D3D9InterfaceBridge::GetConfig() const {
    return m_interface->GetInstance()->config();
  }
}