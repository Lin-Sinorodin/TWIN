#include "Registry.h"


HKEY openLogonRegistryEntry() {
    HKEY hKey = 0;
    LSTATUS status = RegOpenKeyEx(LOGON_KEY, LOGON_SUBKEY, 0, KEY_READ | KEY_SET_VALUE, &hKey);
    if (status != ERROR_SUCCESS) {
        throw std::runtime_error("Failed to open Logon registry");
    }
    return hKey;
}


void addRegistryEntry(HKEY hKey, LPCWSTR lpValueName, LPCWSTR lpValueData) {
    DWORD cbDataSize;
    LSTATUS status;
    status = RegGetValueW(hKey, NULL, lpValueName, RRF_RT_REG_SZ, NULL, NULL, NULL);
    switch (status) {
        case ERROR_SUCCESS:
            std::cout << "[+] Found registry value" << std::endl;
            break;
        case ERROR_FILE_NOT_FOUND:
            // couldn't find, add a value with the given name and data to the registry
            cbDataSize = static_cast<DWORD>((wcslen(lpValueData) + 1) * sizeof(WCHAR));
            status = RegSetKeyValueW(hKey, NULL, lpValueName, REG_SZ, lpValueData, cbDataSize);
            if (status != ERROR_SUCCESS) {
                throw std::runtime_error("Failed to set registry value");
            }
            std::cout << "[+] Set registry value" << std::endl;
            break;
        default:
            throw std::runtime_error("Failed to read registry value");
            break;
    }
}


void addLogonRegistryEntry(LPCWSTR lpValueName, LPCWSTR lpValueData) {
    HKEY hKey = openLogonRegistryEntry();
    addRegistryEntry(hKey, lpValueName, lpValueData);
    RegCloseKey(hKey);
}
