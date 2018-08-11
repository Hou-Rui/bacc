#include "decl.hpp"
#include "error.hpp"

map<string, string> basic_to_c;

void declare_functions() {
    std::ifstream fin("res/functions.c");
    string line, basic_func_name, c_func_name;
    while (std::getline(fin, line)) {
        if (line == "// bacc begin") {
            std::getline(fin, line);
            sscanf("%s => %s", basic_func_name.c_str(), c_func_name.c_str());
            basic_to_c[basic_func_name] = c_func_name;
            stringstream ss;
            while (std::getline(fin, line) && line != "// bacc end") {
                if (!line.empty())
                    ss << line << endl;
            }
            add_func_impl(ss.str());
        }
    }
}

string converted_funcname(int line, string funcname) {
    if (basic_to_c.count(funcname) < 1) {
        throw Error(line, 0xd); // unknown function
    }
    return basic_to_c[funcname];
}

