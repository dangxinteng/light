#pragma once
#include<list>
#include"Env.h"
#include"Visit.h"
struct Expr
{
	enum class ExprType { Get };
	ExprType t;
	Expr() = default;
	Expr(ExprType t) :t(t){}
	virtual Object accept(Visit &visit) = 0;
	virtual ~Expr() = default;
};
struct ExprLiteral:public Expr
{
	Token tok;
	Object value;
	ExprLiteral(Token &tok, Object &value)
		: tok(tok), value(value){}
	Object accept(Visit &visit)override
	{ 
		return visit.VisitExprLiteral(*this);
	}
};
struct ExprBinary :public Expr
{
	Token tok;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
	ExprBinary(Token &tok, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
		: tok(tok), left(std::move(left)), right(std::move(right)){}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprBinary(*this);
	}
};
struct ExprUnary :public Expr
{
	Token tok;
	std::unique_ptr<Expr> expr;
	ExprUnary(Token &tok, std::unique_ptr<Expr> expr)
		: tok(tok), expr(std::move(expr)) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprUnary(*this);
	}
};
struct ExprGrouping :public Expr
{
	std::unique_ptr<Expr> expr;
	ExprGrouping(std::unique_ptr<Expr> expr)
		: expr(std::move(expr)){}
	Object accept(Visit &visit)override 
	{
		return visit.VisitExprGrouping(*this);
	}
};
struct ExprAssign :public Expr
{
	Token tok;
	std::unique_ptr<Expr> expr;
	ExprAssign(Token &tok, std::unique_ptr<Expr> expr)
		: tok(tok), expr(std::move(expr)) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprAssign(*this);
	}
};
struct ExprLogic :public Expr
{
	Token tok;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
	ExprLogic(Token &tok, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
		: tok(tok), left(std::move(left)), right(std::move(right)) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprLogic(*this);
	}
};
struct ExprFunCall :public Expr
{
	std::unique_ptr<Expr> callee;
	std::unique_ptr<std::vector<std::unique_ptr<Expr>>> args;
	ExprFunCall(std::unique_ptr<Expr> callee, std::unique_ptr<std::vector<std::unique_ptr<Expr>>> args)
		:callee(std::move(callee)), args(std::move(args)) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprFunCall(*this);
	}

};
struct ExprGet :public Expr
{
	Token tok;
	std::unique_ptr<Expr> expr;
	ExprGet(Token &tok, std::unique_ptr<Expr> expr)
		: Expr(ExprType::Get), tok(tok), expr(std::move(expr)) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprGet(*this);
	}
};
struct ExprSet :public Expr
{
	Token tok;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
	ExprSet(Token &tok, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
		: tok(tok), left(std::move(left)), right(std::move(right)) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprSet(*this);
	}
};
struct ExprThis :public Expr
{
	Token tok;
	ExprThis(Token &tok)
		: tok(tok) {}
	Object accept(Visit &visit)override
	{
		return visit.VisitExprThis(*this);
	}
};