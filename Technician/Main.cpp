#include "Main.h"
#include "Registry.h"


int main() {
    ManagementProgram prog = ManagementProgram{};
    prog.setLogonRegistryEntry();
    prog.showMessageBox();
    prog.runProgram();
    return EXIT_SUCCESS;
}
