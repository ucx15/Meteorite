#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>


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

	while (i < m_rawData.length()) {
		const char c = m_rawData[i];
		i++;

		// std::cout << "\n[ " << c << " ]\t";


		bool matchedWord = false;

		bool matchedWhiteSpace = false;
		bool matchedDelim = false;
		bool matchedStringLiteral = false;
		bool matchedOperator = false;
		bool matchedComment = false;


		// ----------- Matchhing WhiteSpace -----------
		if ( (c == ' ') || (c == '\t') || (c == '\n') ) {
			matchedWord = matchWord(word);
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
			Token delimterToken;

			switch (c) {
				case '(' : {
					matchedWord = matchWord(word);
					delimterToken = Token(TokenType::OPEN_PAREN, "(");
					matchedDelim = true;
					break;
				}

				case ')': {
					matchedWord = matchWord(word);
					delimterToken = Token(TokenType::CLOSE_PAREN, ")");
					matchedDelim = true;
					break;
				}

				case '{': {
					matchedWord = matchWord(word);
					delimterToken = Token(TokenType::OPEN_CURLY, "{");
					matchedDelim = true;
					break;
				}

				case '}': {
					matchedWord = matchWord(word);
					delimterToken = Token(TokenType::CLOSE_CURLY, "}");
					matchedDelim = true;
					break;
				}

				case ':': {
					matchedWord = matchWord(word);
					delimterToken = Token(TokenType::COLON, ":");
					matchedDelim = true;
					break;
				}

				case ';': {
					matchedWord = matchWord(word);
					delimterToken = Token(TokenType::SEMICOLON, ";");
					matchedDelim = true;
					break;
				}

				default:
					matchedDelim = false;
			}

			if ( matchedDelim ) {
				m_tokens.push_back(delimterToken);
			}

		}


		// ----------- Matching Operators -----------
		if ( !matchedDelim && !matchedComment ) {

			// TODO: Support multiwide character operators like <=, >= +=, -= ,etc.
			Token operatorToken;

			switch (c) {

				case '+': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::PLUS, "+");
					matchedOperator = true;
					break;
				}

				case '-': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::MINUS, "-");
					matchedOperator = true;
					break;
				}

				case '*': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::MULTIPLY, "*");
					matchedOperator = true;
					break;
				}

				case '/': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::DIVIDE, "/");
					matchedOperator = true;
					break;
				}

				case '%': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::PERCENT, "%");
					matchedOperator = true;
					break;
				}

				case '&': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::AMPERSAND, "&");
					matchedOperator = true;
					break;
				}

				case '<': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::LESS_THAN, "<");
					matchedOperator = true;
					break;
				}

				case '>': {
					matchedWord = matchWord(word);
					operatorToken = Token(TokenType::GREATER_THAN, ">");
					matchedOperator = true;
					break;
				}

				case '=': {
					matchedWord = matchWord(word);
					matchedOperator = true;

					// == Case - Multiwide Character
					if (m_rawData[i] == '=') {
						i++;
						operatorToken = Token(TokenType::EQUAL_EQUAL, "==");
					}
					else {
						operatorToken = Token(TokenType::EQUAL, "=");
					}
					break;

				}

				case '!': {
					matchedWord = matchWord(word);
					matchedOperator = true;

					// != Case - Multiwide Character
					if (m_rawData[i] == '=') {
						i++;
						operatorToken = Token(TokenType::NOT_EQUAL, "!=");
					}
					else {
						operatorToken = Token(TokenType::NOT, "!");
					}
					break;
				}

				default:
					matchedOperator = false;
			}

			if ( matchedOperator ) {
				m_tokens.push_back(operatorToken);
			}
		}


		// Printers
		(void) matchedWord;
		/*

		if ( matchedWord ) {
			std::cout << '<'<< word << ">  <- matched Word";
		}

		if ( matchedDelim ) {
			std::cout << "  <- matched Delim\n";
		}

		if ( matchedStringLiteral ) {
			std::cout << '"'<< word << "\" <-- matched String Literal \n";
		}

		if ( matchedOperator ) {
			std::cout << "  <- matched Operator\n";
		}

		if ( matchedComment ) {
			std::cout << '<' << word << "> <- Matched Comment\n";
		}


		*/

		// Cleaners
		// if Delimiter or WhiteSpace or Word is matched
		if (matchedDelim || matchedWhiteSpace || matchedStringLiteral || matchedOperator || matchedComment) {
			word.clear();
			continue;
		}


		// Append 'c' to 'word'
		word += c;
		// std::cout << "{" << word << "}";
	}

	// std::cout << std::endl;

	// EOF Token
	m_tokens.push_back(
		Token(TokenType::TOKEN_EOF, "")
	);
}
