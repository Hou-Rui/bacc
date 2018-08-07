#include "general.hpp"

set<string> m_decl;

const set<string> &decl() {
    return m_decl;
}

void add_decl(string name) {
    m_decl.insert(name);
}

bool has_decl(string name) {
    return m_decl.count(name) == 1;
}