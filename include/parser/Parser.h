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
    std::unique_ptr<Node> parseParameterList();
    std::unique_ptr<Node> parseParameter();
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseAndExpression();
    std::unique_ptr<Node> parseComparison();
    std::unique_ptr<Node> parseAddExpression();
    std::unique_ptr<Node> parseMultExpression();
    std::unique_ptr<Node> parseTerm();
    std::unique_ptr<Node> parseIdOrFunctionCall();
    std::unique_ptr<Node> parseArgumentList();
    std::unique_ptr<Node> parseStructCreation();
    std::unique_ptr<Node> parseStructFieldList();
    std::unique_ptr<Node> parseStructField();
    std::unique_ptr<Node> parseFieldAccess();
    std::unique_ptr<Node> parseVariant();
    std::unique_ptr<Node> parseMatchCase();
    std::unique_ptr<Node> parseLiteral();

public:
    Parser(Lexer &lexer);
    std::unique_ptr<Node> parseProgram();
};

#endif