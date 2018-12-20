#pragma once
#include"Expr.h"


struct Stmt
{
	Stmt() = default;
	virtual void accept(Visit &visit) = 0;
	virtual ~Stmt() = default;
};
struct ExprStmt :public Stmt
{
	std::unique_ptr<Expr> expr;
	ExprStmt(std::unique_ptr<Expr> expr)
		:expr(std::move(expr)) {}
	void accept(Visit &visit)override
	{ 
		visit.VisitExprStmt(*this);
	}
};
struct PrintStmt :public Stmt
{
	std::unique_ptr<Expr> expr;
	PrintStmt(std::unique_ptr<Expr> expr)
		:expr(std::move(expr)) {}
	void accept(Visit &visit)override
	{ 
		visit.VisitPrintStmt(*this);
	}
};

struct VarStmt :public Stmt
{
	Token tok;
	std::unique_ptr<Expr> expr;
	VarStmt(Token tok)
		:  tok(tok){}
	VarStmt(Token tok, std::unique_ptr<Expr> expr)
		: tok(tok), expr(std::move(expr)) {}
	void accept(Visit &visit)override
	{
		visit.VisitVarStmt(*this);
	}
};
struct BlockStmt :public Stmt
{
	std::unique_ptr<std::vector<std::unique_ptr<Stmt>>> blockStmts;
	BlockStmt(std::unique_ptr<std::vector<std::unique_ptr<Stmt>>> blockStmts)
		:blockStmts(std::move(blockStmts)) {}
	void accept(Visit &visit)override
	{
		visit.VisitBlockStmt(*this);
	}
};
struct IfStmt :public Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> thenStmt;
	std::unique_ptr<Stmt> elseStmt;
	IfStmt(
		std::unique_ptr<Expr> expr,     
		std::unique_ptr<Stmt> thenStmt,
		std::unique_ptr<Stmt> elseStmt  
	)
		:expr(std::move(expr)), thenStmt(std::move(thenStmt)),
	elseStmt(std::move(elseStmt)) {}
	void accept(Visit &visit)override
	{
		visit.VisitIfStmt(*this);
	}
};
struct WhileStmt :public Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> bodyStmt;
	WhileStmt(
		std::unique_ptr<Expr> expr,
		std::unique_ptr<Stmt> bodyStmt
	)
		:expr(std::move(expr)), bodyStmt(std::move(bodyStmt)) {}
	void accept(Visit &visit)override
	{
		visit.VisitWhileStmt(*this);
	}
};
struct FunctionStmt :public Stmt
{
	Token tok;
	std::list<Object> params;
	std::unique_ptr<Stmt> body;
	FunctionStmt(Token &tok, std::list<Object> params, std::unique_ptr<Stmt> body)
		: tok(tok), params(std::move(params)), body(std::move(body)) {}
	void accept(Visit &visit)override
	{
		visit.VisitFunctionStmt(*this);
	}
};
struct ReturnStmt :public Stmt
{
	std::unique_ptr<Expr> expr;
	ReturnStmt(std::unique_ptr<Expr> expr)
		: expr(std::move(expr)) {}
	void accept(Visit &visit)override
	{
		visit.VisitReturnStmt(*this);
	}
};
struct ClassStmt :public Stmt
{
	Token tok;
	std::unique_ptr<std::vector<std::unique_ptr<FunctionStmt>>> methods;
	ClassStmt(Token &tok, std::unique_ptr<std::vector<std::unique_ptr<FunctionStmt>>> methods)
		: tok(tok), methods(std::move(methods)) {}
	void accept(Visit &visit)override
	{
		visit.VisitClassStmt(*this);
	}
};
