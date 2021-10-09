#pragma once
#include <vector>
#include <string>

class Lexer {
public:
	enum token {
		FUN,
		RETURN,

		IDENTIFIER,

		INTEGER,
		DECIMAL,
		STRING,

		LBRACKET,
		RBRACKET,

		LCURLY,
		RCURLY,

		LPAREN,
		RPAREN,

		SEMICOLON,
		COMMA,

		TOKEN_EOF
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

	std::vector<std::tuple<token, std::string, unsigned int>> tokens;
private:
	std::string currentLexeme;
	unsigned int currentLine;
	unsigned int index;
	bool nextToken(char* data);
	token identifyToken();
};
