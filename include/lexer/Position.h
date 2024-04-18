#ifndef POSITION_H
#define POSITION_H


class Position
{
private:
    unsigned int line;
    unsigned int column;
public:
    Position();
    Position(unsigned int lin, unsigned int col);

    unsigned int getLine();
    unsigned int getColumn();

    void advance(char currentChar);
};

#endif