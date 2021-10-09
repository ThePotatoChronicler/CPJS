#include "lexer.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <regex>

using tokens = Lexer::token;

std::unordered_map<std::string, tokens> keywords = { // NOLINT(cert-err58-cpp)
		{"fun",    tokens::FUN},
		{"return", tokens::RETURN}
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
		|| data[index] == '}' || data[index] == '[' || data[index] == ']' || data[index] == ',') {
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

std::vector<std::pair<tokens, std::string>> Lexer::Lex(char *data) {
	while (!nextToken(data)) {
		tokens.emplace_back(identifyToken(), currentLexeme, currentLine);
		currentLexeme.clear();
	}
	tokens.emplace_back(token::TOKEN_EOF, "", currentLine);
	std::cout << "successfully ate the file data!" << std::endl << std::flush;

	for (const auto& token : tokens) {
		const auto& [_token, string, line] = token;
		if (_token != TOKEN_EOF) {
			std::cout << "Lexeme " << string << " identified." << std::endl << std::flush;
		}
	}
	return {};
}

tokens Lexer::identifyToken() {
	if (keywords.contains(currentLexeme)) {
		return keywords[currentLexeme];
	}
	else if (std::regex_match(currentLexeme, std::regex("^[a-zA-Z]+[\\w]*$"))) {
		return token::IDENTIFIER;
	}
	else if (std::regex_match(currentLexeme, std::regex("^\".*\"")) ||
		std::regex_match(currentLexeme, std::regex("^'.*'$")))
		return token::STRING;
	else if (std::regex_match(currentLexeme, std::regex("^[\\d]+$"))) {
		return token::INTEGER;
	}
	else if (std::regex_match(currentLexeme, std::regex("^[\\d.,]+$"))) {
		return token::DECIMAL;
	}
	else if (std::regex_match(currentLexeme, std::regex("^;$"))) {
		return token::SEMICOLON;
	}


	else if (std::regex_match(currentLexeme, std::regex("^\\{$"))) {
		return token::LCURLY;
	}
	else if (std::regex_match(currentLexeme, std::regex("^\\}$"))) {
		return token::RCURLY;
	}
	else if (std::regex_match(currentLexeme, std::regex("^\\[$"))) {
		return token::LBRACKET;
	}
	else if (std::regex_match(currentLexeme, std::regex("^\\]$"))) {
		return token::RBRACKET;
	}
	else if (std::regex_match(currentLexeme, std::regex("^\\($"))) {
		return token::LPAREN;
	}
	else if (std::regex_match(currentLexeme, std::regex("^\\)$"))) {
		return token::RPAREN;
	}
	else {
		throw std::runtime_error("Invalid identifier " + currentLexeme + " at line " + std::to_string(currentLine + 1));
	}
}
