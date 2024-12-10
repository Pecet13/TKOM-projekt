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

public:
    ParserException(std::string message, const Position& pos, const std::string& found, const std::string& expected = "")
        : std::runtime_error(message), position(pos), found(found), expected(expected){}

    virtual const char* what() const noexcept override {
        std::string fullMessage;
        fullMessage = "Error in line " + std::to_string(position.getLine()) + ", column " + std::to_string(position.getColumn())
            + ": " + std::runtime_error::what() + "\nFound: " + found;
        if (expected != "")
        {
            fullMessage += "\nExpected: " + expected;
        }
        return fullMessage.c_str();
    }
};

#endif