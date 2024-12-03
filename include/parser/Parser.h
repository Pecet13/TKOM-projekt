#ifndef PARSER_H
#define PARSER_H

#include "lexer/Lexer.h"
#include "nodes/Node.h"
#include <memory>

class Parser
{
private:
    Lexer &lexer;
    Token currentToken;

    void advance();

    std::unique_ptr<Node> parseDeclaration();
    std::unique_ptr<Node> parseFunctionDeclaration();
    std::unique_ptr<Node> parseVariableDeclaration();
    std::unique_ptr<Node> parseVariantDeclaration();
    std::unique_ptr<Node> parseStructDeclaration();
    std::unique_ptr<Node> parseBlock();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<Node> parseAssignment();
    std::unique_ptr<Node> parseIfStatement();
    std::unique_ptr<Node> parseWhileStatement();
    std::unique_ptr<Node> parseReturnStatement();
    std::unique_ptr<Node> parseMatchStatement();
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseLiteral();
    std::unique_ptr<Node> parseFunctionCall();
    std::unique_ptr<Node> parseArithmeticOperation();
    std::unique_ptr<Node> parseLogicalOperation();
    std::unique_ptr<Node> parseComparison();
    std::unique_ptr<Node> parseParameterList();
    std::unique_ptr<Node> parseParameter();
    std::unique_ptr<Node> parseArgumentList();
    std::unique_ptr<Node> parseStructCreation();
    std::unique_ptr<Node> parseStructFieldList();
    std::unique_ptr<Node> parseStructField();
    std::unique_ptr<Node> parseFieldAccess();
    std::unique_ptr<Node> parseVariant();
    std::unique_ptr<Node> parseMatchCase();

public:
    Parser(Lexer &l);
    std::unique_ptr<Node> parseProgram();
};

#endif