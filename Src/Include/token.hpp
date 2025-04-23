#pragma once
#include <string>

enum TokenType {
	KEYWORD,
	BUILTIN_FN,
	MAIN_FN,

	TYPE,

	IDENTIFIER,

	// Punctuation
	OPEN_PAREN,     // (
	CLOSE_PAREN,    // )
	OPEN_CURLY,     // {
	CLOSE_CURLY,    // }
	COMMA,          // ,
	COLON,          // :
	SEMICOLON,      // ;


	// Literals
	STRING_LITERAL,
	INTEGER_LITERAL,
	DECIMAL_LITERAL,

	// Operators
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	PERCENT,
	AMPERSAND,
	NOT,
	GREATER_THAN,
	LESS_THAN,
	EQUAL,
	EQUAL_EQUAL,
	NOT_EQUAL,

	// others
	TOKEN_EOF,
	TOKEN_UNKNOWN,
};


std::string tokenTypeToString(TokenType type);


class Token {
public:
	TokenType type;
	std::string value;

	// ctos & dtors
	Token();
	Token(TokenType t, const std::string &val);
	~Token();

	// methods
	std::string toString() const;
};

// overloads
std::ostream& operator<<(std::ostream& os, const Token& token);
