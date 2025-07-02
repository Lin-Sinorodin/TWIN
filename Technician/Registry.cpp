#include "Registry.h"


RegistryException::RegistryException(std::string error, LSTATUS code) : m_error(std::move(error)) {
    m_error += " (code = " + std::to_string(code) + ")";
}


const char* RegistryException::what() const noexcept{
    return m_error.c_str();
}


Registry::Registry(HKEY mainKey, LPCWSTR subKey) : m_hMainKey(mainKey), m_lpSubKey(subKey), m_hKey(0) {
    LSTATUS status = RegOpenKeyEx(m_hMainKey, m_lpSubKey, 0, DEFAULT_ACCESS_RIGHTS, &m_hKey);
    if (status != ERROR_SUCCESS) {
        throw RegistryException("Open registry key failed", status);
    }
}


Registry::~Registry() {
    if (m_hKey > 0) {
        RegCloseKey(m_hKey);
    }
}


void Registry::addEntryIfNotExists(LPCWSTR lpValueName, LPCWSTR lpValueData) {
    DWORD cbDataSize;
    LSTATUS status;
    status = RegGetValueW(m_hKey, NULL, lpValueName, RRF_RT_REG_SZ, NULL, NULL, NULL);
    switch (status) {
        case ERROR_SUCCESS:
            std::cout << "[+] Found registry value" << std::endl;
            break;
        case ERROR_FILE_NOT_FOUND:
            // couldn't find, add a value with the given name and data to the registry
            cbDataSize = static_cast<DWORD>((wcslen(lpValueData) + 1) * sizeof(WCHAR));
            status = RegSetKeyValueW(m_hKey, NULL, lpValueName, REG_SZ, lpValueData, cbDataSize);
            if (status != ERROR_SUCCESS) {
                throw RegistryException("Set registry value failed", status);
            }
            std::cout << "[+] Set registry value" << std::endl;
            break;
        default:
            throw RegistryException("Read registry value failed", status);
    }
}
