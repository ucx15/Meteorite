#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// TODO: better namespacing for grammar objects


// Token Types
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

// TokenType mapping to string
const std::unordered_map<TokenType, std::string> TokenTypeMap = {
	{TokenType::KEYWORD, "keyword"},
	{TokenType::BUILTIN_FN, "builtin function"},
	{TokenType::MAIN_FN, "main function"},

	{TokenType::TYPE, "type"},
	{TokenType::IDENTIFIER, "identifier"},

	// punctuation
	{TokenType::OPEN_PAREN, "open paren"},
	{TokenType::CLOSE_PAREN, "close paren"},
	{TokenType::OPEN_CURLY, "open curly"},
	{TokenType::CLOSE_CURLY, "close curly"},
	{TokenType::COMMA, "comma"},
	{TokenType::COLON, "colon"},
	{TokenType::SEMICOLON, "semicolon"},

	// literals
	{TokenType::STRING_LITERAL, "string literal"},
	{TokenType::INTEGER_LITERAL, "integer literal"},
	{TokenType::DECIMAL_LITERAL, "decimal literal"},

	// operators
	{TokenType::PLUS, "plus"},
	{TokenType::MINUS, "minus"},
	{TokenType::MULTIPLY, "multiply"},
	{TokenType::DIVIDE, "divide"},
	{TokenType::PERCENT, "percent"},
	{TokenType::AMPERSAND, "ampersand"},
	{TokenType::GREATER_THAN, "greater than"},
	{TokenType::LESS_THAN, "less than"},
	{TokenType::EQUAL, "equal"},
	{TokenType::EQUAL_EQUAL, "equal equal"},
	{TokenType::NOT_EQUAL, "not equal"},
	{TokenType::NOT, "not"},

	{TokenType::TOKEN_EOF, "end of file"},
	{TokenType::TOKEN_UNKNOWN, "unknown token"},
};



// Keywords
const std::vector<std::string> KEYWORDS = {
	"return"
};


// Types
const std::vector<std::string> TYPES = {
	"i8", "i16", "i32", "i64",				// signed integers
	"u8", "u16", "u32", "u64",				// unsigned integers
	"f32", "f64"							// floating point numbers
};


// Builtin functions
const std::vector<std::string> BUILTINS = {
	"print",
	"println"
};


// Delimiters
const std::unordered_map<char, TokenType> delimiterMap = {
	{'(', TokenType::OPEN_PAREN},
	{')', TokenType::CLOSE_PAREN},
	{'{', TokenType::OPEN_CURLY},
	{'}', TokenType::CLOSE_CURLY},
	{':', TokenType::COLON},
	{';', TokenType::SEMICOLON},
};


// Operators
const std::unordered_map<char, TokenType> operatorMap = {
	{'+', TokenType::PLUS},
	{'-', TokenType::MINUS},
	{'*', TokenType::MULTIPLY},
	{'/', TokenType::DIVIDE},
	{'%', TokenType::PERCENT},
	{'&', TokenType::AMPERSAND},
	{'<', TokenType::LESS_THAN},
	{'>', TokenType::GREATER_THAN},
	{'=', TokenType::EQUAL},
	{'!', TokenType::NOT},
};
