#include "parser/Parser.h"

const std::unordered_map<TokenType, std::string> tokenTypeToStringMap = 
{
    {T_IF, "if"},
    {T_ELSE, "else"},
    {T_WHILE, "while"},
    {T_RETURN, "return"},
    {T_MATCH, "match"},
    {T_DEFAULT, "default"},
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
    advance();
}

std::string Parser::tokenTypeToString(TokenType type)
{
    auto it = tokenTypeToStringMap.find(type);
    if (it != tokenTypeToStringMap.end())
    {
        return it->second;
    }
    return "unknown";
}

// program		=	{declaration};
std::unique_ptr<ProgramNode> Parser::parseProgram()
{
    std::unique_ptr<ProgramNode> program = std::make_unique<ProgramNode>();
    while (currentToken.type != T_EOF)
    {
        program->addDeclaration((std::move(parseDeclaration())));
    }
    return program;
}

// declaration	=	function_declaration
//				|	variable_declaration
//				|	variant_declaration
//				|	struct_declaration
std::unique_ptr<DeclarationNode> Parser::parseDeclaration()
{
    std::unique_ptr<DeclarationNode> declaration;
    if ((declaration = parseFunctionDeclaration()) ||
        (declaration = parseStructDeclaration()) ||
        (declaration = parseVariableDeclaration()) ||
        (declaration = parseVariantDeclaration()))
    {
        return declaration;
    }
    throw ParserException("unexpected token", currentToken.position, tokenTypeToString(currentToken.type));
}

// function_declaration	=	(type | ‘void’), identifier, ‘(’, parameter_list, ‘)’, block;
std::unique_ptr<FunctionDeclarationNode> Parser::parseFunctionDeclaration()
{
    if (currentToken.type != T_INT && currentToken.type != T_FLOAT && currentToken.type != T_STRING &&
        currentToken.type != T_BOOL && currentToken.type != T_VOID)
    {
        return nullptr;
    }
    std::string type = tokenTypeToString(currentToken.type);
    advance();

    must_be(T_ID, "missing identifier");
    std::string identifier = std::get<std::string>(currentToken.value);

    must_be(T_BRACKET_OPEN, "invalid token");
    std::unique_ptr<ParameterListNode> parameterList = parseParameterList();

    must_be(T_BRACKET_CLOSE, "invalid token");
    std::unique_ptr<BlockNode> block = parseBlock();
    
    std::unique_ptr<FunctionDeclarationNode> functionDeclaration = std::make_unique<FunctionDeclarationNode>(type, identifier, std::move(parameterList), std::move(block));
    return functionDeclaration;
}