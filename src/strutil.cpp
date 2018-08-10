#include "general.hpp"

string upper(const string& str) {
    string result;
    for (int i = 0; i < str.size(); i++) {
        result.push_back(toupper(str[i]));
    }
    return result;
}

string trim(const string &str) {
    static auto not_blank = [](char c) { return !isblank(c); };
    auto right = std::find_if(str.rbegin(), str.rend(), not_blank).base();
    auto left = std::find_if(str.begin(), right, not_blank);
    return string(left, right);
}

bool isnumber(const string &str) {
    for (int i = 0; i < str.size(); i++) {
        if (!isnumber(str[i])) return false;
    }
    return true;
}