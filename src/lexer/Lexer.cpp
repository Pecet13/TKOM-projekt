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

void Lexer::skipWhitespaceAndComment()
{
    while (std::isspace(currentChar) || currentChar == '#')
    {
        if (currentChar == '#')
        {
            while (currentChar != '\n' && source)
            {
                nextChar();
            }
        }
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

bool Lexer::checkSingleCharToken()
{
    switch (currentChar)
    {
        case '+':
            currentToken = Token(T_PLUS, position);
            return true;
        case '-':
            currentToken = Token(T_MINUS, position);
            return true;
        case '*':
            currentToken = Token(T_ASTERISK, position);
            return true;
        case '/':
            currentToken = Token(T_SLASH, position);
            return true;
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
        case ',':
            currentToken = Token(T_COMMA, position);
            return true;
        case ';':
            currentToken = Token(T_SEMICOLON, position);
            return true;
        case '.':
            currentToken = Token(T_DOT, position);
            return true;
    }
    return false;
}

bool Lexer::checkKeywordOrId()
{
    if (isalpha(currentChar))
    {
        std::string word;
        while(isalnum(currentChar) || currentChar == '_')
        {
            word += currentChar;
            nextChar();
        }
        auto it = keywords.find(word);
        if (it != keywords.end())
        {
            currentToken = Token(it->second, position);
        }
        else
        {
            currentToken = Token(T_ID, position, word);
        }
        return true;
    }
    return false;
}

Token Lexer::nextToken()
{
    skipWhitespaceAndComment();
    Position startPosition = position;
    if (checkEOF()
    || checkSingleCharToken()
    || checkKeywordOrId())
    {
        nextChar();
        currentToken.position = startPosition;
        return currentToken;
    }
    currentToken = Token(T_UNKNOWN, position);
    nextChar();
    return currentToken;
}