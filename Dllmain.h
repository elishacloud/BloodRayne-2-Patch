#pragma once

#include <windows.h>
#include "BuildNo.rc"

// Included resource files
#define IDR_SH2WID   101
#define IDR_SH2FOG   102
#define IDR_SH2UPD   103

// Main resource file details
#define APP_NAME				"BloodRayne 2 Patch Module"
#define APP_MAJOR				1
#define APP_MINOR				0
#define APP_BUILDNUMBER			BUILD_NUMBER
#define APP_REVISION			0
#define APP_COMPANYNAME			"Sadrate Presents"
#define APP_DESCRPTION			"A project designed to fix the BloodRayne 2 'BR2 FSAA Patch 1.666' patch on Windows 8.1 and Windows 10."
#define APP_COPYRIGHT			"Copyright (C) 2019 Elisha Riedlinger"
#define APP_ORIGINALVERSION		"d3d8.dll"
#define APP_INTERNALNAME		"BloodRayne-2-Patch"

// Get APP_VERSION
#define _TO_STRING_(x) #x
#define _TO_STRING(x) _TO_STRING_(x)
#define APP_VERSION _TO_STRING(APP_MAJOR) "." _TO_STRING(APP_MINOR) "." _TO_STRING(APP_BUILDNUMBER) "." _TO_STRING(APP_REVISION)
