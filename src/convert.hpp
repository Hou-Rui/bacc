#ifndef CONVERT_HPP
#define CONVERT_HPP

#include "token.hpp"

enum StructTag {
    TAG_IF, TAG_WHILE, TAG_DO, TAG_FOR
};

void convert_c_prog_begin(ostream &out);
void convert_c_prog_end(ostream &out);

void convert_c_dim(ostream &out, vector<Token> &tokens, int &id);

void convert_c_print(ostream &out, vector<Token> &tokens, int &id);
void convert_c_input(ostream &out, vector<Token> &tokens, int &id);

void convert_c_if(ostream &out, vector<Token> &tokens, int &id);
void convert_c_else(ostream &out, vector<Token> &tokens, int &id);
void convert_c_elseif(ostream &out, vector<Token> &tokens, int &id);
void convert_c_end(ostream &out, vector<Token> &tokens, int &id);

void convert_c_while(ostream &out, vector<Token> &tokens, int &id);
void convert_c_wend(ostream &out, vector<Token> &tokens, int &id);

void convert_c_do(ostream &out, vector<Token> &tokens, int &id);
void convert_c_loop(ostream &out, vector<Token> &tokens, int &id);

void convert_c_for(ostream &out, vector<Token> &tokens, int &id);
void convert_c_next(ostream &out, vector<Token> &tokens, int &id);

void convert_c_exit(ostream &out, vector<Token> &tokens, int &id);

void convert_c_let(ostream &out, vector<Token> &tokens, int &id);

void convert_c(ostream &out, vector<Token> &tokens);

#endif /* CONVERT_HPP */
