#include "Management.h"
#include "Registry.h"


MutexException::MutexException(string error, DWORD code) : m_error(std::move(error)) {
    m_error += " (code = " + std::to_string(code) + ")";
}


const char* MutexException::what() const noexcept {
    return m_error.c_str();
}


ManagementProgram::ManagementProgram() {
    m_ghMutex = CreateMutex(NULL, FALSE, PROGRAM_TITLE.c_str());
    if (m_ghMutex == NULL) {
        throw MutexException("Failed to create mutex", GetLastError());
    }
    
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        throw MutexException("Mutex already owned", ERROR_ALREADY_EXISTS);
    }
}


ManagementProgram::~ManagementProgram() {
    ReleaseMutex(m_ghMutex);
    CloseHandle(m_ghMutex);
}


void ManagementProgram::setLogonRegistryEntry() {
    Registry logonRegistry{ LOGON_KEY, LOGON_SUBKEY };
    logonRegistry.addEntryIfNotExists(REGISTRY_RUN_VALUE_NAME, REGISTRY_RUN_VALUE_PATH);
}


void ManagementProgram::showMessageBox() {
    MessageBox(NULL, PROGRAM_MESSAGE.c_str(), PROGRAM_TITLE.c_str(), MB_OK);
}


void ManagementProgram::runProgram() {
    Sleep(HOUR_IN_MILLISECONDS);
}
