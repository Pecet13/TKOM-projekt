#ifndef LEXER_EXCEPTION_H
#define LEXER_EXCEPTION_H

#include <stdexcept>
#include <string>
#include "Position.h"

class LexerException : public std::runtime_error
{
private:
    Position position;
    std::string fullMessage;

public:
    LexerException(const std::string& message, const Position& pos)
        : std::runtime_error(message), position(pos)
    {
        fullMessage = "Error in line " + std::to_string(position.getLine()) + ", column " + std::to_string(position.getColumn()) + ": " + message;
    }

    virtual const char* what() const noexcept override
    {
        return fullMessage.c_str();
    }
};


#endif