#pragma once

#include "External\d3d8to9\source\d3d8to9.hpp"

#define MAX_D3D9ON12_QUEUES        2

typedef struct _D3D9ON12_ARGS
{
	BOOL Enable9On12;
	IUnknown* pD3D12Device;
	IUnknown* ppD3D12Queues[MAX_D3D9ON12_QUEUES];
	UINT NumQueues;
	UINT NodeMask;
} D3D9ON12_ARGS;

Direct3D8 *WINAPI Direct3DCreate8to9(UINT SDKVersion);
IDirect3D9* WINAPI Direct3DCreate9On12(UINT SDKVersion, D3D9ON12_ARGS* pOverrideList, UINT NumOverrideEntries);
