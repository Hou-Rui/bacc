#include "decl.hpp"
#include "error.hpp"
#include "token.hpp"

map<string, string> basic_to_c;

void declare_functions() {
    std::ifstream fin("res/functions.c");
    string line, basic_func_name, c_func_name;
    basic_func_name.resize(0xff);
    c_func_name.resize(0xff);
    while (std::getline(fin, line)) {
        if (line == "// bacc begin") {
            std::getline(fin, line);
            stringstream ss(line);
            string s1, s2;
            ss >> s1 >> basic_func_name >> s2 >> c_func_name;
            basic_to_c[converted_decl(basic_func_name)] = c_func_name;
            add_decl(func_decl(), converted_decl(basic_func_name));
            ss.str("");
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

