#include <iostream>
#include <fstream>

#include "lexer.hpp"


int main(int argc, char* argv[]) {
	// Handling commandline args
	if (argc != 2) {
		std::cerr << "Usage: MeteoriteVM <Source File>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string sourceFile = argv[1];
	std::cout << "Source file: " << sourceFile << std::endl;


	Lexer lexer(sourceFile);

	lexer.readFile();
	lexer.tokenize();

	lexer.printTokens();


	return EXIT_SUCCESS;
}
