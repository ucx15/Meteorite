#include <iostream>
#include <fstream>

#include "lexer.hpp"


int main(int argc, char* argv[]) {
	// Handling commandline args
	if (argc != 2) {
		std::cerr << "Input file not provided\n";
		std::cerr << "Usage: meteoritecc <Source File>\n";
		return EXIT_FAILURE;
	}

	std::string sourceFile = argv[1];

	// Lexical Analysis
	Lexer lexer(sourceFile);

	if ( !lexer.analyzeSource() ) {
		return EXIT_FAILURE;
	}

	lexer.printTokens();
	lexer.clearSource();

	return EXIT_SUCCESS;
}
