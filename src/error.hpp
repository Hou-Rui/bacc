#ifndef ERROR_HPP
#define ERROR_HPP

#include "general.hpp"

class Error {
private:
    int m_line, m_errcode;
public:
    Error(int line, int errcode)
        : m_line(line), m_errcode(errcode) {}
    void print();
};

#endif /* ERROR_HPP */
