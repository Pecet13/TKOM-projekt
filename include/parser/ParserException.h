#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

#include <stdexcept>
#include <string>
#include "lexer/Token.h"

class ParserException : public std::runtime_error
{
private:
    Token token;
    TokenType expected;

public:
    ParserException(std::string message, Token token, TokenType expected)
        : std::runtime_error(message), token(token), expected(expected){}

    virtual const char* what() const noexcept override {
        std::string fullMessage;
        fullMessage = "Error in line " + std::to_string(token.position.getLine()) + ", column " + std::to_string(token.position.getColumn()) 
            + ": " + std::runtime_error::what() + "\nFound: " + std::to_string(token.type) + "\nExpected: " + std::to_string(expected);
        return fullMessage.c_str();
    }
};

#endif