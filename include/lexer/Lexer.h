#ifndef LEXER_H
#define LEXER_H

#include "Position.h"
#include "Token.h"
#include "LexerException.h"
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <climits>
#include <cfloat>


class Lexer
{
private:
    std::istream &source;
    Position position;
    Position tokenStartPosition;
    char currentChar;
    Token currentToken;
    size_t maxIDLength = 50;
    static const std::unordered_map<std::string, TokenType> keywords;

    void nextChar();
    void skipWhitespaceAndComment();

    bool checkEOF();
    bool checkSingleCharToken();
    bool checkKeywordOrId();
    bool checkNumber();
    bool checkString();
    bool checkComp();
public:
    Lexer(std::istream &s, size_t maxIDLen=50);
    Token nextToken();
};


#endif