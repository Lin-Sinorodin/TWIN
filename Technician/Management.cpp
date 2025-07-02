#include "Management.h"
#include "Registry.h"


ManagementProgram::ManagementProgram() {
    m_ghMutex = CreateMutex(NULL, FALSE, PROGRAM_TITLE);
    if (m_ghMutex == NULL) {
        throw std::runtime_error("Failed to create mutex");
    }
    
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        throw std::runtime_error("Mutex already owned");
    }

    // initialize the server
    server = Server(DEFAULT_PORT);
}


ManagementProgram::~ManagementProgram() {
    CloseHandle(m_ghMutex);
}


void ManagementProgram::setLogonRegistryEntry() {
    Registry logonRegistry{ LOGON_KEY, LOGON_SUBKEY };
    logonRegistry.addEntryIfNotExists(REGISTRY_RUN_VALUE_NAME, REGISTRY_RUN_VALUE_PATH);
}


void ManagementProgram::showMessageBox() {
    MessageBox(NULL, PROGRAM_MESSAGE, PROGRAM_TITLE, MB_OK);
}


void ManagementProgram::runProgram() {
    server.startListening();
    server.acceptClient();
    server.handleClient();
    server.disconnectClient();
}
