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

typedef IDirect3D8*(WINAPI *Direct3DCreate8Proc)(UINT SDKVersion);
extern Direct3DCreate8Proc RealDirect3DCreate8_dll;

IDirect3D8 *WINAPI _Direct3DCreate8(UINT SDKVersion)
{
	Logging::Log() << "Redirecting 'Direct3DCreate8' ...";

	LPDIRECT3D8 pD3D8 = RealDirect3DCreate8_dll(SDKVersion);

	if (pD3D8)
	{
		return new m_IDirect3D8(pD3D8);
	}

	Logging::Log() << "Error: 'Direct3DCreate8' failed!";

	return nullptr;
}
