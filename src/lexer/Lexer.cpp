#include "lexer/Lexer.h"
#include "lexer/LexerException.h"

Lexer::Lexer(std::istream &s, size_t maxIDLen, size_t maxNumLen):
source(s), position(1, 0), currentToken(T_UNKNOWN, position), maxIDLength(maxIDLen), maxNumberLength(maxNumLen)
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
        currentToken = Token(T_EOF, tokenStartPosition);
        return true;
    }
    return false;
}

bool Lexer::checkSingleCharToken()
{
    switch (currentChar)
    {
        case '+':
            currentToken = Token(T_PLUS, tokenStartPosition);
            nextChar();
            return true;
        case '-':
            currentToken = Token(T_MINUS, tokenStartPosition);
            nextChar();
            return true;
        case '*':
            currentToken = Token(T_ASTERISK, tokenStartPosition);
            nextChar();
            return true;
        case '/':
            currentToken = Token(T_SLASH, tokenStartPosition);
            nextChar();
            return true;
        case '(':
            currentToken = Token(T_BRACKET_OPEN, tokenStartPosition);
            nextChar();
            return true;
        case ')':
            currentToken = Token(T_BRACKET_CLOSE, tokenStartPosition);
            nextChar();
            return true;
        case '[':
            currentToken = Token(T_SQUARE_OPEN, tokenStartPosition);
            nextChar();
            return true;
        case ']':
            currentToken = Token(T_SQUARE_CLOSE, tokenStartPosition);
            nextChar();
            return true;
        case ',':
            currentToken = Token(T_COMMA, tokenStartPosition);
            nextChar();
            return true;
        case ';':
            currentToken = Token(T_SEMICOLON, tokenStartPosition);
            nextChar();
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
            if (word.length() > maxIDLength)
            {
                throw LexerException("id too long", tokenStartPosition);
            }
        }
        auto it = keywords.find(word);
        if (it != keywords.end())
        {
            currentToken = Token(it->second, tokenStartPosition);
        }
        else
        {
            currentToken = Token(T_ID, tokenStartPosition, word);
        }
        return true;
    }
    return false;
}

bool Lexer::checkNumber()
{
    if (currentChar == '0')
    {
        nextChar();
        if (isdigit(currentChar))
        {
            throw LexerException("number starting with too many zeros", tokenStartPosition);
        }
        if (currentChar != '.')
        {
            currentToken = Token(T_INT_VALUE, tokenStartPosition, 0);
            return true;
        }
    }
    if (isdigit(currentChar))
    {
        int number = currentChar - '0';
        size_t digitCount = 1;
        nextChar();
        while (isdigit(currentChar))
        {
            number = number * 10 + currentChar - '0';
            digitCount++;
            nextChar();
            if (digitCount > maxNumberLength)
            {
                throw LexerException("number too long", tokenStartPosition);
            }
        }
        if (currentChar == '.')
        {
            int exponent = 1;
            nextChar();
            float fraction;
            if (isdigit(currentChar))
            {
                fraction = currentChar - '0';
                nextChar();
                digitCount++;
                while (isdigit(currentChar))
                {
                    fraction = fraction * 10 + currentChar - '0';
                    exponent++;
                    nextChar();
                    if (digitCount > maxNumberLength)
                    {
                        throw LexerException("number too long", tokenStartPosition);
                    }
                }
            }
            fraction = fraction / pow(10, exponent);
            currentToken = Token(T_FLOAT_VALUE, tokenStartPosition, number + fraction);
            return true;
        }
        currentToken = Token(T_INT_VALUE, tokenStartPosition, number);
        return true;
    }
    if (currentChar == '.')
    {
        int exponent = 1;
        nextChar();
        float fraction;
        if (isdigit(currentChar))
        {
            fraction = currentChar - '0';
            nextChar();
            while (isdigit(currentChar))
            {
                fraction = fraction * 10 + currentChar - '0';
                exponent++;
                nextChar();
            }
            fraction = fraction / pow(10, exponent);
            currentToken = Token(T_FLOAT_VALUE, tokenStartPosition, fraction);
            return true;
        }
        currentToken = Token(T_DOT, tokenStartPosition);
        return true;
    }
    return false;
}

bool Lexer::checkString()
{
    if (currentChar == '"')
    {
        std::string text;
        nextChar();
        while (currentChar != '"' && currentChar != EOF)
        {
            text += currentChar;
            nextChar();
        }
        if (currentChar == '"')
        {
            currentToken = Token(T_STRING_VALUE, tokenStartPosition, text);
            nextChar();
            return true;
        }
        else
        {
            throw LexerException("unclosed string", tokenStartPosition);
        }
    }
    return false;
}

bool Lexer::checkComp()
{
    switch (currentChar)
    {
    case '=':
        nextChar();
        if (currentChar == '=')
        {
            currentToken = Token(T_EQUAL, tokenStartPosition);
            nextChar();
            return true;
        }
        currentToken = Token(T_ASSIGN, tokenStartPosition);
        return true;
    case '!':
        nextChar();
        if (currentChar == '=')
        {
            currentToken = Token(T_NOT_EQUAL, tokenStartPosition);
            nextChar();
            return true;
        }
        currentToken = Token(T_NOT, tokenStartPosition);
        return true;
    case '>':
        nextChar();
        if (currentChar == '=')
        {
            currentToken = Token(T_GREATER_EQUAL, tokenStartPosition);
            nextChar();
            return true;
        }
        currentToken = Token(T_GREATER, tokenStartPosition);
        return true;
    case '<':
        nextChar();
        if (currentChar == '=')
        {
            currentToken = Token(T_LESS_EQUAL, tokenStartPosition);
            nextChar();
            return true;
        }
        currentToken = Token(T_LESS, tokenStartPosition);
        return true;
    }
    return false;
}

Token Lexer::nextToken()
{
    skipWhitespaceAndComment();
    tokenStartPosition = position;
    if (checkEOF()
    || checkSingleCharToken()
    || checkKeywordOrId()
    || checkNumber()
    || checkString()
    || checkComp())
    {
        return currentToken;
    }
    currentToken = Token(T_UNKNOWN, position);
    nextChar();
    return currentToken;
}