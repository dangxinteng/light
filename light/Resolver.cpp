#include"Resolver.h"

void Resolver::PushScope()
{
	m_scopes.push_back({});
}
void Resolver::PopScope()
{
	m_scopes.pop_back();
}
void Resolver::Declare(Object &name)
{
	if (m_scopes.empty()) 
		return;
	if (m_scopes.back().find(name) != m_scopes.back().end())
		throw Error("Variable '" + (std::string)name + "' was already declared.");
	m_scopes.back()[name] = false;
}
void Resolver::Define(Object &name)
{
	if (m_scopes.empty()) return;
	if (m_scopes.back().find(name) == m_scopes.back().end())
		throw Error("Undeclared name '" + (std::string)name + "'.");
	m_scopes.back()[name] = true;
}
void Resolver::Resolve(std::vector<std::unique_ptr<Stmt>> &stmts)
{
	for (auto &stmt : stmts)
		stmt->accept(*this);
}
void Resolver::Resolve(std::unique_ptr<Stmt>& stmt)
{
	stmt->accept(*this);
}
void Resolver::Resolve(std::unique_ptr<Expr>& expr)
{
	expr->accept(*this);
}
void Resolver::ResolveLocal(Expr& expr, Token &name)
{
	size_t n = 0;
	for (auto scope = m_scopes.rbegin(); scope != m_scopes.rend(); scope++)
	{
		if (scope->find(name.value) != scope->end())
		{
			m_inter.Resolve(&expr, n);
			return;
		}
		n++;
	}
}
void Resolver::ResolveFunction(FunctionStmt & funstmt, FunctionType ft)
{
	FunctionType markFunType = m_currentFunction;
	m_currentFunction = ft;
	PushScope();
	for (auto &param : funstmt.params)
	{
		Declare(param);
		Define(param);
	}
	Resolve(funstmt.body);
	PopScope();
	m_currentFunction = markFunType;
}

Object Resolver::VisitExprLiteral(ExprLiteral & liter)
{
	if (liter.tok.t == Token::IDENTIFIER)
	{
		if (!m_scopes.empty()
			&& m_scopes.back().find(liter.value) != m_scopes.back().end()
			&& !m_scopes.back()[liter.value])
		{
			throw Error("can't read variable refers to itself in initialisation.");
		}

		ResolveLocal(liter,liter.tok);
	}
	return Object();
}

Object Resolver::VisitExprBinary(ExprBinary & binary)
{
	Resolve(binary.left);
	Resolve(binary.right);
	return Object();
}

Object Resolver::VisitExprUnary(ExprUnary & unary)
{
	Resolve(unary.expr);
	return Object();
}

Object Resolver::VisitExprGrouping(ExprGrouping & grouping)
{
	Resolve(grouping.expr);
	return Object();
}

Object Resolver::VisitExprAssign(ExprAssign & assign)
{
	Resolve(assign.expr);
	ResolveLocal(assign, assign.tok);
	return Object();
}
Object Resolver::VisitExprLogic(ExprLogic & logic)
{
	Resolve(logic.left);
	Resolve(logic.right);
	return Object();
}
Object Resolver::VisitExprFunCall(ExprFunCall & call)
{
	Resolve(call.callee);
	if (call.args)
		for (auto &arg : *call.args)
			Resolve(arg);
	
	return Object();
}
Object Resolver::VisitExprGet(ExprGet & get)
{
	Resolve(get.expr);
	return Object();
}
Object Resolver::VisitExprSet(ExprSet & set)
{
	Resolve(set.left);
	Resolve(set.right);
	return Object();
}
Object Resolver::VisitExprThis(ExprThis & ts)
{
	if (m_currentClass == ClassType::NONE)
		throw Error("Cannot use 'this' outside of a class.");
	ResolveLocal(ts, ts.tok);
	return Object();
}
void Resolver::VisitExprStmt(ExprStmt & expr)
{
	Resolve(expr.expr);
}
void Resolver::VisitPrintStmt(PrintStmt & print)
{
	Resolve(print.expr);
}
void Resolver::VisitVarStmt(VarStmt & var)
{
	Declare(var.tok.value);
	if (var.expr)
		Resolve(var.expr);
	Define(var.tok.value);
}
void Resolver::VisitBlockStmt(BlockStmt & block)
{
	PushScope();
	Resolve(*block.blockStmts);
	PopScope();
}

void Resolver::VisitIfStmt(IfStmt & ifstmt)
{
	Resolve(ifstmt.expr);
	Resolve(ifstmt.thenStmt);
	if (!ifstmt.elseStmt)
		Resolve(ifstmt.elseStmt);
}

void Resolver::VisitWhileStmt(WhileStmt & whilestmt)
{
	Resolve(whilestmt.expr);
	Resolve(whilestmt.bodyStmt);
}

void Resolver::VisitFunctionStmt(FunctionStmt & funstmt)
{
	Declare(funstmt.tok.value);
	Define(funstmt.tok.value);
	ResolveFunction(funstmt, FunctionType::FUNCTION);
}

void Resolver::VisitReturnStmt(ReturnStmt & ret)
{
	if (m_currentFunction == FunctionType::NONE)
		throw Error("Cannot return from top-level code.");
	if (ret.expr)
		Resolve(ret.expr);
}

void Resolver::VisitClassStmt(ClassStmt & cls)
{
	ClassType ct = m_currentClass;
	m_currentClass = ClassType::CLASS;
	Declare(cls.tok.value);
	Define(cls.tok.value);
	PushScope();
	m_scopes.back()["this"] = true;
	if (cls.methods)
		for (auto &method : *cls.methods)
			ResolveFunction(*method, FunctionType::METHOD);
	PopScope();
	m_currentClass = ct;
}

