#pragma once

class m_IDirect3DMaterialX
{
private:
	IDirect3DMaterial3 *ProxyInterface;
	m_IDirect3DMaterial3 *WrapperInterface;
	DWORD DirectXVersion;
	DWORD ProxyDirectXVersion;
	IID WrapperID;

public:
	m_IDirect3DMaterialX(IDirect3DMaterial3 *aOriginal, DWORD Version, m_IDirect3DMaterial3 *Interface) : ProxyInterface(aOriginal), DirectXVersion(Version), WrapperInterface(Interface)
	{
		WrapperID = (DirectXVersion == 1) ? IID_IDirect3DMaterial :
			(DirectXVersion == 2) ? IID_IDirect3DMaterial2 :
			(DirectXVersion == 3) ? IID_IDirect3DMaterial3 : IID_IDirect3DMaterial3;

		ProxyDirectXVersion = GetIIDVersion(WrapperID);
	}
	~m_IDirect3DMaterialX() {}

	DWORD GetDirectXVersion() { return DDWRAPPER_TYPEX; }
	REFIID GetWrapperType() { return WrapperID; }
	IDirect3DMaterial3 *GetProxyInterface() { return ProxyInterface; }
	m_IDirect3DMaterial3 *GetWrapperInterface() { return WrapperInterface; }

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3DMaterial methods ***/
	STDMETHOD(Initialize)(THIS_ LPDIRECT3D);
	STDMETHOD(SetMaterial)(THIS_ LPD3DMATERIAL);
	STDMETHOD(GetMaterial)(THIS_ LPD3DMATERIAL);
	STDMETHOD(GetHandle)(THIS_ LPDIRECT3DDEVICE3, LPD3DMATERIALHANDLE);
	STDMETHOD(Reserve)(THIS);
	STDMETHOD(Unreserve)(THIS);

	// Helper functions
	IDirect3DMaterial *GetProxyInterfaceV1() { return (IDirect3DMaterial *)ProxyInterface; }
	IDirect3DMaterial2 *GetProxyInterfaceV2() { return (IDirect3DMaterial2 *)ProxyInterface; }
	IDirect3DMaterial3 *GetProxyInterfaceV3() { return ProxyInterface; }
};
