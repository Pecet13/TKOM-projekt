#ifndef LEXER_H
#define LEXER_H

#include "Position.h"
#include "Token.h"
#include <sstream>


class Lexer
{
private:
    std::istream &source;
    Position position;
    char currentChar;
    Token currentToken;

    void nextChar();
    void skipWhitespace();

    bool checkEOF();
    bool checkComment();
    bool checkKeywordOrId();
    bool checkNumber();
    bool checkString();
    bool checkMath();
    bool checkRel();
    bool checkBracket();
public:
    Lexer(std::istream &s);
    Token nextToken();
};


#endif