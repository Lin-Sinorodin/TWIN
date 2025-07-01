#pragma once

#include "pch.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 1000

// Registry path to the Logon process 
#define LOGON_KEY HKEY_CURRENT_USER
#define LOGON_SUBKEY L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

/* Add the given value with name and data to the key entry (assumed to be open), only if not exists */
LSTATUS AddRegistryEntry(HKEY hKey, LPCWSTR lpValueName, LPCWSTR lpValueData);

/* Add the given value with name and data to the Logon key entry */
LSTATUS AddLogonRegistryEntry(LPCWSTR lpValueName, LPCWSTR lpValueData);
