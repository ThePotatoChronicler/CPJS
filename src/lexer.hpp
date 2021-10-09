#pragma once
#include <vector>
#include <string>

class Lexer {
public:
	enum token {
		TOKEN_EOF,
		TOKEN_FUN,
		TOKEN_IDENTIFIER,
		TOKEN_NUM,
		TOKEN_RETURN,

		TOKEN_LBRACKET, // [
		TOKEN_RBRACKET, // ]

		TOKEN_LBRACE, // {
		TOKEN_RBRACE, // }

		TOKEN_LPAREN, // (
		TOKEN_RPAREN, // )

		TOKEN_SEMICOLON
	};

	std::vector<std::pair<token, std::string>> Lex(char* data);

	Lexer() {
		currentLexeme = "";
		currentLine = 0;
		index = 0;
	}
	Lexer(const Lexer& lexer) = delete;
	Lexer operator=(Lexer lexer) = delete;
	~Lexer() = default;

private:
	std::vector<std::pair<token, std::string>> tokens;
	std::string currentLexeme;
	unsigned int currentLine;
	unsigned int index;
	bool nextToken(char* data);
};
