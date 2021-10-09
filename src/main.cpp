#include <iostream>
#include <filesystem>
#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"

int main() {
	std::string filename = "test/test.cpjs";
	uintmax_t filesize = std::filesystem::file_size(filename);
	if (filesize == 0) {
		std::cerr << "File doesn't exist or is empty." << std::endl;
		exit(-1);
	}
	std::ifstream file {filename};

	char* filecontent = (char*) malloc(filesize);
	file.read(filecontent, (long) filesize);
	file.close();

	Lexer lexer {};
	lexer.Lex(filecontent);
	Parser parser {lexer};
	parser.Parse();

	free(filecontent);
	return 0;
}