#ifndef LEXER_EXCEPTION_H
#define LEXER_EXCEPTION_H

#include <stdexcept>
#include <string>
#include "Position.h"

class LexerException : public std::runtime_error
{
private:
    Position position;

public:
    LexerException(std::string message, Position position)
        : std::runtime_error(message), position(position) {}

    virtual const char* what() const noexcept override {
        std::string fullMessage;
        fullMessage = "Error in line " + std::to_string(position.getLine()) + ", column " + std::to_string(position.getColumn()) + ": " + std::runtime_error::what();
        return fullMessage.c_str();
    }
};


#endif