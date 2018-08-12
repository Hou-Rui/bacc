#include "decl.hpp"

set<string> m_var_decl;
set<string> m_func_decl;
set<string> m_array_decl;
vector<string> m_func_impl;

set<string> &var_decl() {
    return m_var_decl;
}

set<string> &func_decl() {
    return m_func_decl;
}

set<string> &array_decl() {
    return m_array_decl;
}

vector<string> &func_impl() {
    return m_func_impl;
}

void add_func_impl(string impl) {
    m_func_impl.push_back(impl);
}

void add_decl(set<string> &decl, string name) {
    if (decl == func_decl()) {
        //cerr << "declare " << name << endl;
    }
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