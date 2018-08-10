#include "token.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "convert.hpp"
#include "error.hpp"

void compile_to_c(const string &prog_name) {
    std::ifstream fin(prog_name + ".bas");
    std::ofstream fout(prog_name + ".c");
    string data = to_string(fin.rdbuf());
    vector<Token> tokens;
    tokenize(tokens, data);
    try {
        convert_c(fout, tokens);
    }
    catch (Error &err) {
        err.print();
        exit(1);
    }
    fin.close(); fout.close();
}

void c_to_exe(const string &prog_name) {
    string cmd = "gcc -O2 -o " + prog_name + " ./" + prog_name + ".c && ./" + prog_name;
    system(cmd.c_str()); 
}