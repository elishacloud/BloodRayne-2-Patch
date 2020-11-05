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

HWND DeviceWindow = nullptr;
LONG BufferWidth = 0, BufferHeight = 0;

HRESULT m_IDirect3D8::QueryInterface(REFIID riid, LPVOID *ppvObj)
{
	Logging::LogDebug() << __FUNCTION__;

	if ((riid == IID_IDirect3D8 || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	return ProxyInterface->QueryInterface(riid, ppvObj);
}

ULONG m_IDirect3D8::AddRef()
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->AddRef();
}

ULONG m_IDirect3D8::Release()
{
	Logging::LogDebug() << __FUNCTION__;

	ULONG count = ProxyInterface->Release();

	if (count == 0)
	{
		delete this;
	}

	return count;
}

HRESULT m_IDirect3D8::EnumAdapterModes(THIS_ UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->EnumAdapterModes(Adapter, Mode, pMode);
}

UINT m_IDirect3D8::GetAdapterCount()
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->GetAdapterCount();
}

HRESULT m_IDirect3D8::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT m_IDirect3D8::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIdentifier)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT m_IDirect3D8::GetAdapterModeCount(THIS_ UINT Adapter)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->GetAdapterModeCount(Adapter);
}

HMONITOR m_IDirect3D8::GetAdapterMonitor(UINT Adapter)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->GetAdapterMonitor(Adapter);
}

HRESULT m_IDirect3D8::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 *pCaps)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HRESULT m_IDirect3D8::RegisterSoftwareDevice(void *pInitializeFunction)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->RegisterSoftwareDevice(pInitializeFunction);
}

HRESULT m_IDirect3D8::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT m_IDirect3D8::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	Logging::LogDebug() << __FUNCTION__;

	return ProxyInterface->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT m_IDirect3D8::CheckDeviceMultiSampleType(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
{
	Logging::LogDebug() << __FUNCTION__;

	if (FullscreenWndMode)
	{
		Windowed = true;
	}

	return ProxyInterface->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
}

HRESULT m_IDirect3D8::CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
	Logging::LogDebug() << __FUNCTION__;

	if (FullscreenWndMode)
	{
		Windowed = true;
	}

	return ProxyInterface->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

DWORD GetBitCount(D3DFORMAT Format)
{
	switch (Format)
	{
	case D3DFMT_UNKNOWN:
		break;
	case D3DFMT_R3G3B2:
	case D3DFMT_A8:
	case D3DFMT_P8:
	case D3DFMT_L8:
	case D3DFMT_A4L4:
		return 8;
	case D3DFMT_R5G6B5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A4R4G4B4:
	case D3DFMT_A8R3G3B2:
	case D3DFMT_X4R4G4B4:
	case D3DFMT_A8P8:
	case D3DFMT_A8L8:
	case D3DFMT_V8U8:
	case D3DFMT_L6V5U5:
	case D3DFMT_D16_LOCKABLE:
	case D3DFMT_D15S1:
	case D3DFMT_D16:
	case D3DFMT_UYVY:
	case D3DFMT_YUY2:
		return 16;
	case D3DFMT_R8G8B8:
		return 24;
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_A2B10G10R10:
	case D3DFMT_G16R16:
	case D3DFMT_X8L8V8U8:
	case D3DFMT_Q8W8V8U8:
	case D3DFMT_V16U16:
	case D3DFMT_A2W10V10U10:
	case D3DFMT_D32:
	case D3DFMT_D24S8:
	case D3DFMT_D24X8:
	case D3DFMT_D24X4S4:
		return 32;
	case D3DFMT_DXT1:
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
		break;
	}

	LOG_LIMIT(100, __FUNCTION__ << " Display format not Implemented: " << Format);
	return 0;
}

HRESULT m_IDirect3D8::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface)
{
	Logging::LogDebug() << __FUNCTION__;

	// Get device information
	static bool RunOnce = true;
	if (RunOnce)
	{
		RunOnce = false;
		D3DADAPTER_IDENTIFIER8 dai;
		if (SUCCEEDED(ProxyInterface->GetAdapterIdentifier(Adapter, 0, &dai)))
		{
			Logging::Log() << "|---------- VIDEO CARD ----------";
			Logging::Log() << "| " __FUNCTION__ << " Using video card: " << dai.Description << " (" << Logging::hex(dai.VendorId) << ")";
			Logging::Log() << "|--------------------------------";
		}
	}

	// Update presentation parameters
	UpdatePresentParameter(pPresentationParameters, &hFocusWindow, true);

	// Set game window to forground
	SetForegroundWindow(DeviceWindow);

	HRESULT hr = ProxyInterface->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	if (SUCCEEDED(hr) && ppReturnedDeviceInterface)
	{
		*ppReturnedDeviceInterface = new m_IDirect3DDevice8(*ppReturnedDeviceInterface, this);
	}
	else
	{
		Logging::Log() << __FUNCTION__ << " Failed to CreateDevice!";
	}

	return hr;
}

// Set Presentation Parameters
void UpdatePresentParameter(D3DPRESENT_PARAMETERS* pPresentationParameters, HWND *hFocusWindow, bool SetWindow)
{
	if (!pPresentationParameters)
	{
		return;
	}

	BufferWidth = (pPresentationParameters->BackBufferWidth) ? pPresentationParameters->BackBufferWidth : BufferWidth;
	BufferHeight = (pPresentationParameters->BackBufferHeight) ? pPresentationParameters->BackBufferHeight : BufferHeight;

	DeviceWindow = (pPresentationParameters->hDeviceWindow) ? pPresentationParameters->hDeviceWindow :
		(hFocusWindow && *hFocusWindow) ? *hFocusWindow : DeviceWindow;

	// Check if window is minimized and restore it
	if (IsWindow(DeviceWindow) && IsIconic(DeviceWindow))
	{
		ShowWindow(DeviceWindow, SW_RESTORE);
	}

	// Set window size if window mode is enabled
	if (FullscreenWndMode && IsWindow(DeviceWindow))
	{
		pPresentationParameters->Windowed = true;
		pPresentationParameters->FullScreen_RefreshRateInHz = 0;
		if (pPresentationParameters->SwapEffect)
		{
			pPresentationParameters->FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}

		if (SetWindow)
		{
			if (!BufferWidth || !BufferHeight)
			{
				RECT tempRect;
				GetClientRect(DeviceWindow, &tempRect);
				BufferWidth = tempRect.right;
				BufferHeight = tempRect.bottom;
			}
			if (FullscreenWndMode)
			{
				DEVMODE newSettings;
				ZeroMemory(&newSettings, sizeof(newSettings));
				if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &newSettings) != 0)
				{
					newSettings.dmPelsWidth = BufferWidth;
					newSettings.dmPelsHeight = BufferHeight;
					newSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
					ChangeDisplaySettings(&newSettings, CDS_FULLSCREEN);
				}
			}
			AdjustWindow(DeviceWindow, BufferWidth, BufferHeight);
		}
	}
}

void GetDesktopRes(LONG &screenWidth, LONG &screenHeight)
{
	HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);
	MONITORINFO info = {};
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	screenWidth = info.rcMonitor.right - info.rcMonitor.left;
	screenHeight = info.rcMonitor.bottom - info.rcMonitor.top;
}

// Adjusting the window position for WindowMode
void AdjustWindow(HWND MainhWnd, LONG displayWidth, LONG displayHeight)
{
	if (!MainhWnd || !displayWidth || !displayHeight)
	{
		Logging::Log() << __FUNCTION__ << " Error: could not set window size, nullptr.";
		return;
	}

	// Get screen width and height
	LONG screenWidth = 0, screenHeight = 0;
	GetDesktopRes(screenWidth, screenHeight);

	// Set window active and focus
	SetActiveWindow(MainhWnd);
	SetFocus(MainhWnd);

	// Get window border
	LONG lStyle = GetWindowLong(MainhWnd, GWL_STYLE) | WS_VISIBLE;
	if (screenHeight > displayHeight + 32)
	{
		lStyle |= WS_OVERLAPPEDWINDOW;
	}
	else
	{
		lStyle &= ~WS_OVERLAPPEDWINDOW;
	}

	// Set window border
	SetWindowLong(MainhWnd, GWL_STYLE, lStyle);
	SetWindowPos(MainhWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// Set window size
	SetWindowPos(MainhWnd, nullptr, 0, 0, displayWidth, displayHeight, SWP_NOMOVE | SWP_NOZORDER);

	// Adjust for window decoration to ensure client area matches display size
	RECT tempRect;
	GetClientRect(MainhWnd, &tempRect);
	LONG newDisplayWidth = (displayWidth - tempRect.right) + displayWidth;
	LONG newDisplayHeight = (displayHeight - tempRect.bottom) + displayHeight;

	// Move window to center and adjust size
	LONG xLoc = 0;
	LONG yLoc = 0;
	if (screenWidth >= newDisplayWidth && screenHeight >= newDisplayHeight)
	{
		xLoc = (screenWidth - newDisplayWidth) / 2;
		yLoc = (screenHeight - newDisplayHeight) / 2;
	}
	SetWindowPos(MainhWnd, nullptr, xLoc, yLoc, newDisplayWidth, newDisplayHeight, SWP_NOZORDER);
}
