#include "error.hpp"

void Error::print() {
    #define ERR_CODE(x, msg) \
        case x: message = msg; break;
    string message;
    switch (m_errcode) {
        ERR_CODE(0x0, "unexpected end of file")
        ERR_CODE(0x1, "expected , or ;")
        ERR_CODE(0x2, "unexpected ,")
        ERR_CODE(0x3, "unexpected ;")
        ERR_CODE(0x4, "expected expression")
        ERR_CODE(0x5, "END IF without IF")
        ERR_CODE(0x6, "expected end of line")
        ERR_CODE(0x7, "WEND without WHILE")
        ERR_CODE(0x8, "LOOP without DO")
        ERR_CODE(0x9, "expected WHILE or LOOP")
        ERR_CODE(0xa, "invalid expression")
        ERR_CODE(0xb, "ELSE without IF")
        ERR_CODE(0xc, "undeclared variable")
        ERR_CODE(0xd, "unknown function")
        ERR_CODE(0xe, "unexpected end of line")
        ERR_CODE(0xf, "expected =")
        ERR_CODE(0x10, "NEXT without FOR")
        ERR_CODE(0x11, "unmatched NEXT and FOR")
        ERR_CODE(0x12, "unmatched EXIT")
        ERR_CODE(0x13, "expected DO or WHILE or FOR")
        default: message = "unknown error";
    }
    if (m_line != -1)
        cerr << "<line " << m_line << "> ";
    cerr << "ERROR: " << message << " (error code: " << m_errcode << ")" << endl;
    #undef ERR_CODE
}