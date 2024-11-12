#ifndef LEXER_H
#define LEXER_H

#include "Position.h"
#include "Token.h"
#include "LexerException.h"
#include <sstream>
#include <unordered_map>
#include <cmath>


class Lexer
{
private:
    std::istream &source;
    Position position;
    Position tokenStartPosition;
    char currentChar;
    Token currentToken;
    size_t maxIDLength = 50;
    size_t maxNumberLength = 20;
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
    Lexer(std::istream &s, size_t maxIDLen=50, size_t maxNumLen=20);
    Token nextToken();
};


#endif