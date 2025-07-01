#include "management.h"
#include "registry.h"

#define PROGRAM_MESSAGE L"MANAGEMENT PROGRAM IS UP"
#define PROGRAM_TITLE L"MANAGEMENT PROGRAM"
#define REGISTRY_NAME L"ManagementProgram"
#define REGISTRY_PATH L"C:\\path\\to\\this\\program"

int main() {
    LSTATUS status = AddLogonRegistryEntry(REGISTRY_NAME, REGISTRY_PATH);
    MessageBox(NULL, PROGRAM_MESSAGE, PROGRAM_TITLE, MB_OK);
}