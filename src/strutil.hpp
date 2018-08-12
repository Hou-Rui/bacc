#ifndef STRUTIL_HPP
#define STRUTIL_HPP

#include "general.hpp"

string upper(const string &str);
string trim(const string &str);
bool is_number(const string &str);
bool is_alpha(const string &str);
bool start_with(const string &str, const string &prefix);

template <typename Tp> string to_string(Tp x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

#endif /* STRUTIL_HPP */
