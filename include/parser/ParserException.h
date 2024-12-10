#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

#include <stdexcept>
#include <string>
#include "lexer/Token.h"

class ParserException : public std::runtime_error
{
private:
    Position position;
    std::string found;
    std::string expected;
    std::string fullMessage;

public:
    ParserException(const std::string& message, const Position& pos, const std::string& found, const std::string& expected = "")
        : std::runtime_error(message), position(pos), found(found), expected(expected)
    {
        fullMessage = "Error in line " + std::to_string(position.getLine()) + ", column " + std::to_string(position.getColumn())
            + ": " + message + "\nFound: " + found;
        if (expected != "")
        {
            fullMessage += "\nExpected: " + expected;
        }
    }

    virtual const char* what() const noexcept override
    {
        return fullMessage.c_str();
    }
};

#endif