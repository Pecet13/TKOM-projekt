#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "NodeVisitor.h"
#include "InterpreterException.h"
#include "../parser/nodes/AddExpressionNode.h"
#include "../parser/nodes/AndExpressionNode.h"
#include "../parser/nodes/ArgumentListNode.h"
#include "../parser/nodes/AssignmentNode.h"
#include "../parser/nodes/BlockNode.h"
#include "../parser/nodes/BoolLiteralNode.h"
#include "../parser/nodes/ComparisonNode.h"
#include "../parser/nodes/ExpressionNode.h"
#include "../parser/nodes/FieldOrFunCallNode.h"
#include "../parser/nodes/FloatLiteralNode.h"
#include "../parser/nodes/FunctionDeclarationNode.h"
#include "../parser/nodes/IfStatementNode.h"
#include "../parser/nodes/IntLiteralNode.h"
#include "../parser/nodes/MatchCaseNode.h"
#include "../parser/nodes/MatchStatementNode.h"
#include "../parser/nodes/MultExpressionNode.h"
#include "../parser/nodes/ParameterListNode.h"
#include "../parser/nodes/ParameterNode.h"
#include "../parser/nodes/ProgramNode.h"
#include "../parser/nodes/ReturnStatementNode.h"
#include "../parser/nodes/StringLiteralNode.h"
#include "../parser/nodes/StructCreationNode.h"
#include "../parser/nodes/StructDeclarationNode.h"
#include "../parser/nodes/StructFieldListNode.h"
#include "../parser/nodes/StructFieldNode.h"
#include "../parser/nodes/TermNode.h"
#include "../parser/nodes/VariableDeclarationNode.h"
#include "../parser/nodes/VariantDeclarationNode.h"
#include "../parser/nodes/VariantNode.h"
#include "../parser/nodes/WhileStatementNode.h"
#include <unordered_map>

class Interpreter : public NodeVisitor
{
private:
    std::unordered_map<std::string, const FunctionDeclarationNode*> functions;
    std::unordered_map<std::string, std::variant<int, float, std::string, bool>> variables;
    std::unordered_map<std::string, std::unordered_map<std::string, std::variant<int, float, std::string, bool>>> localVariables;

public:
    void visit(const ProgramNode& node) override;
    void visit(const FunctionDeclarationNode& node) override;
    void visit(const VariableDeclarationNode& node) override;
    void visit(const VariantDeclarationNode& node) override;
    void visit(const StructDeclarationNode& node) override;
    void visit(const BlockNode& node) override;
    void visit(const AssignmentNode& node) override;
    void visit(const IfStatementNode& node) override;
    void visit(const WhileStatementNode& node) override;
    void visit(const ReturnStatementNode& node) override;
    void visit(const MatchStatementNode& node) override;
    void visit(const ParameterListNode& node) override;
    void visit(const ParameterNode& node) override;
    void visit(const ExpressionNode& node) override;
    void visit(const AndExpressionNode& node) override;
    void visit(const ComparisonNode& node) override;
    void visit(const AddExpressionNode& node) override;
    void visit(const MultExpressionNode& node) override;
    void visit(const TermNode& node) override;
    void visit(const FieldOrFunCallNode& node) override;
    void visit(const ArgumentListNode& node) override;
    void visit(const StructCreationNode& node) override;
    void visit(const StructFieldListNode& node) override;
    void visit(const StructFieldNode& node) override;
    void visit(const VariantNode& node) override;
    void visit(const MatchCaseNode& node) override;
    void visit(const IntLiteralNode& node) override;
    void visit(const FloatLiteralNode& node) override;
    void visit(const BoolLiteralNode& node) override;
    void visit(const StringLiteralNode& node) override;
};

#endif