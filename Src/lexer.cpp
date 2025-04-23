#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

#include "lexer.hpp"
#include "token.hpp"


// ctors & dtors
Lexer::Lexer(const std::string filePath): m_sourceFile(filePath) {}

Lexer::~Lexer() {
	this->clear();

	(void)m_sourceFile;
	(void)m_rawData;
}


// methods
void Lexer::readFile() {
	std::fstream file(m_sourceFile);
	std::ostringstream buffer;

	buffer << file.rdbuf();
	m_rawData = buffer.str();

	file.close();
}

void Lexer::printTokens() {
	for (const Token &t: m_tokens) {
		std::cout << t << '\n';
	}
}


void Lexer::clear() {
	m_sourceFile.clear();
	m_rawData.clear();

}


// TODO: these should be in Token class
bool Lexer::isIdentifier(const std::string& s) {
    if (s.empty()) return false;
    if (!isalpha(s[0]) && s[0] != '_') return false;
    for (char c : s) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

bool Lexer::isIntegerLiteral(const std::string& s) {
    bool digitFound = false;
    for (char c : s) {
        if (isdigit(c)) {
            digitFound = true;
        } else if (c != '\'') {
            return false;
        }
    }
    return digitFound;
}

bool Lexer::isDecimalLiteral(const std::string& s) {
    bool hasDot = false;
    bool digitFound = false;
    size_t i = 0;

    while (i < s.size()) {
        char c = s[i];
        if (isdigit(c)) {
            digitFound = true;
        } else if (c == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (c == '\'') {
            // okay, digit separator
        } else if (c == 'e' || c == 'E') {
            // handle exponent
            ++i;
            if (i < s.size() && (s[i] == '+' || s[i] == '-')) ++i;
            if (i == s.size() || !isdigit(s[i])) return false;
            while (i < s.size() && isdigit(s[i])) ++i;
            return true;
        } else {
            return false;
        }
        ++i;
    }

    return hasDot && digitFound;
}


bool Lexer::matchWord(std::string &word) {
	// Handles identifiers, literals, keywords, types, builtins

	const std::vector<std::string> KEYWORDS = {"return"};

	const std::vector<std::string> TYPES = {
		"i8", "i16", "i32", "i64",
		"u8", "u16", "u32", "u64",
		"f32", "f64"
	};

	const std::vector<std::string> BUILTINS = {"print", "println"};

	// Types
	for (const std::string &type: TYPES) {
		if (word == type) {
			m_tokens.push_back(
				Token(TokenType::TYPE, type)
			);
			return true;
		}
	}

	// Keywords
	for (const std::string &keyword: KEYWORDS) {
		if (word == keyword) {
			m_tokens.push_back(
				Token(TokenType::KEYWORD, keyword)
			);
			return true;
		}
	}

	// Builtins
	for (const std::string &fn: BUILTINS) {
		if (word == fn) {
			m_tokens.push_back(
				Token(TokenType::BUILTIN_FN, fn)
			);
			return true;
		}
	}

	// main fn
	if (word == "main") {
		m_tokens.push_back(
			Token(TokenType::MAIN_FN, "main()")
		);
		return true;
	}

	// identifier and numeric literals

	if ( !word.empty() ) {

		if (isIntegerLiteral(word)) {
			m_tokens.push_back(
				Token(TokenType::INTEGER_LITERAL, word)
			);
			return true;
		}

		else if (isDecimalLiteral(word)) {
			m_tokens.push_back(
				Token(TokenType::DECIMAL_LITERAL, word)
			);
			return true;
		}

		else if (isIdentifier(word)) {
			m_tokens.push_back(
				Token(TokenType::IDENTIFIER, word)
			);
			return true;
		}

		else {
			m_tokens.push_back(
				Token(TokenType::TOKEN_UNKNOWN, word)
			);
		}

	}
	return false;
}

void Lexer::tokenize() {

	size_t i = 0;
	std::string word = "";

	//TODO: move these maps to a token class or somewhere appropriate

	static const std::unordered_map<char, TokenType> delimiterMap = {
		{'(', TokenType::OPEN_PAREN},
		{')', TokenType::CLOSE_PAREN},
		{'{', TokenType::OPEN_CURLY},
		{'}', TokenType::CLOSE_CURLY},
		{':', TokenType::COLON},
		{';', TokenType::SEMICOLON},
	};

	static const std::unordered_map<char, TokenType> operatorMap = {
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


	while (i < m_rawData.length()) {
		const char c = m_rawData[i];
		i++;

		bool matchedWhiteSpace = false;
		bool matchedDelim = false;
		bool matchedStringLiteral = false;
		bool matchedOperator = false;
		bool matchedComment = false;


		// ----------- Matchhing WhiteSpace -----------
		if ( (c == ' ') || (c == '\t') || (c == '\n') ) {
			matchWord(word);
			matchedWhiteSpace = true;
		}


		// ----------- Matching String Literals -----------
		else if ( c == '"' ) {
			// TODO: handle escaped characters

			char strChr = m_rawData[i];
			while (strChr != '"') {
				word += strChr;
				strChr = m_rawData[++i];
			}

			m_tokens.push_back(
				Token(TokenType::STRING_LITERAL, word)
			);
			matchedStringLiteral = true;
			i++;
		}


		// ----------- Matching Comments -----------
		else if ( !matchedStringLiteral && c == '/' ) {
			char cNxt = m_rawData[i];
			if (cNxt == '/') {

				while (cNxt != '\n') {
					word += cNxt;
					cNxt = m_rawData[i++];
				}
				matchedComment = true;
			}
		}


		// ----------- Matching Delimiters -----------
		if ( !matchedComment ) {

			if ( delimiterMap.contains(c) ) {
				matchWord(word);
				m_tokens.push_back(
					Token(delimiterMap.at(c), std::string(1, c))
				);

				matchedDelim = true;
			}
		}


		// ----------- Matching Operators -----------
		if ( !matchedDelim && !matchedComment ) {

			// TODO: Support multiwide character operators like <=, >= +=, -= ,etc.

			if ( operatorMap.contains(c) ) {
				matchWord(word);

				m_tokens.push_back(
					Token(operatorMap.at(c), std::string(1, c))
				);
				matchedOperator = true;
			}
		}


		// Cleaners
		// if Delimiter or WhiteSpace or Word is matched
		if (matchedDelim || matchedWhiteSpace || matchedStringLiteral || matchedOperator || matchedComment) {
			word.clear();
			continue;
		}


		// Append 'c' to 'word'
		word += c;
	}

	// EOF Token
	m_tokens.push_back(
		Token(TokenType::TOKEN_EOF, "")
	);
}
