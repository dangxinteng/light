#pragma once
#include"Scanner.h"
#include"Stmt.h"

struct Parse
{
	std::vector<Token> &m_tokensVector;
	std::vector<std::unique_ptr<Stmt>> m_statements;
	int current = 0;
	Parse(std::vector<Token> &tokens) :m_tokensVector(tokens)
	{
	}
	std::vector<std::unique_ptr<Stmt>>* Program();
	std::vector<std::unique_ptr<Stmt>>* ParseDeclar();

	std::unique_ptr<Stmt> Declar();
	std::unique_ptr<Stmt> ClassDeclar();
	std::unique_ptr<Stmt> FunDeclar();
	std::unique_ptr<Stmt> VarDeclar();

	std::unique_ptr<Stmt> Statement();
	std::unique_ptr<Stmt> ExpressionStatement();
	std::unique_ptr<Stmt> PrintStatement();
	std::unique_ptr<Stmt> ReturnStatement();
	std::unique_ptr<Stmt> IfStatement();
	std::unique_ptr<Stmt> WhileStatement();
	std::unique_ptr<Stmt> ForStatement();
	std::unique_ptr<Stmt> Block();

	std::unique_ptr<Expr> Expression();	
	std::unique_ptr<Expr> Assign();
	std::unique_ptr<Expr> Or();
	std::unique_ptr<Expr> And();
	std::unique_ptr<Expr> Equal();
	std::unique_ptr<Expr> Compar();
	std::unique_ptr<Expr> Add();
	std::unique_ptr<Expr> Mul();
	std::unique_ptr<Expr> Unary();
	std::unique_ptr<Expr> FunCall();
	std::unique_ptr<Expr> Primary();	
};