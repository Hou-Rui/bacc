#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "strutil.hpp"

enum TokenType {
    NORMAL, STRING, EOL, EOF
};

class Token {
private:
    string m_data;
    TokenType m_type;
    int m_line;
public:
    Token() {}
    Token(int line, TokenType type, string data = "");
    string data() const;
    bool is(const string &data) const;
    TokenType type() const;
    int line() const;
};

stringstream &operator<<(stringstream &ss, Token &tok);

bool delim(char c);
string converted_decl(string name);
void tokenize(vector<Token> &vec, string data);

#endif /* TOKEN_HPP */
