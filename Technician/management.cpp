#include "management.h"
#include "registry.h"

#define PROGRAM_MESSAGE L"MANAGEMENT PROGRAM IS UP"
#define PROGRAM_TITLE L"MANAGEMENT PROGRAM"
#define REGISTRY_NAME L"ManagementProgram"
#define REGISTRY_PATH L"C:\\Users\\hp\\Documents\\GitHub\\TWIN\\x64\\Release\\Technician.exe"

HANDLE ghMutex;

int main() {
    // create mutex and own it to ensure this program runs only one time
    ghMutex = CreateMutex(NULL, TRUE, NULL);
    if (ghMutex == NULL) {
        return EXIT_FAILURE;
    }

    // try to add a registry entry to run this file on login
    LSTATUS status = AddLogonRegistryEntry(REGISTRY_NAME, REGISTRY_PATH);
    if (status != ERROR_SUCCESS) {
        return EXIT_FAILURE;
    }

    MessageBox(NULL, PROGRAM_MESSAGE, PROGRAM_TITLE, MB_OK);
    Sleep(1000 * 60 * 60);  // 1 hour = 1000 milliseconds * 60 seconds * 60 minutes
    CloseHandle(ghMutex);

    return EXIT_SUCCESS;
}