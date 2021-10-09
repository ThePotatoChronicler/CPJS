#pragma once
#include "lexer.hpp"
#include <iostream>
using tokens = Lexer::token;

class ExprAST;

class PrototypeAST;
class FunctionAST;
class ReturnAST;

class LiteralAST;

class Parser {
public:
	enum class variableTypes {
		INT
	};

	explicit Parser(Lexer& lexer) : allTokens{lexer.tokens}, currentToken{allTokens[0]}, currentIndex{0}
	{
		auto [_token, _value, _line] = currentToken;
		token = _token;
		value = _value;
		line = _line;
	}
	Parser(const Parser& parser) = delete;
	Parser operator=(Parser parser) = delete;
	~Parser() = default;

	void Parse();

	std::unique_ptr<PrototypeAST> ParsePrototype();
	std::unique_ptr<FunctionAST> ParseFunction();
	std::unique_ptr<ReturnAST> ParseReturn();

	std::unique_ptr<LiteralAST> ParseLiteral();

	std::unique_ptr<ExprAST> ParseExpression();



private:
	tokens token;
	std::string value;
	unsigned int line;

	std::vector<std::tuple<tokens, std::string, unsigned int>> allTokens;
	std::tuple<tokens, std::string, unsigned int> currentToken;
	unsigned int currentIndex = 0;
	void nextToken();
};