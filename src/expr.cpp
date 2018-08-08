#include "token.hpp"
#include "decl.hpp"
#include "func.hpp"
#include "error.hpp"

inline bool is_operator(string str) {
    return delim(str[0]) || str == "AND" || str == "OR" || str == "MOD";
}

int calc_level(string oper) {
    #define REGISTER_OPERATOR(x, level) if (oper == #x) return level;
    REGISTER_OPERATOR(AND, 10)
    REGISTER_OPERATOR(OR, 10)
    REGISTER_OPERATOR(=, 20)
    REGISTER_OPERATOR(<, 20)
    REGISTER_OPERATOR(>, 20)
    REGISTER_OPERATOR(<=, 20)
    REGISTER_OPERATOR(>=, 20)
    REGISTER_OPERATOR(<>, 20)
    REGISTER_OPERATOR(+, 50)
    REGISTER_OPERATOR(-, 50)
    REGISTER_OPERATOR(*, 60)
    REGISTER_OPERATOR(/, 60)
    REGISTER_OPERATOR(\\, 60)
    REGISTER_OPERATOR(MOD, 60)
    REGISTER_OPERATOR(^, 70)
    #undef REGISTER_OPERATOR
    return 0;
}

string converted_operation(string op, string lhs, string rhs) {
    if (op == "^") return string("pow(") + lhs + "," + rhs + ")";
    else if (op == "\\") return string("floor(") + lhs + "/" + rhs + "+0.00001)";
    else if (op == "MOD") return string("(((int)(") + lhs + "+0.5))%((int)(" + rhs + "+0.5)))";
    else if (op == "AND") op = "&&";
    else if (op == "OR") op = "||";
    else if (op == "=") op = "==";
    else if (op == "<>") op = "!=";
    return string("(") + lhs + op + rhs + ")";
}

string converted_expr(vector<Token> &tokens, int &id, 
    std::function<bool(Token &)> should_stop) {
    stack<string> ovs, ops;
    bool may_call_function = false;
    for (id++; !should_stop(tokens[id - 1]); id++) {
        string data = tokens[id].data();
        if (data == "(") {
            string expr = converted_expr(tokens, id, [](Token &tok) -> bool {
                return tok.is(")");
            });
            if (may_call_function) {
                expr = converted_funcname(tokens[id].line(), ovs.top()) + "(" + expr + ")";
                ovs.pop();
            }
            ovs.push(expr);
            may_call_function = false;
        }
        else if (is_operator(data) || should_stop(tokens[id])) {
            while (!ops.empty() && 
            (should_stop(tokens[id]) || calc_level(data) <= calc_level(ops.top()))) {
                if (ovs.size() < 2) {
                    throw Error(tokens[id].line(), 0xa); // invalid expression
                }
                string rhs = ovs.top(); ovs.pop();
                string lhs = ovs.top(); ovs.pop();
                string op = ops.top(); ops.pop();
                ovs.push(converted_operation(op, lhs, rhs));
                may_call_function = false;
            }
            if (!should_stop(tokens[id])) ops.push(data);
        }
        else {
            if (!isnumber(data[0]) && !has_decl(data)) {
                throw Error(tokens[id].line(), 0xc); // undeclared variable
            }
            ovs.push(data);
            may_call_function = true;
        }
    }
    if (ovs.size() != 1 || !ops.empty()) {
        throw Error(tokens[id].line(), 0xa); // invalid expression
    }
    id--;
    return ovs.top();
}