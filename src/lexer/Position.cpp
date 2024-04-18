#include "lexer/Position.h"

Position::Position()
{}

Position::Position(unsigned int lin, unsigned int col):
line(lin), column(col) {}

unsigned int Position::getLine()
{
    return line;
}

unsigned int Position::getColumn()
{
    return column;
}

void Position::advance(char currentChar)
{
    if (currentChar == '\n') 
    {
        line++;
        column = 0;
    }
    else 
    {
        column++;
    }
}
