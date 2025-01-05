#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H

class AddExpressionNode;
class AndExpressionNode;
class ArgumentListNode;
class AssignmentNode;
class BlockNode;
class BoolLiteralNode;
class ComparisonNode;
class ExpressionNode;
class FieldOrFunCallNode;
class FloatLiteralNode;
class FunctionDeclarationNode;
class IfStatementNode;
class IntLiteralNode;
class MatchCaseNode;
class MatchStatementNode;
class MultExpressionNode;
class ParameterListNode;
class ParameterNode;
class ProgramNode;
class ReturnStatementNode;
class StringLiteralNode;
class StructCreationNode;
class StructDeclarationNode;
class StructFieldListNode;
class StructFieldNode;
class TermNode;
class VariableDeclarationNode;
class VariantDeclarationNode;
class VariantNode;
class WhileStatementNode;

class NodeVisitor
{
public:
    virtual ~NodeVisitor() = default;

    virtual void visit(const ProgramNode& node) = 0;
    virtual void visit(const FunctionDeclarationNode& node) = 0;
    virtual void visit(const VariableDeclarationNode& node) = 0;
    virtual void visit(const VariantDeclarationNode& node) = 0;
    virtual void visit(const StructDeclarationNode& node) = 0;
    virtual void visit(const BlockNode& node) = 0;
    virtual void visit(const AssignmentNode& node) = 0;
    virtual void visit(const IfStatementNode& node) = 0;
    virtual void visit(const WhileStatementNode& node) = 0;
    virtual void visit(const ReturnStatementNode& node) = 0;
    virtual void visit(const MatchStatementNode& node) = 0;
    virtual void visit(const ParameterListNode& node) = 0;
    virtual void visit(const ParameterNode& node) = 0;
    virtual void visit(const ExpressionNode& node) = 0;
    virtual void visit(const AndExpressionNode& node) = 0;
    virtual void visit(const ComparisonNode& node) = 0;
    virtual void visit(const AddExpressionNode& node) = 0;
    virtual void visit(const MultExpressionNode& node) = 0;
    virtual void visit(const TermNode& node) = 0;
    virtual void visit(const FieldOrFunCallNode& node) = 0;
    virtual void visit(const ArgumentListNode& node) = 0;
    virtual void visit(const StructCreationNode& node) = 0;
    virtual void visit(const StructFieldListNode& node) = 0;
    virtual void visit(const StructFieldNode& node) = 0;
    virtual void visit(const VariantNode& node) = 0;
    virtual void visit(const MatchCaseNode& node) = 0;
    virtual void visit(const IntLiteralNode& node) = 0;
    virtual void visit(const FloatLiteralNode& node) = 0;
    virtual void visit(const BoolLiteralNode& node) = 0;
    virtual void visit(const StringLiteralNode& node) = 0;
};
#endif