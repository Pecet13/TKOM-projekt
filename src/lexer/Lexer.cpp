#include "lexer/Lexer.h"

Lexer::Lexer(std::istream &s):
source(s), position(1, 0), currentToken(T_UNKNOWN, position)
{
    nextChar();
}

void Lexer::nextChar()
{
    currentChar = static_cast<char>(source.get());
    position.advance(currentChar);
}

void Lexer::skipWhitespace()
{
    while (std::isspace(currentChar) && source)
    {
        nextChar();
    }
}

bool Lexer::checkEOF()
{
    if (currentChar == EOF)
    {
        currentToken = Token(T_EOF, position);
        return true;
    }
    return false;
}

bool Lexer::checkBracket()
{
    switch (currentChar)
    {
        case '(':
            currentToken = Token(T_BRACKET_OPEN, position);
            return true;
        case ')':
            currentToken = Token(T_BRACKET_CLOSE, position);
            return true;
        case '[':
            currentToken = Token(T_SQUARE_OPEN, position);
            return true;
        case ']':
            currentToken = Token(T_SQUARE_CLOSE, position);
            return true;
    }
    return false;
}

Token Lexer::nextToken()
{
    skipWhitespace();
    if (checkEOF() || checkBracket())
    {
        nextChar();
        return currentToken;
    }
    currentToken = Token(T_UNKNOWN, position);
    nextChar();
    return currentToken;
}