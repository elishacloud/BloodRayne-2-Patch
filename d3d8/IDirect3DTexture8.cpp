/**
* Copyright (C) 2020 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "d3d8.h"

HRESULT m_IDirect3DTexture8::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DTexture8 || riid == IID_IUnknown || riid == IID_IDirect3DResource8 || riid == IID_IDirect3DBaseTexture8) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	HRESULT hr = ProxyInterface->QueryInterface(riid, ppvObj);

	if (SUCCEEDED(hr))
	{
		genericQueryInterface(riid, ppvObj, m_pDevice);
	}

	return hr;
}

ULONG m_IDirect3DTexture8::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DTexture8::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DTexture8::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDevice->AddRef();

	*ppDevice = m_pDevice;

	return D3D_OK;
}

HRESULT m_IDirect3DTexture8::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return ProxyInterface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT m_IDirect3DTexture8::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return ProxyInterface->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT m_IDirect3DTexture8::FreePrivateData(THIS_ REFGUID refguid)
{
	return ProxyInterface->FreePrivateData(refguid);
}

DWORD m_IDirect3DTexture8::SetPriority(THIS_ DWORD PriorityNew)
{
	return ProxyInterface->SetPriority(PriorityNew);
}

DWORD m_IDirect3DTexture8::GetPriority(THIS)
{
	return ProxyInterface->GetPriority();
}

void m_IDirect3DTexture8::PreLoad(THIS)
{
	return ProxyInterface->PreLoad();
}

D3DRESOURCETYPE m_IDirect3DTexture8::GetType(THIS)
{
	return ProxyInterface->GetType();
}

DWORD m_IDirect3DTexture8::SetLOD(THIS_ DWORD LODNew)
{
	return ProxyInterface->SetLOD(LODNew);
}

DWORD m_IDirect3DTexture8::GetLOD(THIS)
{
	return ProxyInterface->GetLOD();
}

DWORD m_IDirect3DTexture8::GetLevelCount(THIS)
{
	return ProxyInterface->GetLevelCount();
}

HRESULT m_IDirect3DTexture8::GetLevelDesc(THIS_ UINT Level, D3DSURFACE_DESC *pDesc)
{
	return ProxyInterface->GetLevelDesc(Level, pDesc);
}

HRESULT m_IDirect3DTexture8::GetSurfaceLevel(THIS_ UINT Level, IDirect3DSurface8** ppSurfaceLevel)
{
	HRESULT hr = ProxyInterface->GetSurfaceLevel(Level, ppSurfaceLevel);

	if (SUCCEEDED(hr) && ppSurfaceLevel)
	{
		*ppSurfaceLevel = m_pDevice->ProxyAddressLookupTable->FindAddress<m_IDirect3DSurface8>(*ppSurfaceLevel);
	}

	return hr;
}

HRESULT m_IDirect3DTexture8::LockRect(THIS_ UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return ProxyInterface->LockRect(Level, pLockedRect, pRect, Flags);
}

HRESULT m_IDirect3DTexture8::UnlockRect(THIS_ UINT Level)
{
	return ProxyInterface->UnlockRect(Level);
}

HRESULT m_IDirect3DTexture8::AddDirtyRect(THIS_ CONST RECT* pDirtyRect)
{
	return ProxyInterface->AddDirtyRect(pDirtyRect);
}
