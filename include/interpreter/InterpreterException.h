#ifndef INTERPRETER_EXCEPTION_H
#define INTERPRETER_EXCEPTION_H

#include <stdexcept>
#include <string>

class InterpreterException : public std::runtime_error
{
private:
    std::string fullMessage;

public:
    InterpreterException(const std::string& message)
        : std::runtime_error(message)
    {
        fullMessage = message;
    }

    virtual const char* what() const noexcept override
    {
        return fullMessage.c_str();
    }
};

#endif