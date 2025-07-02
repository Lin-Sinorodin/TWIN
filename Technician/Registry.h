#pragma once

#include <windows.h>
#include <exception>
#include <iostream>
#include <string>

using std::string;
using std::wstring;

// Registry default settings
constexpr REGSAM DEFAULT_ACCESS_RIGHTS{ KEY_READ | KEY_SET_VALUE };

// Registry path to the Logon process 
const HKEY LOGON_KEY{ HKEY_CURRENT_USER };
const wstring LOGON_SUBKEY{ L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" };


/* The exception thrown by the Registry class, appends the error code to the provide message. */
class RegistryException : public std::exception {
public:
	explicit RegistryException(string error, LSTATUS code);
	[[nodiscard]] const char* what() const noexcept override;
private:
	string m_error;
};


class Registry {
public:
	/* Open the registry value with the given mainKey and subKey (read + set value access). Raise if failed. */
	explicit Registry(HKEY mainKey, wstring subKey);

	/* Close the opened registry (only if successfully opened). */
	~Registry();

	/* Add the given value with name and data to the key entry, only if not exists */
	void addEntryIfNotExists(wstring valueName, wstring valueData);
private:
	HKEY m_hKey;         // internal key used by this class
	HKEY m_hMainKey;     // main registry key, see MSDN for RegOpenKeyExW for info
	wstring m_sSubKey;   // sub registry key, optional
};
