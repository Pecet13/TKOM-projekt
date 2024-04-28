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
    std::unordered_map<std::string, TokenType> keywords = 
    {
        {"if", T_IF},
        {"else", T_ELSE},
        {"while", T_WHILE},
        {"return", T_RETURN},
        {"match", T_MATCH},
        {"void", T_VOID},
        {"int", T_INT},
        {"float", T_FLOAT},
        {"string", T_STRING},
        {"bool", T_BOOL},
        {"struct", T_STRUCT},
        {"variant", T_VARIANT},
        {"true", T_TRUE},
        {"false", T_FALSE},
        {"and", T_AND},
        {"or", T_OR},
        {"mut", T_MUT}
    };

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