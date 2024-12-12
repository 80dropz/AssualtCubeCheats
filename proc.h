#pragma once
#include "Includes.h"
#include <Windows.h>

DWORD GetProcessID(const wchar_t* procName);

DWORD GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
