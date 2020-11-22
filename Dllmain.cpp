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

#include <windows.h>
#include "Dllmain.h"
#include "d3d8to9\d3d8to9.h"
#include "External\Hooking\Hook.h"
#include "Settings\Settings.h"
#include "Logging\Logging.h"

std::ofstream LOG;

struct IDirect3D8;
typedef IDirect3D8*(WINAPI *Direct3DCreate8Proc)(UINT SDKVersion);
IDirect3D8 *WINAPI _Direct3DCreate8(UINT SDKVersion);

HMODULE m_hModule = nullptr;
HMODULE Patch_dll = nullptr;
HMODULE RealD3d8_dll = nullptr;
HMODULE wrapper_dll = nullptr;
Direct3DCreate8Proc RealDirect3DCreate8_dll = nullptr;
Direct3DCreate8Proc Direct3DCreate8_dll = (Direct3DCreate8Proc)*_Direct3DCreate8;

IDirect3D8 *WINAPI Direct3DCreate8(UINT SDKVersion)
{
	static bool RunOnce = true;
	if (RunOnce)
	{
		RunOnce = false;

		// Init vars
		wchar_t path[MAX_PATH];

		// Game -> Patch
		if (EnableFSSAPatch)
		{
			// Load d3d8patch.dll from exe folder
			if (GetModuleFileName(nullptr, path, sizeof(path)) && wcsrchr(path, '\\'))
			{
				wcscpy_s(wcsrchr(path, '\\'), MAX_PATH - wcslen(path), L"\\d3d8patch.dll");
				Patch_dll = LoadLibrary(path);
			}

			// Direct3DCreate8 -> Patch
			if (Patch_dll)
			{
				Logging::Log() << "Loading 'd3d8patch.dll' APIs...";
				Direct3DCreate8_dll = (Direct3DCreate8Proc)GetProcAddress(Patch_dll, "Direct3DCreate8");
			}
			else
			{
				Logging::Log() << "Error: Could not load 'd3d8patch.dll' patch file!";
			}
		}

		// Patch -> Internal D3d8
		{
			// Load d3d8.dll from System32 folder
			if (GetSystemDirectory(path, MAX_PATH))
			{
				wcscat_s(path, MAX_PATH, L"\\d3d8.dll");
				RealD3d8_dll = LoadLibrary(path);
			}

			// Real_D3d8 -> Direct3DCreate8to9
			if (RealD3d8_dll)
			{
				if (EnableFSSAPatch)
				{
					Logging::Log() << "Hooking System32 d3d8.dll APIs...";
					RealDirect3DCreate8_dll = (Direct3DCreate8Proc)Hook::HotPatch(Hook::GetProcAddress(RealD3d8_dll, "Direct3DCreate8"), "Direct3DCreate8", _Direct3DCreate8);
				}
				else
				{
					Logging::Log() << "Loading System32 d3d8.dll...";
					RealDirect3DCreate8_dll = (Direct3DCreate8Proc)GetProcAddress(RealD3d8_dll, "Direct3DCreate8");
				}
			}
			else
			{
				Logging::Log() << "Error: Could not load System32 'd3d8.dll' file!";
			}
		}

		// Get real d3d8 dll function
		{
			if (d3d8to9)
			{
				RealDirect3DCreate8_dll = (Direct3DCreate8Proc)*Direct3DCreate8to9;
			}
			else
			{
				while (!D3d8WrapperPath.empty())
				{
					wrapper_dll = LoadLibraryA(D3d8WrapperPath.c_str());
					if (wrapper_dll && wrapper_dll != RealD3d8_dll && wrapper_dll != m_hModule && wrapper_dll != Patch_dll)
					{
						Direct3DCreate8Proc WrapperDirect3DCreate8_dll = (Direct3DCreate8Proc)GetProcAddress(wrapper_dll, "Direct3DCreate8");
						if (WrapperDirect3DCreate8_dll)
						{
							LOG_ONCE("Loaded '" << D3d8WrapperPath.c_str() << "'");
							RealDirect3DCreate8_dll = (Direct3DCreate8Proc)*WrapperDirect3DCreate8_dll;
							break;
						}
					}
					Logging::Log() << "Error: Could not load '" << D3d8WrapperPath.c_str() << "' file!";
					break;
				}
			}
		}
	}

	return Direct3DCreate8_dll(SDKVersion);
}

// Dll main function
bool APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		m_hModule = hModule;

		// Get config file path
		wchar_t configpath[MAX_PATH] = {};
		if (GetModuleFileName(hModule, configpath, MAX_PATH) && wcsrchr(configpath, '\\'))
		{
			wchar_t t_name[MAX_PATH] = {};
			wcscpy_s(t_name, MAX_PATH - wcslen(configpath) - 1, wcsrchr(configpath, '\\') + 1);
			if (wcsrchr(configpath, '.'))
			{
				wcscpy_s(wcsrchr(t_name, '.'), MAX_PATH - wcslen(t_name), L"\0");
			}
			wcscpy_s(wcsrchr(configpath, '\\'), MAX_PATH - wcslen(configpath), L"\0");
			std::wstring name(t_name);
			std::transform(name.begin(), name.end(), name.begin(), [](wchar_t c) { return towlower(c); });
			wcscpy_s(configpath, MAX_PATH, std::wstring(std::wstring(configpath) + L"\\" + name + L".ini").c_str());
		}

		// Read config file
		char* szCfg = Read(configpath);

		// Parce config file
		bool IsLoadConfig = false;
		if (szCfg)
		{
			IsLoadConfig = true;
			Parse(szCfg, ParseCallback);
			free(szCfg);
		}

		// Get log file path and open log file
		wchar_t pathname[MAX_PATH];
		if (GetModuleFileName(hModule, pathname, MAX_PATH) && wcsrchr(pathname, '.'))
		{
			wcscpy_s(wcsrchr(pathname, '.'), MAX_PATH - wcslen(pathname), L".log");
			Logging::Open(pathname);
		}

		// Starting
		Logging::Log() << "Starting BloodRayne 2 Patch! v" << APP_VERSION;
		Logging::LogComputerManufacturer();
		Logging::LogVideoCard();
		Logging::LogOSVersion();
		Logging::LogProcessNameAndPID();
		Logging::LogGameType();
		Logging::LogCompatLayer();
	}
	break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Hook::UnhookAll();
		if (Patch_dll)
		{
			FreeLibrary(Patch_dll);
		}
		if (RealD3d8_dll)
		{
			FreeLibrary(RealD3d8_dll);
		}
		if (wrapper_dll)
		{
			FreeLibrary(wrapper_dll);
		}
		break;
	}
	return true;
}
