#pragma once

class m_IDirectDrawX
{
private:
	IDirectDraw7 *ProxyInterface;
	m_IDirectDraw7 *WrapperInterface;
	DWORD DirectXVersion;
	DWORD ProxyDirectXVersion;
	IID WrapperID;

public:
	m_IDirectDrawX(IDirectDraw7 *aOriginal, DWORD Version, m_IDirectDraw7 *Interface) : ProxyInterface(aOriginal), DirectXVersion(Version), WrapperInterface(Interface)
	{
		WrapperID = (DirectXVersion == 1) ? IID_IDirectDraw :
			(DirectXVersion == 2) ? IID_IDirectDraw2 :
			(DirectXVersion == 3) ? IID_IDirectDraw3 :
			(DirectXVersion == 4) ? IID_IDirectDraw4 :
			(DirectXVersion == 7) ? IID_IDirectDraw7 : IID_IDirectDraw7;

		ProxyDirectXVersion = GetIIDVersion(WrapperID);
	}
	~m_IDirectDrawX() {}

	DWORD GetDirectXVersion() { return DDWRAPPER_TYPEX; }
	REFIID GetWrapperType() { return WrapperID; }
	IDirectDraw7 *GetProxyInterface() { return ProxyInterface; }
	m_IDirectDraw7 *GetWrapperInterface() { return WrapperInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj);
	STDMETHOD_(ULONG, AddRef) (THIS);
	STDMETHOD_(ULONG, Release) (THIS);

	/*** IDirectDraw methods ***/
	STDMETHOD(Compact)(THIS);
	STDMETHOD(CreateClipper)(THIS_ DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR *);
	STDMETHOD(CreatePalette)(THIS_ DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR *);
	STDMETHOD(CreateSurface)(THIS_ LPDDSURFACEDESC, LPDIRECTDRAWSURFACE7 FAR *, IUnknown FAR *);
	STDMETHOD(CreateSurface2)(THIS_ LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7 FAR *, IUnknown FAR *);
	STDMETHOD(DuplicateSurface)(THIS_ LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 FAR *);
	STDMETHOD(EnumDisplayModes)(THIS_ DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK);
	STDMETHOD(EnumDisplayModes2)(THIS_ DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2);
	STDMETHOD(EnumSurfaces)(THIS_ DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMSURFACESCALLBACK);
	STDMETHOD(EnumSurfaces2)(THIS_ DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7);
	STDMETHOD(FlipToGDISurface)(THIS);
	STDMETHOD(GetCaps)(THIS_ LPDDCAPS, LPDDCAPS);
	STDMETHOD(GetDisplayMode)(THIS_ LPDDSURFACEDESC);
	STDMETHOD(GetDisplayMode2)(THIS_ LPDDSURFACEDESC2);
	STDMETHOD(GetFourCCCodes)(THIS_ LPDWORD, LPDWORD);
	STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE7 FAR *);
	STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD);
	STDMETHOD(GetScanLine)(THIS_ LPDWORD);
	STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL);
	STDMETHOD(Initialize)(THIS_ GUID FAR *);
	STDMETHOD(RestoreDisplayMode)(THIS);
	STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD);
	STDMETHOD(SetDisplayMode)(THIS_ DWORD, DWORD, DWORD, DWORD, DWORD);
	STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD, HANDLE);

	/*** Added in the v2 interface ***/
	STDMETHOD(GetAvailableVidMem) (THIS_ LPDDSCAPS, LPDWORD, LPDWORD);
	STDMETHOD(GetAvailableVidMem2) (THIS_ LPDDSCAPS2, LPDWORD, LPDWORD);

	/*** Added in the V4 Interface ***/
	STDMETHOD(GetSurfaceFromDC) (THIS_ HDC, LPDIRECTDRAWSURFACE7 *);
	STDMETHOD(RestoreAllSurfaces)(THIS);
	STDMETHOD(TestCooperativeLevel)(THIS);
	STDMETHOD(GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER, DWORD);
	STDMETHOD(GetDeviceIdentifier2)(THIS_ LPDDDEVICEIDENTIFIER2, DWORD);

	/*** Added in the V7 Interface ***/
	STDMETHOD(StartModeTest)(THIS_ LPSIZE, DWORD, DWORD);
	STDMETHOD(EvaluateMode)(THIS_ DWORD, DWORD *);

	// Helper functions
	IDirectDraw *GetProxyInterfaceV1() { return (IDirectDraw *)ProxyInterface; }
	IDirectDraw2 *GetProxyInterfaceV2() { return (IDirectDraw2 *)ProxyInterface; }
	IDirectDraw3 *GetProxyInterfaceV3() { return (IDirectDraw3 *)ProxyInterface; }
	IDirectDraw4 *GetProxyInterfaceV4() { return (IDirectDraw4 *)ProxyInterface; }
	IDirectDraw7 *GetProxyInterfaceV7() { return (IDirectDraw7 *)ProxyInterface; }
};
