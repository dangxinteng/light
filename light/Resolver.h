#pragma once
#include"Interpret.h"
struct Resolver :public Visit
{
	enum class FunctionType
	{
		NONE,
		FUNCTION,
		METHOD
	};
	enum class ClassType
	{
		NONE,
		CLASS
	};
	Interpret &m_inter;
	std::vector<std::map<std::string, bool>> m_scopes;
	FunctionType m_currentFunction = FunctionType::NONE;
	ClassType m_currentClass = ClassType::NONE;
	Resolver(Interpret &inter)
		:m_inter(inter) {}
	~Resolver() = default;
	void PushScope();
	void PopScope();
	void Declare(Object &name);
	void Define(Object &name);
	void Resolve(std::vector<std::unique_ptr<Stmt>> &stmts);
	void Resolve(std::unique_ptr<Stmt> &stmt);
	void Resolve(std::unique_ptr<Expr> &expr);
	void ResolveLocal(Expr& expr, Token &name);
	void ResolveFunction(FunctionStmt & funstmt, FunctionType ft);
	Object VisitExprLiteral(ExprLiteral &liter)override;
	Object VisitExprBinary(ExprBinary &binary)override;
	Object VisitExprUnary(ExprUnary &unary)override;
	Object VisitExprGrouping(ExprGrouping &grouping)override;
	Object VisitExprAssign(ExprAssign &assign)override;
	Object VisitExprLogic(ExprLogic &logic)override;
	Object VisitExprFunCall(ExprFunCall &call)override;
	Object VisitExprGet(ExprGet &get)override;
	Object VisitExprSet(ExprSet &set)override;
	Object VisitExprThis(ExprThis &ts)override;
	void VisitExprStmt(ExprStmt &expr)override;
	void VisitPrintStmt(PrintStmt &print)override;
	void VisitVarStmt(VarStmt &var)override;
	void VisitBlockStmt(BlockStmt &block)override;
	void VisitIfStmt(IfStmt &ifstmt)override;
	void VisitWhileStmt(WhileStmt &whilestmt)override;
	void VisitFunctionStmt(FunctionStmt &funstmt)override;
	void VisitReturnStmt(ReturnStmt &ret)override;
	void VisitClassStmt(ClassStmt &cls)override;
};



