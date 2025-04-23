#pragma once

#include <vector>
#include <string>

#include "token.hpp"


class Lexer {
	private:
	std::string m_sourceFile;
	std::string m_rawData;
	std::vector<Token> m_tokens;

	public:

	// ctors & dtors
	Lexer(const std::string filePath);
	~Lexer();

	// methods
	void clear();

	void readFile();

	bool isIdentifier(const std::string& s);
	bool isIntegerLiteral(const std::string& s);
	bool isDecimalLiteral(const std::string& s);

	bool matchWord(std::string &word);
	void tokenize();

	void printTokens();
};
