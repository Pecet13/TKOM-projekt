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
#include <stack>
#include <iostream>
#include <algorithm>
#include <cmath>

struct Variant;
struct StructureInstance;

using Value = std::variant<int, float, std::string, bool, std::shared_ptr<Variant>, std::shared_ptr<StructureInstance>>;

struct Function
{
    std::string type;
    std::vector<std::pair<std::string, std::string>> parameters;
    const BlockNode* block;
};

struct Field
{
    bool isMutable;
    std::string type;
    std::string identifier;
};

struct Structure
{
    std::vector<Field> fields;
};

struct Variant
{
    std::vector<std::string> allowedTypes;
    size_t activeIndex = -1;
    std::vector<Value> values;

    Variant() = default;

    Variant(const std::vector<std::string>& allowedTypes, size_t active_index, const std::vector<Value>& values)
        : allowedTypes(allowedTypes), activeIndex(activeIndex), values(values) {}
};

struct StructureInstance
{
    std::string type;
    std::unordered_map<std::string, Value> values;
};

struct Variable
{
    bool isMutable;
    std::string type;
    Value value;
};

struct Scope
{
    std::unordered_map<std::string, Variable> variables;
    std::unordered_map<std::string, Structure> structs;
    std::unordered_map<std::string, StructureInstance> structInstances;
    std::unordered_map<std::string, std::shared_ptr<Variant>> variants;
    bool typeMatched = false;
};

struct FunctionCallContext
{
    std::string identifier;
    std::vector<Scope> scopes;
};

class Interpreter : public NodeVisitor
{
private:
    size_t recursionDepth = 0;
    const size_t maxRecursionDepth = 1000;
    const size_t maxLoopIterations = 100000;
    std::unordered_map<std::string, Function> functions;
    Scope globalScope;
    std::stack<FunctionCallContext> callStack;
    std::vector<std::string> parameterTypes;
    std::vector<std::string> parameterIdentifiers;
    std::vector<Field> fieldBuffer;
    std::stack<Value> valueStack;
    std::variant<std::monostate, Variable*, Variant*, std::pair<StructureInstance*, std::string>> toAssign;

    Scope& currentScope();
    void enterScope();
    void exitScope();
    void enterCallContext(const std::string& identifier);
    void exitCallContext();
    void checkDuplicateId(const std::string& identifier);
    Value castValueType(const Value& value, const std::string& targetType);
    Function& getFunction(const std::string& identifier);
    Variable& getVariable(const std::string& identifier);
    Structure& getStructure(const std::string& identifier);
    std::shared_ptr<Variant>& getVariant(const std::string& identifier);
    StructureInstance& getStructureInstance(const std::string& identifier);
    std::string buildVariantType(const Variant& variant);
    std::string determineValueType(const Value& value);

public:
    Interpreter(const size_t maxRecursionDepth = 1000, const size_t maxLoopIterations = 100000);

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