#pragma once
#include <string>
#include <ostream>

#include "grammar.hpp"


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

std::string tokenTypeToString(TokenType type);

// overloads
std::ostream& operator<<(std::ostream& os, const Token& token);
