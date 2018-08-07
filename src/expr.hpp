#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"

string converted_expr(vector<Token> &tokens, int &id, 
    std::function<bool(Token&)> should_stop);

#endif /* EXPR_HPP */
