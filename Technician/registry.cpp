#include "Registry.h"


LSTATUS AddRegistryEntry(HKEY hKey, LPCWSTR lpValueName, LPCWSTR lpValueData) {
    LSTATUS status;
    status = RegGetValueW(hKey, NULL, lpValueName, RRF_RT_REG_SZ, NULL, NULL, NULL);
    if (status == ERROR_FILE_NOT_FOUND) {
        // couldn't find, add a value with the given name and data to the registry
        DWORD cbDataSize = (wcslen(lpValueData) + 1) * sizeof(WCHAR);
        status = RegSetKeyValueW(hKey, NULL, lpValueName, REG_SZ, lpValueData, cbDataSize);
        if (status != ERROR_SUCCESS) {
            std::cout << "[!] Failed to create registry value, error code: " << status << std::endl;
        }
        else {
            std::cout << "[+] Created registry value" << std::endl;
        }
    }
    else if (status == ERROR_SUCCESS) {
        std::cout << "[+] Found registry value" << std::endl;
    }
    else {
        std::cout << "[-] Failed to read registry value, error code: " << status << std::endl;
    }
    return status;
}


LSTATUS AddLogonRegistryEntry(LPCWSTR lpValueName, LPCWSTR lpValueData) {
    HKEY hKey = 0;
    LSTATUS status;

    // open the Logon registry
    status = RegOpenKeyEx(LOGON_KEY, LOGON_SUBKEY, 0, KEY_READ | KEY_SET_VALUE, &hKey);
    if (status != ERROR_SUCCESS) {
        std::cout << "[!] Failed to open run registry, error code: " << status << std::endl;
        return status;
    }

    status = AddRegistryEntry(hKey, lpValueName, lpValueData);

    RegCloseKey(hKey);
    return status;
}
