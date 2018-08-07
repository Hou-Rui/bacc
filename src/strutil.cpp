#include "general.hpp"

string upper(const string& str) {
    string result;
    for (int i = 0; i < str.size(); i++) {
        result.push_back(toupper(str[i]));
    }
    return result;
}