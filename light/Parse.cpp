#include"Parse.h"

std::vector<std::unique_ptr<Stmt>>* Parse::Program()
{
	std::vector<std::unique_ptr<Stmt>>* declar = ParseDeclar();
	Token token = m_tokensVector[current];
	if (token.t == Token::END_OF_LINE)
		return declar;
	else
		throw Error("valid statements, expect EOF of the end!");	
}

std::vector<std::unique_ptr<Stmt>>* Parse::ParseDeclar()
{
	Token token = m_tokensVector[current];
	while (token.t != Token::END_OF_LINE)
	{
		m_statements.push_back(std::move(Declar()));
		token = m_tokensVector[current];
	}

	return &m_statements;
}

std::unique_ptr<Stmt> Parse::Declar()
{
	
	Token token = m_tokensVector[current];
	if (token.t == Token::CLASS)
	{
		current++;
		return ClassDeclar();
	}
	if (token.t == Token::FUN)
	{
		current++;
		return FunDeclar();
	}
	if (token.t == Token::VAR)
	{
		current++;
		return VarDeclar();
	}

	return Statement();
}
std::unique_ptr<Stmt> Parse::ClassDeclar()
{
	std::unique_ptr<std::vector<std::unique_ptr<FunctionStmt>>> methods \
		(new std::vector<std::unique_ptr<FunctionStmt>>);
	Token idToken = m_tokensVector[current];
	if (idToken.t == Token::IDENTIFIER)
	{
		current++;
		Token token = m_tokensVector[current];
		if (token.t == Token::LEFT_BRACE)
		{
			current++;
			token = m_tokensVector[current];
			while (token.t != Token::RIGHT_BRACE && token.t != Token::END_OF_LINE)
			{
				//methods->push_back(std::move((FunDeclar())));
				methods->emplace_back(static_cast<FunctionStmt*>(FunDeclar().release()));
				token = m_tokensVector[current];
			}
			if (token.t == Token::END_OF_LINE)
				throw Error("Expect '}' after class body.");
			current++;
			return std::unique_ptr<Stmt>(new ClassStmt(idToken, std::move(methods)));
		}
		else
			throw Error("Expect '{' after class declar.");
	}
	else
		throw Error("Expect identifier after class declar.");
}
std::unique_ptr<Stmt> Parse::FunDeclar()
{
	std::list<Object> params;
	Token idToken = m_tokensVector[current];
	if (idToken.t == Token::IDENTIFIER)
	{
		current++;
		Token token = m_tokensVector[current];
		if (token.t == Token::LEFT_PAREN)
		{
			current++;
			token = m_tokensVector[current];
			if (token.t == Token::RIGHT_PAREN)
			{
				current++;
				token = m_tokensVector[current];
				if (token.t == Token::LEFT_BRACE)
				{
					current++;
					return std::unique_ptr<Stmt>(new FunctionStmt(idToken, std::move(params), std::move(Block())));
				}
				else
					throw Error("expect '{' after function declartion.");
			}
			do
			{			
				token = m_tokensVector[current];
				if (token.t = Token::IDENTIFIER)
					params.push_back(std::move(token.value));
				current++;
				token = m_tokensVector[current++];
			} while (token.t == Token::COMMA);

			if (token.t != Token::RIGHT_PAREN)
				throw Error("expect ')' after function declartion.");
			token = m_tokensVector[current];
			if (token.t == Token::LEFT_BRACE)
			{
				current++;
				return std::unique_ptr<Stmt>(new FunctionStmt(idToken, std::move(params), std::move(Block())));
			}
			else
				throw Error("expect '{' after function declartion.");
		}
		else
			throw Error("expect '(' after function declartion.");
	}
	else
		throw Error("expect 'identifier' after function declartion.");
}

std::unique_ptr<Stmt> Parse::VarDeclar()
{
	Token idToken = m_tokensVector[current];
	if (idToken.t == Token::IDENTIFIER)
	{
		current++;
		Token token = m_tokensVector[current];
		if (token.t == Token::SEMICOLON)
		{
			current++;
			return std::unique_ptr<Stmt>(new VarStmt(idToken));
		}
		if (token.t == Token::EQUAL)
		{
			current++;
			std::unique_ptr<Expr> expr = Expression();
			token = m_tokensVector[current];
			if (token.t == Token::SEMICOLON)
			{
				current++;
				return std::unique_ptr<Stmt>(new VarStmt(idToken, std::move(expr)));
			}
			else
				throw Error("expect ';' after VarDeclartion. ");
		}
		else
			throw Error("invalid VarDeclartion. ");
	}
	else
		throw Error("expect identifier after var kewword. ");
}

std::unique_ptr<Stmt> Parse::Statement()
{
	Token token = m_tokensVector[current];
	if (token.t == Token::IF)
	{
		current++;
		return IfStatement();
	}
	if (token.t == Token::WHILE)
	{
		current++;
		return WhileStatement();
	}
	if (token.t == Token::FOR)
	{
		current++;
		return ForStatement();
	}
	if (token.t == Token::PRINT)
	{
		current++;
		return PrintStatement();
	}
	if (token.t == Token::RETURN)
	{
		current++;
		return ReturnStatement();
	}
	if (token.t == Token::LEFT_BRACE)
	{
		current++;
		return Block();
	}
	return ExpressionStatement();
}
std::unique_ptr<Stmt>  Parse::IfStatement()
{
	Token token = m_tokensVector[current];
	if (token.t == Token::LEFT_PAREN)
	{
		current++;
		std::unique_ptr<Expr> expr = Expression();
		token = m_tokensVector[current];
		if (token.t == Token::RIGHT_PAREN)
		{
			current++;
			std::unique_ptr<Stmt> thenStmt = Statement();
			std::unique_ptr<Stmt> elseStmt = nullptr;
			token = m_tokensVector[current];
			if (token.t == Token::ELSE) {
				current++;
				elseStmt = Statement();
			}
			return std::unique_ptr<Stmt>(new IfStmt(std::move(expr), std::move(thenStmt),std::move(elseStmt)));
		}
		else
			throw Error("expect ')' after IfStmt. ");
	}
	else
		throw Error("expect '(' after IfStmt. ");
}
std::unique_ptr<Stmt>  Parse::WhileStatement()
{
	Token token = m_tokensVector[current];
	if (token.t == Token::LEFT_PAREN)
	{
		current++;
		std::unique_ptr<Expr> expr = Expression();
		token = m_tokensVector[current];
		if (token.t == Token::RIGHT_PAREN)
		{
			current++;
			std::unique_ptr<Stmt> bodyStmt = Statement();
			return std::unique_ptr<Stmt>(new WhileStmt(std::move(expr), std::move(bodyStmt)));
		}
		else
			throw Error("expect ')' after end of WhileStmt. ");
	}
	else
		throw Error("expect '(' after WhileStmt. ");
}
std::unique_ptr<Stmt>  Parse::ForStatement()
{
	Token token = m_tokensVector[current];
	if (token.t == Token::LEFT_PAREN)
	{
		current++;
		std::unique_ptr<Stmt> init = nullptr;
		token = m_tokensVector[current];
		if (token.t == Token::SEMICOLON)
			current++;
		else if (token.t == Token::VAR)
		{
			current++;
			init = VarDeclar();
		}
		else
		{
			current++;
			init = ExpressionStatement();
		}		
		std::unique_ptr<Expr> condition = nullptr;
		token = m_tokensVector[current];
		if (token.t != Token::SEMICOLON)
		{
			condition = Expression();
			token = m_tokensVector[current];
			if (token.t != Token::SEMICOLON)
				throw Error("Expect ';' after forloop condition.");
		}
		current++;			
		std::unique_ptr<Expr> increment = nullptr;
		token = m_tokensVector[current];
		if (token.t != Token::RIGHT_PAREN)
		{
			increment = Expression();
			token = m_tokensVector[current];
			if (token.t != Token::RIGHT_PAREN)
				throw Error("Expect ')' after  forloop increment.");
		}
		current++;
		std::unique_ptr<Stmt> body = Statement();
		if (increment != nullptr) 
		{
			std::unique_ptr<std::vector<std::unique_ptr<Stmt>>> blockStmts \
				(new std::vector<std::unique_ptr<Stmt>>);
			blockStmts->push_back(std::move(body));
			blockStmts->push_back(std::move(std::unique_ptr<Stmt>(new ExprStmt(std::move(increment)))));
			body = std::unique_ptr<Stmt>(new BlockStmt(std::move(blockStmts)));
		}
		if (condition == nullptr)
		{
			Token token(Token::TRUE, 0, 0, Object(true));
			condition = std::unique_ptr<Expr>(new ExprLiteral(token, token.value));
		}
		body = std::unique_ptr<Stmt>(new WhileStmt(std::move(condition), std::move(body)));
		if (init != nullptr) 
		{
			std::unique_ptr<std::vector<std::unique_ptr<Stmt>>> blockStmts \
				(new std::vector<std::unique_ptr<Stmt>>);
			blockStmts->push_back(std::move(init));
			blockStmts->push_back(std::move(body));
			body = std::unique_ptr<Stmt>(new BlockStmt(std::move(blockStmts)));
		}
		return body;
	}
	else
		throw Error("expect '(' after ForStmt. ");
}
std::unique_ptr<Stmt>  Parse::Block()
{
	std::unique_ptr<std::vector<std::unique_ptr<Stmt>>> blockStmts \
		(new std::vector<std::unique_ptr<Stmt>>);
	Token token = m_tokensVector[current];
	while (token.t != Token::RIGHT_BRACE && token.t != Token::END_OF_LINE)
	{
		if (token.t == Token::RIGHT_BRACE)
			break;
		blockStmts->push_back(std::move(Declar()));
		token = m_tokensVector[current];
	}
	if (token.t == Token::END_OF_LINE)
		throw Error("Expect '}' after block.");
	current++;
	return std::unique_ptr<Stmt>(new BlockStmt(std::move(blockStmts)));
}
std::unique_ptr<Stmt> Parse::ExpressionStatement()
{
	std::unique_ptr<Expr> expr = Expression();
	Token token = m_tokensVector[current];
	if (token.t == Token::SEMICOLON)
	{
		current++;
		return std::unique_ptr<Stmt>(new ExprStmt(std::move(expr)));
	}
		
	else
		throw Error("expect ';' after ExprStmt. ");
}
std::unique_ptr<Stmt> Parse::PrintStatement()
{
	std::unique_ptr<Expr> expr = Expression();
	Token token = m_tokensVector[current];
	if (token.t == Token::SEMICOLON)
	{
		current++;
		return std::unique_ptr<Stmt>(new PrintStmt(std::move(expr)));
	}
	else
		throw Error("expect ';' after PrintStmt. ");
}
std::unique_ptr<Stmt> Parse::ReturnStatement()
{
	std::unique_ptr<Expr> expr = nullptr;
	Token token = m_tokensVector[current];
	if (token.t != Token::SEMICOLON)	
		expr = Expression();
	token = m_tokensVector[current];
	if (token.t == Token::SEMICOLON)
	{
		current++;
		return std::unique_ptr<Stmt>(new ReturnStmt(std::move(expr)));
	}		
	else
		throw Error("expect ';' after end of returnStmt. ");
	
}
std::unique_ptr<Expr> Parse::Expression()
{
	return Assign();
}

std::unique_ptr<Expr> Parse::Assign()
{
	std::unique_ptr<Expr>  or = Or();

	Token token = m_tokensVector[current];
	if (token.t == Token::EQUAL)
	{
		Token id = m_tokensVector[current - 1];
		current++;
		std::unique_ptr<Expr> expr = Assign();
		if (or ->t == Expr::ExprType::Get)
		{
			auto expget = std::unique_ptr<ExprGet>(static_cast<ExprGet*>(or.release()));
			return std::unique_ptr<Expr>(new ExprSet(expget->tok, std::move(expget->expr), std::move(expr)));
		}
		return std::unique_ptr<Expr>(new ExprAssign(id, std::move(expr)));
	}
	else
		return or;
}
std::unique_ptr<Expr> Parse::Or()
{
	std::unique_ptr<Expr> left = And();
	Token token = m_tokensVector[current];
	while (token.t == Token::OR)
	{
		current++;
		std::unique_ptr<Expr> right = And();
		left = std::unique_ptr<Expr>(new ExprLogic(token, std::move(left), std::move(right)));
		token = m_tokensVector[current];
	}
	return left;
}
std::unique_ptr<Expr> Parse::And()
{
	std::unique_ptr<Expr> left = Equal();
	Token token = m_tokensVector[current];
	while (token.t == Token::AND)
	{
		current++;
		std::unique_ptr<Expr> right = Equal();
		left = std::unique_ptr<Expr>(new ExprLogic(token, std::move(left), std::move(right)));
		token = m_tokensVector[current];
	}
	return left;
}
std::unique_ptr<Expr> Parse::Equal()
{
	std::unique_ptr<Expr> left = Compar();
	Token token = m_tokensVector[current];
	while (token.t == Token::BANG_EQUAL || token.t == Token::EQUAL_EQUAL)
	{
		current++;
		std::unique_ptr<Expr> right = Compar();
		left = std::unique_ptr<Expr>(new ExprBinary(token, std::move(left), std::move(right)));
		token = m_tokensVector[current];
	}
	return left;
}

std::unique_ptr<Expr> Parse::Compar()
{
	std::unique_ptr<Expr> left = Add();
	Token token = m_tokensVector[current];
	while (token.t == Token::GREATER || token.t == Token::GREATER_EQUAL || token.t == Token::LESS || token.t == Token::LESS_EQUAL)
	{
		current++;
		std::unique_ptr<Expr> right = Add();
		left = std::unique_ptr<Expr>(new ExprBinary(token, std::move(left), std::move(right)));
		token = m_tokensVector[current];
	}
	return left;
}

std::unique_ptr<Expr> Parse::Add()
{
	std::unique_ptr<Expr> left = Mul();
	Token token = m_tokensVector[current];
	while (token.t == Token::MINUS || token.t == Token::PLUS)
	{
		current++;
		std::unique_ptr<Expr> right = Mul();
		left = std::unique_ptr<Expr>(new ExprBinary(token, std::move(left), std::move(right)));
		token = m_tokensVector[current];
	}
	return left;
}

std::unique_ptr<Expr> Parse::Mul()
{
	std::unique_ptr<Expr> left = Unary();
	Token token = m_tokensVector[current];
	while (token.t == Token::SLASH || token.t == Token::STAR)
	{
		current++;
		std::unique_ptr<Expr> right = Unary();
		left = std::unique_ptr<Expr>(new ExprBinary(token, std::move(left), std::move(right)));
		token = m_tokensVector[current];
	}
	return left;
}

std::unique_ptr<Expr> Parse::Unary()
{
	Token token = m_tokensVector[current];
	if (token.t == Token::MINUS || token.t == Token::BANG)
	{
		current++;
		std::unique_ptr<Expr> expr = Unary();
		return std::unique_ptr<Expr>(new ExprUnary(token, std::move(expr)));
	}
	return FunCall();
}

std::unique_ptr<Expr> Parse::FunCall()
{
	std::unique_ptr<Expr> callee = Primary();
	std::unique_ptr<std::vector<std::unique_ptr<Expr>>> \
		args(new std::vector<std::unique_ptr<Expr>>);
	while (true)
	{
		Token token = m_tokensVector[current];
		if (token.t == Token::LEFT_PAREN)
		{
			current++;
			token = m_tokensVector[current];
			if (token.t != Token::RIGHT_PAREN)
			{
				do {
					args->push_back(std::move(Expression()));
					token = m_tokensVector[current++];
				} while (token.t == Token::COMMA);
				token = m_tokensVector[current];
				if (token.t != Token::RIGHT_PAREN)
					throw Error("expect ')' after function argument expression. ");				
			}	
			current++;
			callee = std::unique_ptr<Expr>(new ExprFunCall(std::move(callee), std::move(args)));
		}
		else if(token.t == Token::DOT)
		{
			current++;
			token = m_tokensVector[current];
			if (token.t == Token::IDENTIFIER)
			{
				current++;
				callee = std::unique_ptr<Expr>(new ExprGet(token, std::move(callee)));
			}
			else
				throw Error("Expect property name after '.'.");
		}
		else
			break;
	}
	
	return callee;
}

std::unique_ptr<Expr> Parse::Primary()
{
	Token token = m_tokensVector[current];
	if (token.t == Token::NUMBER || token.t == Token::STRING || token.t == Token::TRUE || token.t == Token::FALSE || token.t == Token::NIL || token.t == Token::IDENTIFIER)
	{
		current++;
		return std::unique_ptr<Expr>(new ExprLiteral(token, token.value));
	}
	if (token.t == Token::THIS)
	{
		current++;
		return std::unique_ptr<Expr>(new ExprThis(token));
	}
	if (token.t == Token::LEFT_PAREN)
	{
		current++;
		std::unique_ptr<Expr> expr = Expression();
		token = m_tokensVector[current];
		if (token.t == Token::RIGHT_PAREN)
		{
			current++;
			return std::unique_ptr<Expr>(new ExprGrouping(std::move(expr)));
		}
		else
			throw Error("expect ')' after group expression. ");
	}
	if (token.t == Token::END_OF_LINE)
		return nullptr;
	else
		throw Error("invalid syntax!");
}
