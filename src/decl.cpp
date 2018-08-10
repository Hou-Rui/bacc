#include "decl.hpp"

set<string> m_var_decl;
set<string> m_func_decl;
set<string> m_array_decl;

set<string> &var_decl() {
    return m_var_decl;
}

set<string> &func_decl() {
    return m_func_decl;
}

set<string> &array_decl() {
    return m_array_decl;
}

void add_decl(set<string> &decl, string name) {
    decl.insert(name);
}

void add_array_decl(string name, string size) {
    add_decl(var_decl(), name + "[" + size + "+1]");
    add_decl(array_decl(), name);
}

bool has_decl(string name) {
    return has_decl(var_decl(), name) 
    || has_decl(func_decl(), name)
    || has_decl(array_decl(), name);
}

bool has_decl(const set<string> &decl, string name) {
    return decl.count(name) == 1;
}