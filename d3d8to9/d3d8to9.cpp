/**
* Copyright (C) 2025 Elisha Riedlinger
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

#include "External\d3d8to9\source\d3d8to9.hpp"
#include "External\d3d8to9\source\d3dx9.hpp"
#include "d3d8to9.h"
#include "External\Hooking\Hook.h"
#include "Settings\Settings.h"
#include "Logging\Logging.h"
#include "BuildNo.rc"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define APP_VERSION TOSTRING(FILEVERSION)

typedef IDirect3D9* (WINAPI* Direct3DCreate9Proc)(UINT);
typedef IDirect3D9* (WINAPI* Direct3DCreate9On12Proc)(UINT SDKVersion, D3D9ON12_ARGS* pOverrideList, UINT NumOverrideEntries);

extern FARPROC f_D3DXAssembleShader;
extern FARPROC f_D3DXDisassembleShader;
extern FARPROC f_D3DXLoadSurfaceFromSurface;

PFN_D3DXAssembleShader D3DXAssembleShader = (PFN_D3DXAssembleShader)f_D3DXAssembleShader;
PFN_D3DXDisassembleShader D3DXDisassembleShader = (PFN_D3DXDisassembleShader)f_D3DXDisassembleShader;
PFN_D3DXLoadSurfaceFromSurface D3DXLoadSurfaceFromSurface = (PFN_D3DXLoadSurfaceFromSurface)f_D3DXLoadSurfaceFromSurface;

static HMODULE GetSystemD3d9()
{
	static HMODULE h_d3d9 = nullptr;

	// Get System d3d9.dll
	if (!h_d3d9)
	{
		char Path[MAX_PATH] = {};
		GetSystemDirectoryA(Path, MAX_PATH);
		strcat_s(Path, "\\d3d9.dll");
		GetModuleHandleExA(NULL, "d3d9.dll", &h_d3d9);
	}

	return h_d3d9;
}

static FARPROC GetD3d9UnnamedOrdinal(WORD Ordinal)
{
	FARPROC proc = nullptr;

	HMODULE dll = GetSystemD3d9();
	if (!dll)
	{
		Logging::Log() << __FUNCTION__ << " System32 d3d9.dll is not loaded!";
		return nullptr;
	}

	proc = GetProcAddress(dll, reinterpret_cast<LPCSTR>(Ordinal));

	bool FuncNameExists = Hook::CheckExportAddress(dll, proc);

	if (!proc || FuncNameExists)
	{
		Logging::Log() << __FUNCTION__ << " cannot find unnamed ordinal '" << Ordinal << "' in System32 d3d9.dll!";
		return nullptr;
	}

	return proc;
}

static void WINAPI Direct3D9ForceHybridEnumeration(UINT Mode)
{
	const WORD Ordinal = 16;

	static FARPROC proc = nullptr;
	if (!proc)
	{
		FARPROC addr = GetD3d9UnnamedOrdinal(Ordinal);

		if (!addr)
		{
			Logging::Log() << __FUNCTION__ << " Error: Failed to get address!";
			return;
		}

		if (memcmp(addr, "\x8B\xFF\x55\x8B\xEC\x8B\x45\x08\xA3", 9) != S_OK)
		{
			Logging::Log() << __FUNCTION__ << " Error: Failed to vaidate memory address!";
			return;
		}

		proc = addr;
	}

	Logging::Log() << __FUNCTION__ << " Calling 'Direct3D9ForceHybridEnumeration' ... " << Mode;

	reinterpret_cast<decltype(&Direct3D9ForceHybridEnumeration)>(proc)(Mode);
}

Direct3D8 *WINAPI Direct3DCreate8to9(UINT SDKVersion)
{
	UNREFERENCED_PARAMETER(SDKVersion);

	// Load library first
	static HMODULE dll = LoadLibraryA("d3d9.dll");
	if (!dll)
	{
		Logging::Log() << __FUNCTION__ << " Error: could not load d3d9.dll!";
		return nullptr;
	}

	if (GraphicsHybridAdapter)
	{
		Direct3D9ForceHybridEnumeration(1);
	}

	static Direct3DCreate9Proc Direct3DCreate9 = (Direct3DCreate9Proc)GetProcAddress(dll, "Direct3DCreate9");
	if (!Direct3DCreate9)
	{
		Logging::Log() << __FUNCTION__ << " Error: 'Direct3DCreate9' not found!";
		return nullptr;
	}

	LOG_ONCE("Starting D3d8to9 v" << APP_VERSION);

	Logging::Log() << "Redirecting 'Direct3DCreate8' to --> 'Direct3DCreate9' (" << SDKVersion << ")";

	IDirect3D9* d3d = nullptr;

	if (ForceDirect3D9On12)
	{
		// Setup arguments
		D3D9ON12_ARGS args;
		memset(&args, 0, sizeof(args));
		args.Enable9On12 = TRUE;

		// Call function
		d3d = Direct3DCreate9On12(SDKVersion, &args, 1);
	}

	if (!d3d)
	{
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
	}

	if (d3d == nullptr)
	{
		Logging::Log() << __FUNCTION__ << " Error: Failed to create 'Direct3DCreate9'!";
		return nullptr;
	}

	return new Direct3D8(d3d);
}

IDirect3D9* WINAPI Direct3DCreate9On12(UINT SDKVersion, D3D9ON12_ARGS* pOverrideList, UINT NumOverrideEntries)
{
	// Load library first
	static HMODULE dll = GetSystemD3d9();
	if (!dll)
	{
		dll = LoadLibraryA("d3d9.dll");
		if (!dll) return nullptr;
	}

	static Direct3DCreate9On12Proc Direct3DCreate9On12 = (Direct3DCreate9On12Proc)GetProcAddress(dll, "Direct3DCreate9On12");

	if (!Direct3DCreate9On12)
	{
		Logging::Log() << __FUNCTION__ << " Error: 'Direct3DCreate9On12' not found!";
		return nullptr;
	}

	Logging::Log() << "Redirecting 'Direct3DCreate9On12' ...";

	// Create new d3d9 object
	return Direct3DCreate9On12(SDKVersion, pOverrideList, NumOverrideEntries);
}
