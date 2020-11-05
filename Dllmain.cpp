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
#include "Logging\Logging.h"
#include "External\Hooking\Hook.h"

std::ofstream LOG;

typedef Direct3D8*(WINAPI *Direct3DCreate8Proc)(UINT SDKVersion);
Direct3D8 *WINAPI _Direct3DCreate8(UINT SDKVersion);

Direct3DCreate8Proc RealDirect3DCreate8_dll = nullptr;
Direct3DCreate8Proc Direct3DCreate8_dll = (Direct3DCreate8Proc)*_Direct3DCreate8;

extern "C" __declspec(naked) void __stdcall Direct3DCreate8()
{
	__asm mov edi, edi
	__asm jmp Direct3DCreate8_dll
}

Direct3D8 *WINAPI _Direct3DCreate8(UINT SDKVersion)
{
	return Direct3DCreate8to9(SDKVersion);
}

// Dll main function
bool APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	static HMODULE RealD3d8_dll = nullptr;
	static HMODULE Patch_dll = nullptr;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
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

		// Init vars
		wchar_t path[MAX_PATH];

		// Game -> Patch
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
				Logging::Log() << "Hooking 'd3d8patch.dll' APIs...";
				Direct3DCreate8_dll = (Direct3DCreate8Proc)Hook::GetProcAddress(Patch_dll, "Direct3DCreate8");
			}
			else
			{
				Logging::Log() << "Could not load 'd3d8patch.dll' patch file!";
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
				Logging::Log() << "Hooking System32 d3d8.dll APIs...";
				RealDirect3DCreate8_dll = (Direct3DCreate8Proc)Hook::HotPatch(Hook::GetProcAddress(RealD3d8_dll, "Direct3DCreate8"), "Direct3DCreate8", _Direct3DCreate8);
			}
		}
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
		break;
	}
	return true;
}
