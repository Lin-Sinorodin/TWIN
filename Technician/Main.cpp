#include "Management.h"
#include "Registry.h"


int main() {
    try {
        ManagementProgram prog{};
        prog.setLogonRegistryEntry();
        prog.showMessageBox();
        prog.runProgram();
    }
    catch (const std::exception& e) {
        std::cerr << "[!] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
