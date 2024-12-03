#include "parser/Parser.h"

Parser::Parser(Lexer &l):
lexer(l), currentToken(Token(T_UNKNOWN, Position(0, 0)))
{
    advance();
}

void Parser::advance()
{
    currentToken = lexer.nextToken();
}