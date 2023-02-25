#pragma once

#include "d3d_include.h"

namespace dxvk {
  inline d3d9::D3DFORMAT ConvertFormat(DDPIXELFORMAT& fmt) {
    if (fmt.dwFlags & DDPF_RGB) {
      // R bitmask: 0111 1100 0000 0000
      // G bitmask: 0000 0011 1110 0000
      // B bitmask: 0000 0000 0001 1111
      switch (fmt.dwRGBBitCount) {
        case 16:
          return d3d9::D3DFMT_X1R5G5B5;
        case 32:
          return d3d9::D3DFMT_A8R8G8B8;
      }
    }
    return d3d9::D3DFMT_A8B8G8R8;
  }

  inline d3d9::D3DCUBEMAP_FACES GetCubemapFace(DDSURFACEDESC2* desc) {
    if (desc->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEX) return d3d9::D3DCUBEMAP_FACE_POSITIVE_X;
    if (desc->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEX) return d3d9::D3DCUBEMAP_FACE_NEGATIVE_X;
    if (desc->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEY) return d3d9::D3DCUBEMAP_FACE_POSITIVE_Y;
    if (desc->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEY) return d3d9::D3DCUBEMAP_FACE_NEGATIVE_Y;
    if (desc->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEZ) return d3d9::D3DCUBEMAP_FACE_POSITIVE_Z;
    if (desc->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEZ) return d3d9::D3DCUBEMAP_FACE_NEGATIVE_Z;
    return d3d9::D3DCUBEMAP_FACE_POSITIVE_X;
  }

  inline void PrintSurfaceStructure(IDirectDrawSurface7* pSurface, int indent = 0, const LPDDSURFACEDESC2 pdesc = nullptr) {
    DDSURFACEDESC2 desc;
    if (pdesc) {
      desc = *pdesc;
    } else {
      desc.dwSize = sizeof(DDSURFACEDESC2);
      pSurface->GetSurfaceDesc(&desc);
    }

    std::string flags = "";
    std::string prefix = std::string(indent * 4, ' ');

    if (!(desc.dwFlags & DDSD_CAPS))
      flags += "Unknown Surface ";
    else
      flags += "Surface ";

    if ((desc.ddsCaps.dwCaps & DDSD_WIDTH) && (desc.ddsCaps.dwCaps & DDSD_HEIGHT))
      flags += "(" + std::to_string(desc.dwWidth) + "x" + std::to_string(desc.dwHeight) + ")";

    #define CHECK_FLAG2(flag, name) if ((desc.ddsCaps.dwCaps & DDSCAPS_ ## flag) == DDSCAPS_ ## flag) flags += #name " "
    #define CHECK_FLAG(flag) CHECK_FLAG2(flag, flag)
    CHECK_FLAG(3DDEVICE);
    CHECK_FLAG(ALPHA);
    CHECK_FLAG(BACKBUFFER);
    CHECK_FLAG(COMPLEX);
    CHECK_FLAG(FLIP);
    CHECK_FLAG(FRONTBUFFER);
    CHECK_FLAG(HWCODEC);
    CHECK_FLAG(LIVEVIDEO);
    CHECK_FLAG(LOCALVIDMEM);
    CHECK_FLAG(MIPMAP);
    CHECK_FLAG(MODEX);
    CHECK_FLAG(NONLOCALVIDMEM);
    CHECK_FLAG(OFFSCREENPLAIN);
    CHECK_FLAG(OVERLAY);
    CHECK_FLAG(OWNDC);
    CHECK_FLAG(PALETTE);
    CHECK_FLAG(PRIMARYSURFACE);
    CHECK_FLAG(STANDARDVGAMODE);
    CHECK_FLAG(SYSTEMMEMORY);
    CHECK_FLAG(TEXTURE);
    CHECK_FLAG(VIDEOMEMORY);
    CHECK_FLAG(VIDEOPORT);
    CHECK_FLAG(VISIBLE);
    CHECK_FLAG(WRITEONLY);
    CHECK_FLAG(ZBUFFER);

    Logger::info(str::format(prefix, pSurface, " ", flags));

    if (desc.dwFlags & DDSCAPS_COMPLEX) {
      pSurface->EnumAttachedSurfaces(&indent,
        [](LPDIRECTDRAWSURFACE7 surf, LPDDSURFACEDESC2 pDesc, LPVOID pContext) -> HRESULT {
          PrintSurfaceStructure(surf, *(int*)pContext + 1, pDesc);
          return DDENUMRET_OK;
        }
      );
    }
  }
}