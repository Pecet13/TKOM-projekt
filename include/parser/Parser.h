#ifndef PARSER_H
#define PARSER_H

#include "lexer/Lexer.h"
#include "ParserException.h"
#include "nodes/Node.h"
#include "nodes/AddExpressionNode.h"
#include "nodes/AndExpressionNode.h"
#include "nodes/ArgumentListNode.h"
#include "nodes/AssignmentNode.h"
#include "nodes/BlockNode.h"
#include "nodes/BoolLiteralNode.h"
#include "nodes/ComparisonNode.h"
#include "nodes/DeclarationNode.h"
#include "nodes/ExpressionNode.h"
#include "nodes/FieldOrFunCallNode.h"
#include "nodes/FloatLiteralNode.h"
#include "nodes/FunctionDeclarationNode.h"
#include "nodes/IfStatementNode.h"
#include "nodes/IntLiteralNode.h"
#include "nodes/LiteralNode.h"
#include "nodes/MatchCaseNode.h"
#include "nodes/MatchStatementNode.h"
#include "nodes/MultExpressionNode.h"
#include "nodes/ParameterListNode.h"
#include "nodes/ParameterNode.h"
#include "nodes/ProgramNode.h"
#include "nodes/ReturnStatementNode.h"
#include "nodes/StatementNode.h"
#include "nodes/StringLiteralNode.h"
#include "nodes/StructCreationNode.h"
#include "nodes/StructDeclarationNode.h"
#include "nodes/StructFieldListNode.h"
#include "nodes/StructFieldNode.h"
#include "nodes/TermNode.h"
#include "nodes/VariableDeclarationNode.h"
#include "nodes/VariantDeclarationNode.h"
#include "nodes/VariantNode.h"
#include "nodes/WhileStatementNode.h"
#include <memory>

class Parser
{
private:
    Lexer &lexer;
    Token currentToken;

    void advance();
    void must_be(TokenType tokenType, const std::string& message);

    std::unique_ptr<DeclarationNode> parseDeclaration();
    std::unique_ptr<DeclarationNode> parseFunctionOrVariableDeclaration();
    std::unique_ptr<VariantDeclarationNode> parseVariantDeclaration();
    std::unique_ptr<StructDeclarationNode> parseStructDeclaration();
    std::unique_ptr<BlockNode> parseBlock();
    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration();
    std::unique_ptr<AssignmentNode> parseAssignment();
    std::unique_ptr<IfStatementNode> parseIfStatement();
    std::unique_ptr<WhileStatementNode> parseWhileStatement();
    std::unique_ptr<ReturnStatementNode> parseReturnStatement();
    std::unique_ptr<MatchStatementNode> parseMatchStatement();
    std::unique_ptr<ParameterListNode> parseParameterList();
    std::unique_ptr<ParameterNode> parseParameter();
    std::unique_ptr<ExpressionNode> parseExpression();
    std::unique_ptr<AndExpressionNode> parseAndExpression();
    std::unique_ptr<ComparisonNode> parseComparison();
    std::unique_ptr<AddExpressionNode> parseAddExpression();
    std::unique_ptr<MultExpressionNode> parseMultExpression();
    std::unique_ptr<TermNode> parseTerm();
    std::unique_ptr<FieldOrFunCallNode> parseFieldOrFunCall();
    std::unique_ptr<ArgumentListNode> parseArgumentList();
    std::unique_ptr<StructCreationNode> parseStructCreation();
    std::unique_ptr<StructFieldListNode> parseStructFieldList();
    std::unique_ptr<StructFieldNode> parseStructField();
    std::unique_ptr<VariantNode> parseVariant();
    std::unique_ptr<MatchCaseNode> parseMatchCase();
    std::unique_ptr<LiteralNode> parseLiteral();
    bool parseType();

public:
    Parser(Lexer &lexer);
    std::unique_ptr<ProgramNode> parseProgram();
};

#endif