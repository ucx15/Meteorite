#include "token.hpp"


// ctors & dtors
Token::Token() {value = "";}
Token::Token(TokenType t, const std::string &val) : type(t), value(val){}
Token::~Token() {}

std::string Token::toString() const {
	return tokenTypeToString(type) + "  :  [" + value + ']';
}


// overloads
std::string tokenTypeToString(TokenType type) {

	if (TokenTypeMap.contains(type)) {
		return TokenTypeMap.at(type);
	}

	return "token not found";
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << token.toString();
	return os;
}
