#include "convert.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "func.hpp"
#include "error.hpp"

stack<StructTag> tag_stack;
stack<string> for_var_stack;

void convert_c_prog_begin(ostream &out) {
    out << "#include <stdio.h>" << endl;
    out << "#include <stdlib.h>" << endl;
    out << "#include <math.h>" << endl;
    out << "int main() {" << endl;
    for (auto &name : var_decl()) {
        out << "double " << name << ";" << endl;
    }
}

void convert_c_prog_end(ostream &out) {
    out << "return 0;" << endl;
    out << "}" << endl;
}

void convert_c_print(ostream &out, vector<Token> &tokens, int &id) {
    bool expect_comma = false;
    for (id++; tokens[id].type() != EOL; id++) {
        //std::cout << to_string(tokens[id]) << endl;
        if (tokens[id].type() == STRING) {
            if (expect_comma) throw Error(tokens[id].line(), 0x1); // expected , or ;
            out << "printf(\"" << tokens[id].data() << "\");" << endl;
            expect_comma = true;
        }
        else if (tokens[id].type() == NORMAL) {
            if (tokens[id].data() == ",") {
                if (!expect_comma) throw Error(tokens[id].line(), 0x2); // unexpected ,
                expect_comma = false;
            }
            else if (tokens[id].data() == ";") {
                if (!expect_comma) throw Error(tokens[id].line(), 0x3); // unexpected ;
                out << "printf(\"\\t\");" << endl;
                expect_comma = false;
            }
            else {
                if (expect_comma) throw Error(tokens[id].line(), 0x1); // expected , or ;
                id--;
                string expr = converted_expr(tokens, id, [](Token &tok) -> bool {
                    return tok.type() == EOL;
                });
                id--;
                out << "if (fabs(floor((" << expr << ")+0.5)-" << expr << ") < 0.00001)" << endl;
                out << "printf(\"%.0f\",floor((" << expr << ")+0.5));" << endl;
                out << "else printf(\"%.4f\"," << expr << ");" << endl;
                expect_comma = true;
            }
        }
    }
    out << "printf(\"\\n\");" << endl;
}

void convert_c_dim(ostream &out, vector<Token> &tokens, int &id) {
    string name = tokens[++id].data();
    if (tokens[++id].type() == EOL) {
        add_decl(var_decl(), name);
    }
    else if (tokens[id].is("(")) {
        string size = tokens[++id].data();
        if (!isnumber(size)) {
            throw Error(tokens[id].line(), 0x15); // expected a number
        }
        if (!tokens[++id].is(")")) {
            throw Error(tokens[id].line(), 0x16); // expected )
        }
        add_array_decl(name, size);
    }
}

void convert_c_input(ostream &out, vector<Token> &tokens, int &id) {
    bool expect_comma = false;
    for (id++; tokens[id].type() != EOL; id++) {
        if (tokens[id].type() == STRING) {
            if (expect_comma) throw Error(tokens[id].line(), 0x1); // expected , or ;
            out << "printf(\"" << tokens[id].data() << "\");" << endl;
            expect_comma = true;
        }
        else if (tokens[id].type() == NORMAL) {
            if (tokens[id].data() == ",") {
                if (!expect_comma) throw Error(tokens[id].line(), 0x2); // unexpected ,
                expect_comma = false;
            }
            else if (tokens[id].data() == ";") {
                if (!expect_comma) throw Error(tokens[id].line(), 0x3); // unexpected ;
                out << "printf(\"\\t\");" << endl;
                expect_comma = false;
            }
            else {
                if (expect_comma) throw Error(tokens[id].line(), 0x1); // expected , or ;
                string name = tokens[id].data();
                if (has_decl(array_decl(), name)) {
                    if (!tokens[++id].is("(")) {
                        throw Error(tokens[id].line(), 0x17); // expected (
                    }
                    string index = converted_expr(tokens, id, [](Token &tok) -> bool {
                        return tok.is(")");
                    });
                    name += string("[") + "((int)(" + index + "+0.5))" + "-1]";
                }
                else if (!has_decl(var_decl(), name)) {
                    add_decl(var_decl(), name);
                }
                out << "scanf(\"%lf\", &" << name << ");" << endl;
                expect_comma = true;
            }
        }
    }
}

void convert_c_if(ostream &out, vector<Token> &tokens, int &id) {
    string expr = converted_expr(tokens, id, [](Token &tok) -> bool {
        return tok.is("THEN");
    });
    out << "if (" << expr << ") {" << endl;
    tag_stack.push(TAG_IF);
}

void convert_c_elseif(ostream &out, vector<Token> &tokens, int &id) {
    string expr = converted_expr(tokens, id, [](Token &tok) -> bool {
        return tok.is("THEN");
    });
    out << "}" << endl << "else if (" << expr << ") {" << endl;
}

void convert_c_else(ostream &out, vector<Token> &tokens, int &id) {
    if (tokens[++id].type() != EOL) {
        throw Error(tokens[id].line(), 0x6); // expected end of line
    }
    if (tag_stack.empty() || tag_stack.top() != TAG_IF) {
        throw Error(tokens[id].line(), 0xb); // ELSE without IF
    }
    out << "}" << endl << "else {" << endl;
}

void convert_c_while(ostream &out, vector<Token> &tokens, int &id) {
    string expr = converted_expr(tokens, id, [](Token &tok) -> bool {
        return tok.type() == EOL;
    });
    out << "while (" << expr << ") {" << endl;
    tag_stack.push(TAG_WHILE);
}

void convert_c_wend(ostream &out, vector<Token> &tokens, int &id) {
    if (tokens[++id].type() != EOL) {
        throw Error(tokens[id].line(), 0x6); // expected end of line
    }
    if (tag_stack.empty() || tag_stack.top() != TAG_WHILE) {
        throw Error(tokens[id].line(), 0x7); // WEND without WHILE
    }
    tag_stack.pop();
    out << "}" << endl;
}

void convert_c_end(ostream &out, vector<Token> &tokens, int &id) {
    id++;
    if (tokens[id].type() == EOL) {
        out << "exit(0);" << endl;
    }
    else if (tokens[id].is("IF")) {
        if (tag_stack.empty() || tag_stack.top() != TAG_IF) {
            throw Error(tokens[id].line(), 0x5); // END IF without IF
        }
        tag_stack.pop();
        out << "}" << endl;
    }
    else {
        throw Error(tokens[id].line(), 0x14); // expected IF or end of line
    }
}

void convert_c_do(ostream &out, vector<Token> &tokens, int &id) {
    id++;
    if (tokens[id].type() != EOL) {
        throw Error(tokens[id].line(), 0x6); // expected end of line
    }
    out << "do {" << endl;
    tag_stack.push(TAG_DO);
}

void convert_c_loop(ostream &out, vector<Token> &tokens, int &id) {
    if (tag_stack.empty() || tag_stack.top() != TAG_DO) {
        throw Error(tokens[id].line(), 0x8); // LOOP without DO 
    }
    string cond = tokens[++id].data();
    if (cond != "WHILE" && cond != "UNTIL") {
        throw Error(tokens[id].line(), 0x9); // expected WHILE or LOOP
    }
    string expr = converted_expr(tokens, id, [](Token &tok) -> bool {
        return tok.type() == EOL;
    });
    if (cond == "WHILE")
        out << "}" << endl << "while (" << expr << ");" << endl;
    else
        out << "}" << endl << "while (!(" << expr << "));" << endl;
    tag_stack.pop();
}

void convert_c_for(ostream &out, vector<Token> &tokens, int &id) {
    if (tokens[++id].type() != NORMAL) {
        throw Error(tokens[id].line(), 0xe); // unexpected end of line
    }
    string for_var = tokens[id].data();
    if (!has_decl(var_decl(), for_var)) {
        add_decl(var_decl(), for_var);
    }
    if (!tokens[++id].is("=")) {
        throw Error(tokens[id].line(), 0xf); // expected =
    }
    string from = converted_expr(tokens, id, [](Token &tok) -> bool {
        return tok.is("TO");
    });
    string to = converted_expr(tokens, id, [](Token &tok) -> bool {
        return tok.type() == EOL || tok.is("STEP");
    });
    string step = "1";
    if (tokens[id].is("STEP")) {
        step = converted_expr(tokens, id, [](Token &tok) -> bool {
            return tok.type() == EOL;
        });
    }
    tag_stack.push(TAG_FOR);
    for_var_stack.push(for_var);
    out << "for (" << for_var << "=" << from << "; ";
    out << for_var << "<" << to << "+0.000001; ";
    out << for_var << "+=" << step << ") {" << endl;
}

void convert_c_next(ostream &out, vector<Token> &tokens, int &id) {
    if (tag_stack.empty() || tag_stack.top() != TAG_FOR) {
        throw Error(tokens[id].line(), 0x10); // NEXT without FOR 
    }
    if (tokens[++id].type() != NORMAL) {
        throw Error(tokens[id].line(), 0xe); // unexpected end of line
    }
    string for_var = tokens[id].data();
    if (for_var != for_var_stack.top()) {
        throw Error(tokens[id].line(), 0x11); // unmatched NEXT and FOR
    }
    for_var_stack.pop();
    tag_stack.pop();
    out << "}" << endl;
}

void convert_c_exit(ostream &out, vector<Token> &tokens, int &id) {
    id++;
    int if_cnt = 0;
    while (!tag_stack.empty() && tag_stack.top() == TAG_IF) {
        if_cnt++;
        tag_stack.pop();
    }
    if (tag_stack.empty()) {
        throw Error(tokens[id].line(), 0x12); // unmatched EXIT
    }
    if (tokens[id].is("DO")) {
        if (tag_stack.top() != TAG_DO) {
            throw Error(tokens[id].line(), 0x12); // unmatched EXIT
        }
        out << "break;" << endl;
    }
    else if (tokens[id].is("WHILE")) {
        if (tag_stack.top() != TAG_WHILE) {
            throw Error(tokens[id].line(), 0x12); // unmatched EXIT
        }
        out << "break;" << endl;
    }
    else if (tokens[id].is("FOR")) {
        if (tag_stack.top() != TAG_FOR) {
            throw Error(tokens[id].line(), 0x12); // unmatched EXIT
        }
        out << "break;" << endl;
    }
    else {
        throw Error(tokens[id].line(), 0x13); // expected DO or WHILE or FOR
    }
    for (int i = 0 ; i < if_cnt; i++)
        tag_stack.push(TAG_IF);
}

void convert_c_let(ostream &out, vector<Token> &tokens, int &id) {
    if (tokens[id].type() != NORMAL 
        || (!tokens[id + 1].is("=") && !tokens[id + 1].is("("))) {
        throw Error(tokens[id].line(), 0x4); // expected expression
    }
    string name = tokens[id].data();
    if (has_decl(array_decl(), name)) {
        if (!tokens[++id].is("(")) {
            throw Error(tokens[id].line(), 0x17); // expected (
        }
        string index = converted_expr(tokens, id, [](Token &tok) -> bool {
            return tok.is(")");
        });
        name += string("[") + "((int)(" + index + "+0.5))" + "-1]";
    }
    else if (!has_decl(var_decl(), name)) {
        add_decl(var_decl(), name);
    }
    string expr = converted_expr(tokens, ++id, [](Token &tok) -> bool {
        return tok.type() == EOL;
    });
    out << name << "=" << expr << ";" << endl;
}

void convert_c(ostream &out, vector<Token> &tokens) {
    stringstream buf;
    for (int id = 0; id < tokens.size(); id++) {
        //std::cout << to_string(tokens[id]) << endl;
        declare_functions();
        if (tokens[id].type() != NORMAL)
            continue;
        else if (tokens[id].is("PRINT")) 
            convert_c_print(buf, tokens, id);
        else if (tokens[id].is("INPUT"))
            convert_c_input(buf, tokens, id);
        else if (tokens[id].is("DIM"))
            convert_c_dim(buf, tokens, id);
        else if (tokens[id].is("IF"))
            convert_c_if(buf, tokens, id);
        else if (tokens[id].is("ELSE"))
            convert_c_else(buf, tokens, id);
        else if (tokens[id].is("ELSEIF"))
            convert_c_elseif(buf, tokens, id);
        else if (tokens[id].is("WHILE"))
            convert_c_while(buf, tokens, id);
        else if (tokens[id].is("WEND"))
            convert_c_wend(buf, tokens, id);
        else if (tokens[id].is("END"))
            convert_c_end(buf, tokens, id);
        else if (tokens[id].is("DO"))
            convert_c_do(buf, tokens, id);
        else if (tokens[id].is("LOOP"))
            convert_c_loop(buf, tokens, id);
        else if (tokens[id].is("FOR"))
            convert_c_for(buf, tokens, id);
        else if (tokens[id].is("NEXT"))
            convert_c_next(buf, tokens, id);
        else if (tokens[id].is("EXIT"))
            convert_c_exit(buf, tokens, id);
        else
            convert_c_let(buf, tokens, id);
    }
    if (!tag_stack.empty()) {
        throw Error(tokens[tokens.size() - 1].line(), 0x0); // unexpected end of file
    }
    convert_c_prog_begin(out);
    out << buf.str();
    convert_c_prog_end(out);
}