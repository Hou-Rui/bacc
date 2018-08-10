#ifndef STRUTIL_HPP
#define STRUTIL_HPP

#include "general.hpp"

string upper(const string &str);
string trim(const string &str);
bool isnumber(const string &str);

template <typename Tp> string to_string(Tp x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

#endif /* STRUTIL_HPP */
