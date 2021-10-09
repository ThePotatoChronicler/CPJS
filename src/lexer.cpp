#include "lexer.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using token = Lexer::token;

std::unordered_map<std::string, token> keywords = {
		{"dd", token::TOKEN_EOF}
};

/**
 * Returns the next lexeme from data
 * @param data The data to be processed
 * @return true if null was reached, otherwise false
 */
bool Lexer::nextToken(char *data) {
	// skip any spaces but don't skip newlines
	while (data[index] != '\n' && isspace(data[index])) ++index;
	while (true) {
		// if current character is any of the below characters and currentLexeme is empty then put that character
		// in currentLexeme and return. if currentLexeme contains something then just return and process this
		// character at the next time
		if (data[index] == ';' || data[index] == '(' || data[index] == ')' || data[index] == '{'
		|| data[index] == '}' || data[index] == '[' || data[index] == ']') {
			if (currentLexeme.empty()) {
				currentLexeme = std::string(1, data[index]);
				++index;
			}
			return false;
		}
		// if current character isn't any of the above deliminators
		else {
			// if current character is newline then increment currentLine and index and ignore any spaces at the
			// start of the new line
			if (data[index] == '\n') {
				++index;
				++currentLine;
				while (isspace(data[index])) ++index;

				// prevent splitting
				if (!currentLexeme.empty()) {
					return false;
				}
			}
			// if current character is space then increment index and return
			else if (isspace(data[index])) {
				++index;
				return false;
			}
			// if current character is null then return
			else if (data[index] == 0) return true;
			// else append current character to currentLexeme and increment index
			else {
				currentLexeme += data[index];
				++index;
			}
		}
	}
}

std::vector<std::pair<token, std::string>> Lexer::Lex(char *data) {
	std::vector<std::string> lexemes;
	while (!nextToken(data)) {
		lexemes.push_back(currentLexeme);
		currentLexeme.clear();
	}
	std::cout << "successfully ate the file data!" << std::endl;

	for (const auto& lexeme : lexemes) {
		std::cout << "Lexeme " << lexeme << " identified." << std::endl;
	}
	return {};
}
