
#include "../wrapper/ddraw.h"

#include "../dd7_surface.h"
#include "../d3d_util.h"
#include "d3d7_device.h"

namespace dxvk {
  Logger Logger::s_instance("ddraw.log");

  ULONG D3D7Interface::AddRef() {
    return ProxyInterface->AddRef();
  }

  ULONG D3D7Interface::Release() {
    return ProxyInterface->Release();
  }

  HRESULT D3D7Interface::EnumDevices(LPD3DENUMDEVICESCALLBACK7 lpEnumDevicesCallback, LPVOID lpUserArg) {

    UINT count = GetD3D9()->GetAdapterCount();
    for (UINT i = 0; i < count; i++) {

      d3d9::D3DCAPS9 caps;
      GetD3D9()->GetDeviceCaps(i, d3d9::D3DDEVTYPE_HAL, &caps);

      D3DPRIMCAPS prim;
      prim.dwSize = sizeof(D3DPRIMCAPS);
      prim.dwMiscCaps           |= 0x7F;       // TODO: D3DPMISCCAPS_
      prim.dwRasterCaps         |= 0xFFFFFFFF; // TODO: D3DPRASTERCAPS_
      prim.dwZCmpCaps           |= 0xFFFFFFFF; // TODO: D3DPCMPCAPS_
      prim.dwSrcBlendCaps       |= 0xFFFFFFFF; // TODO: D3DPBLENDCAPS_
      prim.dwDestBlendCaps      |= 0xFFFFFFFF; // TODO: D3DPBLENDCAPS_
      prim.dwAlphaCmpCaps       |= 0xFFFFFFFF; // TODO: D3DPCMPCAPS_
      prim.dwShadeCaps          |= 0xFFFFFFFF; // TODO: D3DPSHADECAPS_
      prim.dwTextureCaps        |= 0xFFFFFFFF; // TODO: D3DPTEXTURECAPS_
      prim.dwTextureFilterCaps  |= 0xFFFFFFFF; // TODO: D3DPTFILTERCAPS_
      prim.dwTextureBlendCaps   |= 0xFFFFFFFF; // TODO: D3DPTBLENDCAPS_
      prim.dwTextureAddressCaps |= 0xFFFFFFFF; // TODO: D3DPTADDRESSCAPS_
      prim.dwStippleWidth        = 32;
      prim.dwStippleHeight       = 32;


      D3DDEVICEDESC7 desc;
      desc.dwDevCaps = D3DDEVCAPS_FLOATTLVERTEX    // TODO: D3DDEVCAPS_
                     | D3DDEVCAPS_EXECUTESYSTEMMEMORY
                     | D3DDEVCAPS_EXECUTEVIDEOMEMORY
                     | D3DDEVCAPS_TLVERTEXSYSTEMMEMORY
                     | D3DDEVCAPS_TLVERTEXVIDEOMEMORY
                     | D3DDEVCAPS_TEXTURESYSTEMMEMORY
                     | D3DDEVCAPS_TEXTUREVIDEOMEMORY
                     | D3DDEVCAPS_DRAWPRIMTLVERTEX
                     | D3DDEVCAPS_CANRENDERAFTERFLIP
                     | D3DDEVCAPS_TEXTURENONLOCALVIDMEM
                     | D3DDEVCAPS_DRAWPRIMITIVES2
                     | D3DDEVCAPS_SEPARATETEXTUREMEMORIES
                     | D3DDEVCAPS_DRAWPRIMITIVES2EX
                     | D3DDEVCAPS_HWTRANSFORMANDLIGHT
                     | D3DDEVCAPS_CANBLTSYSTONONLOCAL
                     | D3DDEVCAPS_HWRASTERIZATION;
      desc.dpcLineCaps = prim;
      desc.dpcTriCaps  = prim;
      desc.dwDeviceRenderBitDepth   = DDBD_16 | DDBD_24 | DDBD_32;
      desc.dwDeviceZBufferBitDepth  = DDBD_16 | DDBD_24 | DDBD_32;
      desc.dwMinTextureWidth        = 0;
      desc.dwMinTextureHeight       = 0;
      desc.dwMaxTextureWidth        = caps.MaxTextureWidth;
      desc.dwMaxTextureHeight       = caps.MaxTextureHeight;
      desc.dwMaxTextureRepeat       = caps.MaxTextureRepeat;
      desc.dwMaxTextureAspectRatio  = caps.MaxTextureAspectRatio;
      desc.dwMaxAnisotropy          = caps.MaxAnisotropy;
      desc.dvGuardBandLeft          = caps.GuardBandLeft;
      desc.dvGuardBandTop           = caps.GuardBandTop;
      desc.dvGuardBandRight         = caps.GuardBandRight;
      desc.dvGuardBandBottom        = caps.GuardBandBottom;
      desc.dvExtentsAdjust          = caps.ExtentsAdjust;
      desc.dwStencilCaps            = caps.StencilCaps;
      desc.dwFVFCaps                = caps.FVFCaps;
      desc.dwTextureOpCaps          = caps.TextureOpCaps;
      desc.wMaxTextureBlendStages   = (WORD)caps.MaxTextureBlendStages;
      desc.wMaxSimultaneousTextures = (WORD)caps.MaxSimultaneousTextures;
      desc.dwMaxActiveLights        = caps.MaxActiveLights;
      desc.dvMaxVertexW             = caps.MaxVertexW;
      desc.wMaxUserClipPlanes       = (WORD)caps.MaxUserClipPlanes;
      desc.wMaxVertexBlendMatrices  = (WORD)caps.MaxVertexBlendMatrices;
      desc.dwVertexProcessingCaps   = caps.VertexProcessingCaps;

      d3d9::D3DADAPTER_IDENTIFIER9 info;
      GetD3D9()->GetAdapterIdentifier(i, 0, &info);

      desc.deviceGUID = IID_IDirect3DHALDevice;

      Logger::info(str::format("Adapter ", i, " ", info.DeviceName, " ", info.Description));
      
      HRESULT res = lpEnumDevicesCallback(&info.Description[0], &info.DeviceName[0], &desc, lpUserArg);
    }

    /*ProxyInterface->EnumDevices7([](LPSTR description, LPSTR name, LPD3DDEVICEDESC7, LPVOID) -> HRESULT {
      
      Logger::info(str::format("DX7 Adapter ", name, " ", description));

      return D3DENUMRET_OK;
    }, lpUserArg);*/

    //return ProxyInterface->EnumDevices7(lpEnumDevicesCallback, lpUserArg);

    return D3D_OK;
  }

  HRESULT D3D7Interface::CreateDevice(REFCLSID rclsid, LPDIRECTDRAWSURFACE7 lpDDS, LPDIRECT3DDEVICE7* lplpD3DDevice) {

    DDSURFACEDESC2 desc;
    desc.dwSize = sizeof(DDSURFACEDESC2);
    lpDDS->GetSurfaceDesc(&desc);

    Logger::info("CreateDevice. BackBuffer:");
    PrintSurfaceStructure(lpDDS, 1);

    // Clipper of the surface with DDSCAPS_PRIMARYSURFACE
    IDirectDrawClipper* clipper = DD7Surface::s_primaryClipper;

    HWND hwnd;
    clipper->GetHWnd(&hwnd);

    d3d9::D3DPRESENT_PARAMETERS params;
    params.BackBufferWidth    = desc.dwWidth;
    params.BackBufferHeight   = desc.dwHeight;
    params.BackBufferFormat   = d3d9::D3DFMT_UNKNOWN;
    params.BackBufferCount    = 1;
    params.MultiSampleType    = d3d9::D3DMULTISAMPLE_NONE;
    params.MultiSampleQuality = 0;
    params.SwapEffect         = d3d9::D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow      = hwnd;
    params.Windowed           = TRUE;
    params.EnableAutoDepthStencil     = TRUE;
    params.AutoDepthStencilFormat     = d3d9::D3DFMT_D16;
    params.Flags                      = 0;
    params.FullScreen_RefreshRateInHz = 0;
    params.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;

    Com<d3d9::IDirect3DDevice9> device = nullptr;

    HRESULT res = GetD3D9()->CreateDevice(
      1,//(UINT)rclsid.Data1,
      d3d9::D3DDEVTYPE_HAL,
      hwnd,
      D3DCREATE_HARDWARE_VERTEXPROCESSING,
      &params,
      &device
    );
    if (FAILED(res)) return res;

    res = ProxyInterface->CreateDevice(IID_IDirect3DHALDevice, lpDDS, lplpD3DDevice, nullptr);
    if (FAILED(res)) return res;

    *lplpD3DDevice = new D3D7Device(((D3D7Device*)*lplpD3DDevice)->GetProxyInterface(), device.ptr(), lpDDS);
    return D3D_OK;
  }

  HRESULT D3D7Interface::CreateVertexBuffer(LPD3DVERTEXBUFFERDESC a, LPDIRECT3DVERTEXBUFFER7 *b, DWORD c) {
    return ProxyInterface->CreateVertexBuffer(a, b, c, nullptr);
  }

  HRESULT D3D7Interface::EnumZBufferFormats(REFCLSID a, LPD3DENUMPIXELFORMATSCALLBACK b, LPVOID c) {
    return ProxyInterface->EnumZBufferFormats(a, b, c);
  }

  HRESULT D3D7Interface::EvictManagedTextures() {
    return ProxyInterface->EvictManagedTextures();
  }

} // namespace dxvk