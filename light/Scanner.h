#pragma once
#include<vector>
#include<map>
#include"Object.h"

struct Token
{
	enum TokenType
	{
		// Single-character tokens.                      
		LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
		COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

		// One or two character tokens.                  
		BANG, BANG_EQUAL,
		EQUAL, EQUAL_EQUAL,
		GREATER, GREATER_EQUAL,
		LESS, LESS_EQUAL,

		// Literals.                                     
		IDENTIFIER, STRING, NUMBER,

		// Keywords.                                     
		AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
		PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

		END_OF_LINE
	};
	TokenType t;
	unsigned row;
	unsigned col;
	Object value;
	Token(TokenType t, unsigned row, unsigned col, Object value = {})
	:t(t), row(row), col(col), value(value)
	{
	}
};
struct Scanner
{
	std::map<std::string, Token::TokenType> m_keywords
	{
		{ "and",      Token::AND },{ "class",  Token::CLASS },
		{ "else",    Token::ELSE },{ "false",  Token::FALSE },
		{ "for",      Token::FOR },{ "fun",      Token::FUN },
		{ "if",        Token::IF },{ "nil",      Token::NIL },
		{ "or",        Token::OR },{ "print",  Token::PRINT },
		{ "return",Token::RETURN },{ "super",  Token::SUPER },
		{ "this",    Token::THIS },{ "true",    Token::TRUE },
		{ "var",      Token::VAR },{ "while",  Token::WHILE }
	};
	std::string &m_source;
	std::vector<Token> m_tokensVector;
	std::vector<Token>& ScanTokens();
	Scanner(std::string &source):m_source(source)
	{
	}
	void AddToken(Token &token)
	{
		m_tokensVector.push_back(token);
	}
	bool IsDigit(char c)
	{
		return c >= '0' && c <= '9';
	}
	bool IsAlpha(char c) 
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}
	bool IsAlphaNumeric(char c)
	{
		return IsAlpha(c) || IsDigit(c);
	}
};