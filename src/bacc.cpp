#include "compile.hpp"

int main(int argc, char const *argv[]) {
    if (argc == 2) {
        const string &prog_name = argv[1];
        compile_to_c(prog_name);
        c_to_exe(prog_name);
    }
    else {
        cerr << "usage: bacc <program-name>" << endl;
    }
    return 0;
}
