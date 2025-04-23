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
	switch (type) {
		case KEYWORD : return "keyword";
		case BUILTIN_FN : return "builtin function";
		case MAIN_FN : return "main function";

		case TYPE : return "type";
		case IDENTIFIER : return "identifier";

		// punctuation
		case OPEN_PAREN : return "open paren";
		case CLOSE_PAREN : return "close paren";
		case OPEN_CURLY : return "open curly";
		case CLOSE_CURLY : return "close curly";
		case COMMA : return "comma";
		case COLON : return "colon";
		case SEMICOLON : return "semicolon";

		// literals
		case STRING_LITERAL : return "string literal";
		case INTEGER_LITERAL : return "integer literal";
		case DECIMAL_LITERAL : return "decimal literal";

		// operators
		case PLUS: return "plus";
		case MINUS: return "minus";
		case MULTIPLY: return "multiply";
		case DIVIDE: return "divide";
		case PERCENT: return "percent";
		case AMPERSAND: return "ampersand";
		case GREATER_THAN: return "greater than";
		case LESS_THAN: return "less than";
		case EQUAL: return "equal";
		case EQUAL_EQUAL: return "equal equal";
		case NOT_EQUAL: return "not equal";
		case NOT: return "not";

		case TOKEN_EOF : return "end of file";
		case TOKEN_UNKNOWN : return "unknown token";
		default : return "token not found";
	}
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << token.toString();
	return os;
}
