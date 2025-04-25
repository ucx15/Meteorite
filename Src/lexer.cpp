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
Lexer::Lexer(const std::string sourceFile): sourceFile_(sourceFile) {}

Lexer::~Lexer() {
	clearSource();
}


// Private methods
bool Lexer::readFile() {
	std::cout << "Source file: " << sourceFile_ << "\n\n";

	std::ifstream file(sourceFile_, std::ios::in);
	if ( ! file.is_open() ) {
		std::cerr << "LEXER:  File '" << sourceFile_ << "' not found or cannot be opened.\n";
		return false;
	}

	std::ostringstream buffer;

	buffer << file.rdbuf();
	rawData_ = buffer.str();

	file.close();
	return true;
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

	// Types
	for (const std::string &type: TYPES) {
		if (word == type) {
			tokens_.push_back(
				Token(TokenType::TYPE, type)
			);
			return true;
		}
	}

	// Keywords
	for (const std::string &keyword: KEYWORDS) {
		if (word == keyword) {
			tokens_.push_back(
				Token(TokenType::KEYWORD, keyword)
			);
			return true;
		}
	}

	// Builtins
	for (const std::string &fn: BUILTINS) {
		if (word == fn) {
			tokens_.push_back(
				Token(TokenType::BUILTIN_FN, fn)
			);
			return true;
		}
	}

	// main fn
	if (word == "main") {
		tokens_.push_back(
			Token(TokenType::MAIN_FN, "main()")
		);
		return true;
	}

	// identifier and numeric literals

	if ( !word.empty() ) {

		if (isIntegerLiteral(word)) {
			tokens_.push_back(
				Token(TokenType::INTEGER_LITERAL, word)
			);
			return true;
		}

		else if (isDecimalLiteral(word)) {
			tokens_.push_back(
				Token(TokenType::DECIMAL_LITERAL, word)
			);
			return true;
		}

		else if (isIdentifier(word)) {
			tokens_.push_back(
				Token(TokenType::IDENTIFIER, word)
			);
			return true;
		}

		else {
			tokens_.push_back(
				Token(TokenType::TOKEN_UNKNOWN, word)
			);
		}

	}
	return false;
}

void Lexer::tokenize() {

	size_t i = 0;
	std::string word = "";

	while (i < rawData_.length()) {
		const char c = rawData_[i];
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

			char strChr = rawData_[i];
			while (strChr != '"') {
				word += strChr;
				strChr = rawData_[++i];
			}

			tokens_.push_back(
				Token(TokenType::STRING_LITERAL, word)
			);
			matchedStringLiteral = true;
			i++;
		}


		// ----------- Matching Comments -----------
		else if ( !matchedStringLiteral && c == '/' ) {
			char cNxt = rawData_[i];
			if (cNxt == '/') {

				while (cNxt != '\n') {
					word += cNxt;
					cNxt = rawData_[i++];
				}
				matchedComment = true;
			}
		}


		// ----------- Matching Delimiters -----------
		if ( !matchedComment ) {

			if ( delimiterMap.contains(c) ) {
				matchWord(word);
				tokens_.push_back(
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

				tokens_.push_back(
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
	tokens_.push_back(
		Token(TokenType::TOKEN_EOF, "")
	);
}


// Exposed Method
bool Lexer::analyzeSource() {
	if (!readFile()) {
		std::cerr << "LEXER:  Unable to analyze provided source file.\n";
		return false;
	}

	tokenize();

	return true;
}


void Lexer::printTokens() {
	for (const Token &t: tokens_) {
		std::cout << t << '\n';
	}
}


void Lexer::clearSource() {
	sourceFile_.clear();
	rawData_.clear();

}
