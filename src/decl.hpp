#ifndef DECL_HPP
#define DECL_HPP

#include "general.hpp"

set<string> &var_decl();
set<string> &func_decl();
set<string> &array_decl();

void add_decl(set<string> &decl, string name);
void add_array_decl(string name, string size);
bool has_decl(string name);
bool has_decl(const set<string> &decl, string name);

#endif /* DECL_HPP */
