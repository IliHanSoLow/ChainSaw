#include <iostream>
#include <string>
#include <csaw/lang/Parser.hpp>

csaw::Token::Token(const TokenType type, const std::string& value, const size_t line)
    : Type(type), Value(value), Line(line)
{
}

csaw::Token::Token(const TokenType type, const int value, const size_t line)
    : Token(type, std::string(1, static_cast<char>(value)), line)
{
}

csaw::Token::Token(const size_t line)
    : Token(TK_EOF, std::string(), line)
{
}

csaw::Token::Token()
    : Token(TK_EOF, std::string(), 0)
{
}

uint32_t csaw::Token::IntValue() const
{
    switch (Type)
    {
    case TK_INT_BIN: return std::stoi(Value, nullptr, 2);
    case TK_INT_OCT: return std::stoi(Value, nullptr, 8);
    case TK_INT_DEC: return std::stoi(Value, nullptr, 10);
    case TK_INT_HEX: return std::stoi(Value, nullptr, 16);
    default: return 0;
    }
}

const char* csaw::ToString(const int type)
{
    switch (type)
    {
    case TK_EOF: return "EOF";
    case TK_IDENTIFIER: return "IDENTIFIER";
    case TK_INT_DEC: return "DEC";
    case TK_INT_HEX: return "HEX";
    case TK_INT_BIN: return "BIN";
    case TK_STRING: return "STRING";
    case TK_CHAR: return "CHAR";
    case TK_OPERATOR: return "OPERATOR";
    case TK_COMPILE_DIRECTIVE: return "COMP DIR";
    default: return "UNDEFINED";
    }
}

std::ostream& csaw::operator<<(std::ostream& out, const TokenType& type)
{
    return out << ToString(type);
}

std::ostream& csaw::operator<<(std::ostream& out, const Token& token)
{
    return out << token.Value << "[" << token.Type << "](" << token.Line << ")";
}