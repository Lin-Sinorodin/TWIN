#pragma once

#include <windows.h>
#include <iostream>

// Registry path to the Logon process 

constexpr HKEY LOGON_KEY{ HKEY_CURRENT_USER };
constexpr LPCWSTR LOGON_SUBKEY{ L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" };

/* Add the given value with name and data to the key entry (assumed to be open), only if not exists */
LSTATUS AddRegistryEntry(HKEY hKey, LPCWSTR lpValueName, LPCWSTR lpValueData);

/* Add the given value with name and data to the Logon key entry */
LSTATUS AddLogonRegistryEntry(LPCWSTR lpValueName, LPCWSTR lpValueData);
