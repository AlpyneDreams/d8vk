#pragma once

class m_IDirect3DX
{
private:
	IDirect3D7 *ProxyInterface;
	m_IDirect3D7 *WrapperInterface;
	DWORD DirectXVersion;
	DWORD ProxyDirectXVersion;
	IID WrapperID;
	m_IDirectDrawX *ddrawParent = nullptr;

public:
	m_IDirect3DX(IDirect3D7 *aOriginal, DWORD Version, m_IDirect3D7 *Interface) : ProxyInterface(aOriginal), DirectXVersion(Version), WrapperInterface(Interface)
	{
		WrapperID = (DirectXVersion == 1) ? IID_IDirect3D :
			(DirectXVersion == 2) ? IID_IDirect3D2 :
			(DirectXVersion == 3) ? IID_IDirect3D3 :
			(DirectXVersion == 7) ? IID_IDirect3D7 : IID_IDirect3D7;

		ProxyDirectXVersion = GetIIDVersion(WrapperID);
	}
	~m_IDirect3DX() {}

	DWORD GetDirectXVersion() { return DDWRAPPER_TYPEX; }
	REFIID GetWrapperType() { return WrapperID; }
	IDirect3D7 *GetProxyInterface() { return ProxyInterface; }
	m_IDirect3D7 *GetWrapperInterface() { return WrapperInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3D methods ***/
	STDMETHOD(Initialize)(THIS_ REFCLSID);
	STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK, LPVOID);
	STDMETHOD(EnumDevices7)(THIS_ LPD3DENUMDEVICESCALLBACK7, LPVOID);
	STDMETHOD(CreateLight)(THIS_ LPDIRECT3DLIGHT*, LPUNKNOWN);
	STDMETHOD(CreateMaterial)(THIS_ LPDIRECT3DMATERIAL3*, LPUNKNOWN);
	STDMETHOD(CreateViewport)(THIS_ LPDIRECT3DVIEWPORT3*, LPUNKNOWN);
	STDMETHOD(FindDevice)(THIS_ LPD3DFINDDEVICESEARCH, LPD3DFINDDEVICERESULT);
	STDMETHOD(CreateDevice)(THIS_ REFCLSID, LPDIRECTDRAWSURFACE7, LPDIRECT3DDEVICE7*, LPUNKNOWN);
	STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC, LPDIRECT3DVERTEXBUFFER7*, DWORD, LPUNKNOWN);
	STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID, LPD3DENUMPIXELFORMATSCALLBACK, LPVOID);
	STDMETHOD(EvictManagedTextures)(THIS);

	// Helper functions
	IDirect3D *GetProxyInterfaceV1() { return (IDirect3D *)ProxyInterface; }
	IDirect3D2 *GetProxyInterfaceV2() { return (IDirect3D2 *)ProxyInterface; }
	IDirect3D3 *GetProxyInterfaceV3() { return (IDirect3D3 *)ProxyInterface; }
	IDirect3D7 *GetProxyInterfaceV7() { return ProxyInterface; }
};
