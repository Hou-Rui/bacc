#include "decl.hpp"
#include "error.hpp"

#define REGISTER_FUNCTIONS \
    REGISTER_FUNCTION(ABS, fabs) \
    REGISTER_FUNCTION(INT, floor) \
    REGISTER_FUNCTION(SIN, sin) \
    REGISTER_FUNCTION(COS, cos) \
    REGISTER_FUNCTION(TAN, tan) \
    REGISTER_FUNCTION(ATN, atan) \
    REGISTER_FUNCTION(EXP, exp) \
    REGISTER_FUNCTION(LOG, log) \
    REGISTER_FUNCTION(SQR, sqrt) \

void add_function_decl() {
    #define REGISTER_FUNCTION(basic_function, c_function) \
        add_decl(#basic_function);
    REGISTER_FUNCTIONS
    #undef REGISTER_FUNCTION
}

string converted_funcname(int line, string funcname) {
    #define REGISTER_FUNCTION(basic_function, c_function) \
        if (funcname == #basic_function) { \
            return #c_function; \
        }
    REGISTER_FUNCTIONS
    throw Error(line, 0xd); // unknown function
    #undef REGISTER_FUNCTION
}

#undef REGISTER_FUNCTIONS
