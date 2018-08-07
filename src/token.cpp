#include "token.hpp"
#include "error.hpp"

Token::Token(int line, TokenType type, string data) {
    if (type != STRING)
        m_data = upper(data);
    else
        m_data = data;
    m_line = line;
    m_type = type;
}
string Token::data() const {
        return m_data;
}

bool Token::is(const string &data) const {
        return m_data == data;
}
TokenType Token::type() const {
    return m_type;
}

int Token::line() const {
    return m_line;
}

stringstream &operator<<(stringstream &ss, Token &tok) {
    if (tok.type() == EOL)
        ss << "<EOL>";
    else if (tok.type() == EOF)
        ss << "<EOF>";
    else if (tok.type() == STRING)
        ss << "\"" << tok.data() << "\"";
    else
        ss << tok.data();
    return ss;
}

const char delims[] = "<>()[]{}+-*/\\^=%,; \t";
bool delim(char c) {
    for (int i = 0; delims[i]; i++)
        if (c == delims[i]) return true;
    return false;
}

void tokenize(vector<Token> &vec, string data) {
    string token = "";
    int line = 1;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == '\n') {
            if (!token.empty())
                vec.push_back(Token(line, NORMAL, token));
            vec.push_back(Token(line++, EOL));
            token = "";
        }
        else if (data[i] == '\"') {
            for (i++; data[i] != '\"'; i++) {
                if (i >= data.size())
                    throw Error(-1, 0x0); // unexpected end of file
                token.push_back(data[i]);
            }
            vec.push_back(Token(line, STRING, token));
            token = "";
        }
        else if (delim(data[i])) {
            if (!token.empty())
                vec.push_back(Token(line, NORMAL, token));
            token = "";
            if (isspace(data[i])) 
                continue;
            for (; !isspace(data[i]) && delim(data[i]); i++) {
                token += data[i];
            }
            i--;
            vec.push_back(Token(line, NORMAL, token));
            token = "";
        }
        else {
            token.push_back(data[i]);
        }
    }
    if (!token.empty())
        vec.push_back(Token(line, NORMAL, token));
    vec.push_back(Token(line, EOL));
    vec.push_back(Token(line, EOF));
}