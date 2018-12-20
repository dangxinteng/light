#pragma once
#include<list>
#include"Parse.h"
struct Interpret:public Visit
{
	std::shared_ptr<Env> m_env = std::make_shared<Env>();
	std::shared_ptr<Env> m_globals = m_env;
	std::map<Expr*, size_t> m_locals;
	Interpret() = default;
	~Interpret() = default;
	std::shared_ptr<Env> GetEnv()const
	{
		return m_env;
	}
	void Resolve(Expr* expr, size_t depth)
	{
		m_locals[expr] = depth;
	}
	Object LookUpVariable(Token &name, Expr* expr)
	{
		if (m_locals.find(expr) != m_locals.end())
		{
			return m_env->getLocal(m_locals[expr], name.value);
		}
		return m_globals->get(name.value);
	}
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
	void ExecuteBlock(BlockStmt &block, std::shared_ptr<Env> env);	
	void VisitIfStmt(IfStmt &ifstmt)override;	
	void VisitWhileStmt(WhileStmt &whilestmt)override;	
	void VisitFunctionStmt(FunctionStmt &funstmt)override;
	void VisitReturnStmt(ReturnStmt &ret)override;
	void VisitClassStmt(ClassStmt &cls)override;
};



