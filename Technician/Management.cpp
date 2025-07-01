#include "Management.h"
#include "Registry.h"


ManagementProgram::ManagementProgram() {
    // create mutex and own it to ensure this program runs only one time
    ghMutex = CreateMutex(NULL, TRUE, NULL);
    if (ghMutex == NULL) {
        throw std::runtime_error("Mutex creation failed");
    }
}


ManagementProgram::~ManagementProgram() {
    CloseHandle(ghMutex);
}


void ManagementProgram::setLogonRegistryEntry() {
    addLogonRegistryEntry(REGISTRY_RUN_VALUE_NAME, REGISTRY_RUN_VALUE_PATH);
}


void ManagementProgram::showMessageBox() {
    MessageBox(NULL, PROGRAM_MESSAGE, PROGRAM_TITLE, MB_OK);
}


void ManagementProgram::runProgram() {
    Sleep(HOUR_IN_MILLISECONDS);
}
