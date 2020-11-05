#pragma once

#include <string>

#define VISIT_BOOL_SETTINGS(visit) \
	visit(d3d8to9, true) \
	visit(EnableFSSAPatch, true) \
	visit(FullscreenWndMode, true)

#define VISIT_STR_SETTINGS(visit) \
	visit(D3d8WrapperPath, "")

// Configurable setting defaults
#define DECLARE_BOOL_SETTINGS(name, unused) \
	extern bool name;

VISIT_BOOL_SETTINGS(DECLARE_BOOL_SETTINGS);

#define DECLARE_STR_SETTINGS(name, unused) \
	extern std::string name;

VISIT_STR_SETTINGS(DECLARE_STR_SETTINGS);

typedef void(__stdcall* NV)(char* name, char* value);

char* Read(wchar_t* szFileName);
void Parse(char* str, NV NameValueCallback);
void __stdcall ParseCallback(char* lpName, char* lpValue);
void __stdcall LogCallback(char* lpName, char* lpValue);
