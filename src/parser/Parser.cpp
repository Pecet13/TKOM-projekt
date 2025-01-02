#include "parser/Parser.h"

const std::unordered_map<TokenType, std::string> tokenTypeToStringMap = 
{
    {T_IF, "if"},
    {T_ELSE, "else"},
    {T_WHILE, "while"},
    {T_RETURN, "return"},
    {T_MATCH, "match"},
    {T_DEFAULT, "default"},
    {T_NEW, "new"},
    {T_VOID, "void"},
    {T_INT, "int"},
    {T_FLOAT, "float"},
    {T_STRING, "string"},
    {T_BOOL, "bool"},
    {T_STRUCT, "struct"},
    {T_VARIANT, "variant"},
    {T_TRUE, "true"},
    {T_FALSE, "false"},
    {T_NOT, "not"},
    {T_AND, "and"},
    {T_OR, "or"},
    {T_INT_VALUE, "int value"},
    {T_FLOAT_VALUE, "float value"},
    {T_STRING_VALUE, "string value"},
    {T_COMMA, ","},
    {T_SEMICOLON, ";"},
    {T_DOT, "."},
    {T_BRACKET_OPEN, "("},
    {T_BRACKET_CLOSE, ")"},
    {T_SQUARE_OPEN, "["},
    {T_SQUARE_CLOSE, "]"},
    {T_PLUS, "+"},
    {T_MINUS, "-"},
    {T_ASTERISK, "*"},
    {T_SLASH, "/"},
    {T_EQUAL, "=="},
    {T_NOT_EQUAL, "!="},
    {T_GREATER, ">"},
    {T_GREATER_EQUAL, ">="},
    {T_LESS, "<"},
    {T_LESS_EQUAL, "<="},
    {T_MUT, "mut"},
    {T_ASSIGN, "="},
    {T_ID, "identifier"},
    {T_EOF, "end of text"},
    {T_UNKNOWN, "unknown"}
};

std::string tokenTypeToString(TokenType type)
{
    auto it = tokenTypeToStringMap.find(type);
    if (it != tokenTypeToStringMap.end())
    {
        return it->second;
    }
    return "unknown";
}

Parser::Parser(Lexer &lexer):
lexer(lexer), currentToken(Token(T_UNKNOWN, Position(0, 0)))
{
    advance();
}

void Parser::advance()
{
    currentToken = lexer.nextToken();
}

void Parser::must_be(TokenType tokenType, const std::string& message)
{
    if (currentToken.type != tokenType)
    {
        throw ParserException(message, currentToken.position, tokenTypeToString(currentToken.type), tokenTypeToString(tokenType));
    }
}

// program		=	{declaration};
std::unique_ptr<ProgramNode> Parser::parseProgram()
{
    std::unique_ptr<ProgramNode> program = std::make_unique<ProgramNode>();
    while (auto declaration = parseDeclaration())
    {
        program->addDeclaration((std::move(declaration)));
    }
    must_be(T_EOF, "unexpected token");
    return program;
}

// declaration	=	function_declaration
//				|	variable_declaration
//				|	variant_declaration
//				|	struct_declaration
std::unique_ptr<DeclarationNode> Parser::parseDeclaration()
{
    std::unique_ptr<DeclarationNode> declaration;
    if ((declaration = parseFunctionOrVariableDeclaration()) ||
        (declaration = parseVariantDeclaration()) ||
        (declaration = parseStructDeclaration()))
    {
        return declaration;
    }
    return nullptr;
}

// function_declaration	=	(type | ‘void’), identifier, ‘(’, parameter_list, ‘)’, block;
// variable_declaration	=	[‘mut’], type, identifier, [‘=’, expression], ‘;’;
std::unique_ptr<DeclarationNode> Parser::parseFunctionOrVariableDeclaration()
{
    bool isMutable = false;
    if (currentToken.type == T_MUT)
    {
        isMutable = true;
        advance();
    }
    else if (!parseType() && currentToken.type != T_VOID)
    {
        return nullptr;
    }

    std::string type = tokenTypeToString(currentToken.type);
    advance();

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    if (currentToken.type == T_BRACKET_OPEN)
    {
        if (isMutable)
        {
            throw ParserException("\"mut\" is not valid for functions", currentToken.position, tokenTypeToString(currentToken.type));
        }

        advance();
        std::unique_ptr<ParameterListNode> parameterList = parseParameterList();

        must_be(T_BRACKET_CLOSE, "unclosed bracket");
        advance();
        std::unique_ptr<BlockNode> block = parseBlock();
        if (!block)
        {
            throw ParserException("missing block", currentToken.position, tokenTypeToString(currentToken.type));
        }
        
        return std::make_unique<FunctionDeclarationNode>(type, identifier, std::move(parameterList), std::move(block));
    }

    if (type == "void")
    {
        throw ParserException("variable cannot be void", currentToken.position, tokenTypeToString(currentToken.type));
    }

    std::unique_ptr<ExpressionNode> expression;
    if (currentToken.type == T_ASSIGN)
    {
        advance();
        expression = parseExpression();
    }

    must_be(T_SEMICOLON, "missing semicolon");
    advance();
    return std::make_unique<VariableDeclarationNode>(isMutable, type, identifier, std::move(expression));
}

// variant_declaration	=	variant, identifier, ‘;’;
std::unique_ptr<VariantDeclarationNode> Parser::parseVariantDeclaration()
{
    std::unique_ptr<VariantNode> variant = parseVariant();
    if (!variant)
    {
        return nullptr;
    }

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    must_be(T_SEMICOLON, "missing semicolon");
    advance();

    return std::make_unique<VariantDeclarationNode>(std::move(variant), identifier);
}

// struct_declaration	=	‘struct’, identifier, ‘[’, struct_field_list, ‘]’;
std::unique_ptr<StructDeclarationNode> Parser::parseStructDeclaration()
{
    if (currentToken.type != T_STRUCT)
    {
        return nullptr;
    }
    advance();

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    must_be(T_SQUARE_OPEN, "invalid token");
    advance();
    std::unique_ptr<StructFieldListNode> fieldList = parseStructFieldList();

    must_be(T_SQUARE_CLOSE, "invalid token");
    advance();

    return std::make_unique<StructDeclarationNode>(identifier, std::move(fieldList));
}

// block	    =	‘[’, {statement}, ‘]’;
std::unique_ptr<BlockNode> Parser::parseBlock()
{
    if (currentToken.type != T_SQUARE_OPEN)
    {
        return nullptr;
    }
    advance();

    std::unique_ptr<BlockNode> block = std::make_unique<BlockNode>();
    while (auto statement = parseStatement())
    {
        block->addStatement(std::move(statement));
    }

    must_be(T_SQUARE_CLOSE, "unclosed block");
    advance();

    return block;
}

// statement 	=	variable_declaration
//              |	assignment
//              |	if_statement
//              |	while_statement
//              |	return_statement
//              |	variant_declaration
//              |	struct_declaration
//              |	struct_creation
//              |	match_statement;
std::unique_ptr<StatementNode> Parser::parseStatement()
{
    std::unique_ptr<StatementNode> statement;
    if ((statement = parseVariableDeclaration()) ||
        (statement = parseAssignment()) ||
        (statement = parseIfStatement()) ||
        (statement = parseWhileStatement()) ||
        (statement = parseReturnStatement()) ||
        (statement = parseVariantDeclaration()) ||
        (statement = parseStructDeclaration()) ||
        (statement = parseStructCreation()) ||
        (statement = parseMatchStatement()))
    {
        return statement;
    }
    return nullptr;
}

// variable_declaration	=	[‘mut’], type, identifier, [‘=’, expression], ‘;’;
std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration()
{
    bool isMutable = false;
    if (currentToken.type == T_MUT)
    {
        isMutable = true;
        advance();
    }
    else if (currentToken.type != T_INT && currentToken.type != T_FLOAT && currentToken.type != T_STRING &&
        currentToken.type != T_BOOL)
    {
        return nullptr;
    }

    std::string type = tokenTypeToString(currentToken.type);
    advance();

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    std::unique_ptr<ExpressionNode> expression;
    if (currentToken.type == T_ASSIGN)
    {
        advance();
        expression = parseExpression();
    }

    must_be(T_SEMICOLON, "missing semicolon");
    advance();
    return std::make_unique<VariableDeclarationNode>(isMutable, type, identifier, std::move(expression));
}

// assignment		=	field_or_fun_call, [‘=’, expression,] ‘;’
std::unique_ptr<AssignmentNode> Parser::parseAssignment()
{
    std::unique_ptr<FieldOrFunCallNode> fieldOrFunCall = parseFieldOrFunCall();
    if (!fieldOrFunCall)
    {
        return nullptr;
    }

    if (currentToken.type == T_ASSIGN)
    {
        advance();

        std::unique_ptr<ExpressionNode> expression = parseExpression();
        if (!expression)
        {
            throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
        }

        must_be(T_SEMICOLON, "missing semicolon");
        advance();
        return std::make_unique<AssignmentNode>(std::move(fieldOrFunCall), std::move(expression));
    }

    must_be(T_SEMICOLON, "missing semicolon");
    advance();
    return std::make_unique<AssignmentNode>(std::move(fieldOrFunCall));
}

// if_statement		=	‘if’, ‘(’, expression, ‘)’, block, [‘else’, block];
std::unique_ptr<IfStatementNode> Parser::parseIfStatement()
{
    if (currentToken.type != T_IF)
    {
        return nullptr;
    }
    advance();

    must_be(T_BRACKET_OPEN, "missing bracket");
    advance();

    std::unique_ptr<ExpressionNode> expression = parseExpression();
    if (!expression)
    {
        throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
    }

    must_be(T_BRACKET_CLOSE, "unclosed bracket");
    advance();

    std::unique_ptr<BlockNode> block = parseBlock();
    if (!block)
    {
        throw ParserException("missing block", currentToken.position, tokenTypeToString(currentToken.type));
    }

    if (currentToken.type == T_ELSE)
    {
        advance();

        std::unique_ptr<BlockNode> elseBlock = parseBlock();
        if (!elseBlock)
        {
            throw ParserException("missing else block", currentToken.position, tokenTypeToString(currentToken.type));
        }
        return std::make_unique<IfStatementNode>(std::move(expression), std::move(block), std::move(elseBlock));
    }

    return std::make_unique<IfStatementNode>(std::move(expression), std::move(block));
}

// while_statement	=	‘while’, ‘(’, expression, ‘)’, block;
std::unique_ptr<WhileStatementNode> Parser::parseWhileStatement()
{
    if (currentToken.type != T_WHILE)
    {
        return nullptr;
    }
    advance();

    must_be(T_BRACKET_OPEN, "missing bracket");
    advance();

    std::unique_ptr<ExpressionNode> expression = parseExpression();
    if (!expression)
    {
        throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
    }

    must_be(T_BRACKET_CLOSE, "unclosed bracket");
    advance();

    std::unique_ptr<BlockNode> block = parseBlock();
    if (!block)
    {
        throw ParserException("missing block", currentToken.position, tokenTypeToString(currentToken.type));
    }

    return std::make_unique<WhileStatementNode>(std::move(expression), std::move(block));
}

// return_statement	=	‘return’, [expression], ‘;’;
std::unique_ptr<ReturnStatementNode> Parser::parseReturnStatement()
{
    if (currentToken.type != T_RETURN)
    {
        return nullptr;
    }
    advance();

    std::unique_ptr<ExpressionNode> expression = parseExpression();

    must_be(T_SEMICOLON, "missing semicolon");
    advance();
    return std::make_unique<ReturnStatementNode>(std::move(expression));
}

// match_statement	=	‘match’, expression, ‘[’, {match_case},‘]’;
std::unique_ptr<MatchStatementNode> Parser::parseMatchStatement()
{
    if (currentToken.type != T_MATCH)
    {
        return nullptr;
    }
    advance();

    std::unique_ptr<ExpressionNode> expression = parseExpression();
    if (!expression)
    {
        throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
    }

    must_be(T_SQUARE_OPEN, "invalid token");
    advance();

    std::unique_ptr<MatchStatementNode> matchStatement = std::make_unique<MatchStatementNode>(std::move(expression));

    while (auto matchCase = parseMatchCase())
    {
        matchStatement->addCase(std::move(matchCase));
    }

    must_be(T_SQUARE_CLOSE, "invalid token");
    advance();

    return matchStatement;
}

// parameter_list	=	[parameter, {‘,’, parameter}];
std::unique_ptr<ParameterListNode> Parser::parseParameterList()
{
    std::unique_ptr<ParameterListNode> parameterList = std::make_unique<ParameterListNode>();

    if (auto parameter = parseParameter())
    {
        parameterList->addParameter(std::move(parameter));
        
        while (currentToken.type == T_COMMA)
        {
            advance();
            if (auto nextParameter = parseParameter())
            {
                parameterList->addParameter(std::move(nextParameter));
            }
            else
            {
                throw ParserException("missing parameter", currentToken.position, tokenTypeToString(currentToken.type));
            }
        }
    }

    return parameterList;
}

// parameter		=	(type | variant), identifier;
std::unique_ptr<ParameterNode> Parser::parseParameter()
{
    if (parseType())
    {
        std::string type = tokenTypeToString(currentToken.type);
        advance();

        must_be(T_ID, "missing identifier");
        std::string identifier = std::get<std::string>(currentToken.value);
        advance();

        return std::make_unique<ParameterNode>(type, identifier);
    }
    if (auto variant = parseVariant())
    {
        advance();

        must_be(T_ID, "missing identifier");
        std::string identifier = std::get<std::string>(currentToken.value);
        advance();

        return std::make_unique<ParameterNode>(std::move(variant), identifier);
    }

    return nullptr;
}

// expression		=	and_expression, {‘or’, and_expression};
std::unique_ptr<ExpressionNode> Parser::parseExpression()
{
    std::unique_ptr<AndExpressionNode> andExpression = parseAndExpression();
    if (!andExpression)
    {
        return nullptr;
    }

    std::unique_ptr<ExpressionNode> expression = std::make_unique<ExpressionNode>(std::move(andExpression));

    while (currentToken.type == T_OR)
    {
        advance();
        if (auto nextExpression = parseAndExpression())
        {
            expression->addRight(std::move(nextExpression));
        }
        else
        {
            throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
        }
    }

    return expression;
}

// and_expression	=	comparison, {‘and’, comparison}
std::unique_ptr<AndExpressionNode> Parser::parseAndExpression()
{
    std::unique_ptr<ComparisonNode> comparison = parseComparison();
    if (!comparison)
    {
        return nullptr;
    }

    std::unique_ptr<AndExpressionNode> andExpression = std::make_unique<AndExpressionNode>(std::move(comparison));

    while (currentToken.type == T_AND)
    {
        advance();
        if (auto nextComparison = parseComparison())
        {
            andExpression->addRight(std::move(nextComparison));
        }
        else
        {
            throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
        }
    }

    return andExpression;
}

// comparison		=	add_expression, [(‘==’ | ‘!=’ | ‘>’ | ‘<’ | ‘>=’ | ‘<=’), add_expression];
std::unique_ptr<ComparisonNode> Parser::parseComparison()
{
    std::unique_ptr<AddExpressionNode> addExpression = parseAddExpression();
    if (!addExpression)
    {
        return nullptr;
    }

    if (currentToken.type != T_EQUAL && currentToken.type != T_NOT_EQUAL && currentToken.type != T_GREATER &&
        currentToken.type != T_LESS && currentToken.type != T_GREATER_EQUAL && currentToken.type != T_LESS_EQUAL)
    {
        return std::make_unique<ComparisonNode>(std::move(addExpression));
    }

    ComparisonOperator op;
    switch (currentToken.type) 
    {
        case T_EQUAL: 
            op = ComparisonOperator::EQUAL;
            break;
        case T_NOT_EQUAL:
            op = ComparisonOperator::NOT_EQUAL;
            break;
        case T_GREATER:
            op = ComparisonOperator::GREATER;
            break;
        case T_LESS:
            op = ComparisonOperator::LESS;
            break;
        case T_GREATER_EQUAL:
            op = ComparisonOperator::GREATER_EQUAL;
            break;
        case T_LESS_EQUAL:
            op = ComparisonOperator::LESS_EQUAL;
            break;
        default:
            throw ParserException("invalid comparison operator", currentToken.position, tokenTypeToString(currentToken.type));
    }
    advance();

    std::unique_ptr<AddExpressionNode> rightAddExpression = parseAddExpression();
    if (!rightAddExpression)
    {
        throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
    }

    return std::make_unique<ComparisonNode>(std::move(addExpression), op, std::move(rightAddExpression));
}

// add_expression	=	mult_expression, {(‘+’ | ‘-’), mult_expression};
std::unique_ptr<AddExpressionNode> Parser::parseAddExpression()
{
    std::unique_ptr<MultExpressionNode> multExpression = parseMultExpression();
    if (!multExpression)
    {
        return nullptr;
    }

    std::unique_ptr<AddExpressionNode> addExpression = std::make_unique<AddExpressionNode>(std::move(multExpression));

    while (currentToken.type == T_PLUS || currentToken.type == T_MINUS)
    {
        AddOperator op;
        switch (currentToken.type)
        {
            case T_PLUS:
                op = AddOperator::PLUS;
                break;
            case T_MINUS:
                op = AddOperator::MINUS;
                break;
        }
        advance();

        std::unique_ptr<MultExpressionNode> rightMultExpression = parseMultExpression();
        if (!rightMultExpression)
        {
            throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
        }

        addExpression->addRight(op, std::move(rightMultExpression));
    }

    return addExpression;
}

// mult_expression	=	term, {(‘*’, (‘/’), term};
std::unique_ptr<MultExpressionNode> Parser::parseMultExpression()
{
    std::unique_ptr<TermNode> term = parseTerm();
    if (!term)
    {
        return nullptr;
    }

    std::unique_ptr<MultExpressionNode> multExpression = std::make_unique<MultExpressionNode>(std::move(term));

    while (currentToken.type == T_ASTERISK || currentToken.type == T_SLASH)
    {
        MultOperator op;
        switch (currentToken.type)
        {
            case T_ASTERISK:
                op = MultOperator::ASTERISK;
                break;
            case T_MINUS:
                op = MultOperator::SLASH;
                break;
        }
        advance();

        std::unique_ptr<TermNode> rightTerm = parseTerm();
        if (!rightTerm)
        {
            throw ParserException("missing term", currentToken.position, tokenTypeToString(currentToken.type));
        }

        multExpression->addRight(op, std::move(rightTerm));
    }

    return multExpression;
}

// term			=	[(‘!’ | ‘-’)], (field_or_fun_call | ‘(’, expression, ‘)’ | literal);
std::unique_ptr<TermNode> Parser::parseTerm()
{
    NegationType negationType = NegationType::NONE;

    if (currentToken.type == T_NOT)
    {
        negationType = NegationType::LOGICAL;
        advance();
    }

    else if (currentToken.type == T_MINUS)
    {
        negationType = NegationType::ARITHMETICAL;
        advance();
    }

    std::unique_ptr<Node> content;

    if (auto fieldOrFunCall = parseFieldOrFunCall())
    {
        content = std::move(fieldOrFunCall);
    }
    else if (currentToken.type == T_BRACKET_OPEN)
    {
        advance();
        content = parseExpression();
        must_be(T_BRACKET_CLOSE, "unclosed bracket");
        advance();
    }
    else if (auto literal = parseLiteral())
    {
        content = std::move(literal);
    }
    else
    {
        if (negationType != NegationType::NONE)
        {
            throw ParserException("missing term after negation", currentToken.position, tokenTypeToString(currentToken.type));
        }
        return nullptr;
    }

    return std::make_unique<TermNode>(negationType, std::move(content));
}


// field_or_fun_call    =	identifier, [‘(’, [argument_list], ‘)’ | {‘.’, identifier}];
std::unique_ptr<FieldOrFunCallNode> Parser::parseFieldOrFunCall()
{
    if (currentToken.type != T_ID)
    {
        return nullptr;
    }

    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    if (currentToken.type == T_BRACKET_OPEN)
    {
        advance();
        std::unique_ptr<ArgumentListNode> argumentList = parseArgumentList();

        must_be(T_BRACKET_CLOSE, "unclosed bracket");
        advance();

        return std::make_unique<FieldOrFunCallNode>(identifier, std::move(argumentList));
    }
    else if (currentToken.type == T_DOT)
    {
        std::vector<std::string> additionalIdentifiers;
        advance();

        must_be(T_ID, "missing identifier");
        additionalIdentifiers.push_back(std::get<std::string>(currentToken.value));
        advance();

        while (currentToken.type == T_DOT)
        {
            must_be(T_ID, "missing identifier");
            additionalIdentifiers.push_back(std::get<std::string>(currentToken.value));
            advance();
        }

        return std::make_unique<FieldOrFunCallNode>(identifier, additionalIdentifiers);
    }
    return std::make_unique<FieldOrFunCallNode>(identifier);
}

// argument_list		=	[expression, {‘,’, expression}];
std::unique_ptr<ArgumentListNode> Parser::parseArgumentList()
{
    std::unique_ptr<ArgumentListNode> argumentList = std::make_unique<ArgumentListNode>();

    if (auto expression = parseExpression())
    {
        argumentList->addArgument(std::move(expression));
        while (currentToken.type == T_COMMA)
        {
            advance();
            std::unique_ptr<ExpressionNode> anotherExpression = parseExpression();
            if (!anotherExpression)
            {
                throw ParserException("missing expression", currentToken.position, tokenTypeToString(currentToken.type));
            }
            argumentList->addArgument(std::move(anotherExpression));
        }
    }

    return argumentList;
}

// struct_creation		=	‘new’, identifier, identifier, ‘(’,argument_list, ‘)’, ';';
std::unique_ptr<StructCreationNode> Parser::parseStructCreation()
{
    if (currentToken.type != T_NEW)
    {
        return nullptr;
    }
    advance();

    must_be(T_ID, "missing struct type");
    std::string type = std::get<std::string>(currentToken.value);
    advance();

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    must_be(T_BRACKET_OPEN, "missing bracket");
    advance();

    std::unique_ptr<ArgumentListNode> argumentList = parseArgumentList();

    must_be(T_BRACKET_CLOSE, "unclosed bracket");
    advance();

    must_be(T_SEMICOLON, "missing semicolon");
    advance();

    return std::make_unique<StructCreationNode>(type, identifier, std::move(argumentList));
}

// struct_field_list	=	struct_field, {struct_field};
std::unique_ptr<StructFieldListNode> Parser::parseStructFieldList()
{
    std::unique_ptr<StructFieldListNode> structFieldList = std::make_unique<StructFieldListNode>();

    std::unique_ptr<StructFieldNode> structField = parseStructField();
    if (!structField)
    {
        throw ParserException("missing struct field", currentToken.position, tokenTypeToString(currentToken.type));
    }
    structFieldList->addField(std::move(structField));

    while (auto anotherField = parseStructField())
    {
        structFieldList->addField(std::move(anotherField));
    }

    return structFieldList;
}

// struct_field		=	[‘mut’], type, identifier, ‘;’
//                  |	identifier, identifier, ‘;’
//                  |	variant_declaration, ‘;’;
std::unique_ptr<StructFieldNode> Parser::parseStructField()
{
    bool isMutable = false;

    if (currentToken.type == T_MUT)
    {
        isMutable = true;
        advance();
    }

    if (parseType())
    {
        std::string type = tokenTypeToString(currentToken.type);
        advance();

        must_be(T_ID, "missing identifier");
        std::string identifier = std::get<std::string>(currentToken.value);
        advance();

        must_be(T_SEMICOLON, "missing semicolon");
        advance();

        return std::make_unique<StructFieldNode>(type, identifier, isMutable);
    }

    if (currentToken.type == T_ID)
    {
        if (isMutable)
        {
            throw ParserException("\"mut\" is not valid for this type of struct field", currentToken.position, tokenTypeToString(currentToken.type));
        }

        std::string type = std::get<std::string>(currentToken.value);
        advance();

        must_be(T_ID, "missing identifier");
        std::string identifier = std::get<std::string>(currentToken.value);
        advance();

        must_be(T_SEMICOLON, "missing semicolon");
        advance();

        return std::make_unique<StructFieldNode>(type, identifier);
    }

    if (auto variantDeclaration = parseVariantDeclaration())
    {
        if (isMutable)
        {
            throw ParserException("\"mut\" is not valid for this type of struct field", currentToken.position, tokenTypeToString(currentToken.type));
        }

        must_be(T_SEMICOLON, "missing semicolon");
        advance();

        return std::make_unique<StructFieldNode>(std::move(variantDeclaration));
    }

    return nullptr;
}

// variant			=	‘variant’, ‘[’, (type | variant), {type | variant}, ‘]’;
std::unique_ptr<VariantNode> Parser::parseVariant()
{
    if (currentToken.type != T_VARIANT)
    {
        return nullptr;
    }
    advance();

    std::unique_ptr<VariantNode> variant = std::make_unique<VariantNode>();

    must_be(T_SQUARE_OPEN, "invalid token");

    do
    {
        advance();

        if (parseType())
        {
            variant->addType(tokenTypeToString(currentToken.type));
            advance();
        }
        else if (auto nestedVariant = parseVariant())
        {
            variant->addType(std::move(nestedVariant));
        }
        else
        {
            throw ParserException("unexpected or missing type in variant", currentToken.position, tokenTypeToString(currentToken.type));
        }
    } while (currentToken.type == T_COMMA);

    must_be(T_SQUARE_CLOSE, "invalid token");
    advance();

    return variant;
}

// match_case		=	(type | variant), identifier, block
//                  |	‘default’, block;
std::unique_ptr<MatchCaseNode> Parser::parseMatchCase()
{
    if (currentToken.type == T_DEFAULT)
    {
        advance();
        std::unique_ptr<BlockNode> block = parseBlock();
        if (!block)
        {
            throw ParserException("missing block", currentToken.position, tokenTypeToString(currentToken.type));
        }

        return std::make_unique<MatchCaseNode>(std::move(block));
    }
    
    std::variant<std::string, std::unique_ptr<VariantNode>> type;

    if (parseType())
    {
        type = tokenTypeToString(currentToken.type);
        advance();
    }
    else if (auto variant = parseVariant())
    {
        type = std::move(variant);
    }
    else
    {
        return nullptr;
    }

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);
    advance();

    std::unique_ptr<BlockNode> block = parseBlock();
    if (!block)
    {
        throw ParserException("missing block", currentToken.position, tokenTypeToString(currentToken.type));
    }

    if (std::holds_alternative<std::string>(type))
    {
        return std::make_unique<MatchCaseNode>(std::get<std::string>(type), identifier, std::move(block));
    }
    else if (std::holds_alternative<std::unique_ptr<VariantNode>>(type))
    {
        return std::make_unique<MatchCaseNode>(std::move(std::get<std::unique_ptr<VariantNode>>(type)), identifier, std::move(block));
    }
    else
    {
        throw ParserException("incorrect case type", currentToken.position, tokenTypeToString(currentToken.type));
    }
}

// literal			=	bool
//                  |	int
//                  |	float
//                  |	string;
std::unique_ptr<LiteralNode> Parser::parseLiteral()
{
    switch (currentToken.type)
    {
        case T_TRUE:
        {
            advance();
            return std::make_unique<BoolLiteralNode>(true);
        }
        case T_FALSE:
        {
            advance();
            return std::make_unique<BoolLiteralNode>(false);
        }
        case T_INT_VALUE:
        {
            int value = std::get<int>(currentToken.value);
            advance();
            return std::make_unique<IntLiteralNode>(value);
        }
        case T_FLOAT_VALUE:
        {
            float value = std::get<float>(currentToken.value);
            advance();
            return std::make_unique<FloatLiteralNode>(value);
        }
        case T_STRING_VALUE:
        {
            std::string value = std::get<std::string>(currentToken.value);
            advance();
            return std::make_unique<StringLiteralNode>(value);
        }
        default:
        {
            return nullptr;
        }
    }
}

bool Parser::parseType()
{
    return (currentToken.type == T_INT || currentToken.type == T_FLOAT || 
        currentToken.type == T_STRING || currentToken.type == T_BOOL);
}