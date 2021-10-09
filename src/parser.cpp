#include "parser.hpp"

#include <utility>
#include <iostream>

using variabletypes = Parser::variableTypes;

void Parser::nextToken() {
	currentToken = allTokens[++currentIndex];
	auto [_token, _value, _line] = currentToken;
	token = _token;
	value = _value;
	line = _line;
}

class IdentifierAST;
class PrototypeAST;
class FunctionAST;
class ReturnAST;
class LiteralAST;

class AbstractDispatcher {
public:
	virtual void Dispatch(IdentifierAST& identifier) = 0;
	virtual void Dispatch(PrototypeAST& prototype) = 0;
	virtual void Dispatch(FunctionAST& function) = 0;
	virtual void Dispatch(ReturnAST& return_) = 0;
	virtual void Dispatch(LiteralAST& literal) = 0;
};
class ExprAST {
public:
	virtual ~ExprAST() = default;
	
	virtual void Dispatch(AbstractDispatcher& dispatcher) = 0;
};

class IdentifierAST : public ExprAST {
	std::string name;

public:
	explicit IdentifierAST(std::string name) : name{std::move(name)} {}

	void Dispatch(AbstractDispatcher& dispatcher) override {
		dispatcher.Dispatch(*this);
	}
};

class PrototypeAST : public ExprAST {
	std::string name;
	std::unique_ptr<std::vector<IdentifierAST>> args;

public:
	PrototypeAST(std::string name, std::unique_ptr<std::vector<IdentifierAST>> args) :
	name{std::move(name)}, args{std::move(args)} {}

	void Dispatch(AbstractDispatcher& dispatcher) override {
		dispatcher.Dispatch(*this);
	}
};

class FunctionAST : public ExprAST {
	std::unique_ptr<PrototypeAST> prototype;
	std::vector<std::unique_ptr<ExprAST>> body;

public:
	FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::vector<std::unique_ptr<ExprAST>> body) :
	prototype{std::move(prototype)}, body{std::move(body)} {}

	void Dispatch(AbstractDispatcher& dispatcher) override {
		dispatcher.Dispatch(*this);
	}
};

class ReturnAST : public ExprAST {
	std::unique_ptr<ExprAST> expression;
public:
	explicit ReturnAST(std::unique_ptr<ExprAST> expression) : expression{std::move(expression)} {}

	void Dispatch(AbstractDispatcher& dispatcher) override {
		dispatcher.Dispatch(*this);
	}
};

class LiteralAST : public ExprAST {
	std::string literal;
	variabletypes type;
public:
	explicit LiteralAST(std::string literal, variabletypes type) : literal{std::move(literal)}, type{type} {}

	void Dispatch(AbstractDispatcher& dispatcher) override {
		dispatcher.Dispatch(*this);
	}
};

std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
	nextToken();
	if (token != tokens::IDENTIFIER)
		throw std::runtime_error("IDIOT WHY DID YOU PUT SOMETHING ELSE THAN IDENTIFIER AFTER FUNCTION??");
	std::string name = value;

	nextToken();
	if (token != tokens::LPAREN)
		throw std::runtime_error("expected ( after " + name);
	nextToken();

	std::vector<IdentifierAST> args;
	if (token == tokens::IDENTIFIER) {
		while (true) {
			if (token != tokens::IDENTIFIER && token != tokens::COMMA )
				break;
			args.emplace_back(value);
			nextToken();
		}
	}
	if (token == tokens::RPAREN) {
		nextToken();
		return std::make_unique<PrototypeAST>(name, std::make_unique<std::vector<IdentifierAST>>(args));
	}
	else {
		throw std::runtime_error("Excepted ) at prototype, but got " + value);
	}
}

std::unique_ptr<FunctionAST> Parser::ParseFunction() {
	auto prototype = ParsePrototype();
	std::vector<std::unique_ptr<ExprAST>> body;
	if (token == tokens::LCURLY) {
		nextToken();
		while (token != tokens::RCURLY) {
			if (token == tokens::TOKEN_EOF)
				throw std::runtime_error("Excepted } but got EOF instead");
			body.emplace_back(ParseExpression());
			nextToken();
		}
		nextToken();
	}

	return std::make_unique<FunctionAST>(std::move(prototype), std::move(body));
}

std::unique_ptr<ReturnAST> Parser::ParseReturn() {
	nextToken();
	if (token != tokens::IDENTIFIER && token != tokens::DECIMAL && token != tokens::INTEGER) {
		throw std::runtime_error("Excepted identifier or literal after return");
	}
	return std::make_unique<ReturnAST>(ParseExpression());
}

std::unique_ptr<LiteralAST> Parser::ParseLiteral() {
	if (token == tokens::INTEGER) {
		if (value[0] == '-') {
			if (value < "-9223372036854775807") {
				throw std::runtime_error("Integer literal " + value + " too small.");
			}
		}
		if (value > "18446744073709551615") {
			throw std::runtime_error("Integer literal " + value + " too large.");
		}

		nextToken();
		return std::make_unique<LiteralAST>(value, variableTypes::INT);
	}
	nextToken();
	return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseExpression() {
	switch (token) {
		case tokens::FUN:
			return ParseFunction();
		case tokens::SEMICOLON:
			nextToken();
			return ParseExpression();
		case tokens::RETURN:
			return ParseReturn();
		case tokens::INTEGER:
			return ParseLiteral();
		default:
			throw std::runtime_error("Unimplemented token " + value);
	}
}

class Analyzer : public AbstractDispatcher {
public:
	void Dispatch(FunctionAST &function) override {

	}
	void Dispatch(IdentifierAST &identifier) override {
		
	}
	void Dispatch(LiteralAST &literal) override {
		
	}
	void Dispatch(PrototypeAST &prototype) override {
		
	}
	void Dispatch(ReturnAST &return_) override {
		
	}
};

void Parser::Parse() {
	std::vector<std::unique_ptr<ExprAST>> tree;
	while (token != tokens::TOKEN_EOF) {
		tree.push_back(ParseExpression());
	}
	Analyzer analyzer;
	for (const auto& exp : tree) {
		exp->Dispatch(analyzer);
	}
}
