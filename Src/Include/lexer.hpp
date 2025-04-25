#pragma once

#include <vector>
#include <string>

#include "token.hpp"


class Lexer {

	private:
	std::string sourceFile_;
	std::string rawData_;
	std::vector<Token> tokens_;

	public:

	// ctors & dtors
	Lexer(const std::string sourceFile);
	~Lexer();


	// Private Methods
	private:

	bool readFile();

	bool isIdentifier(const std::string& s);
	bool isIntegerLiteral(const std::string& s);
	bool isDecimalLiteral(const std::string& s);

	bool matchWord(std::string &word);
	void tokenize();

	// Exposed Methods
	public:
	bool analyzeSource();
	void clearSource();
	void printTokens();
};
