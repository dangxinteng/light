#include "Interpret.h"
#include "Callable.h"

Object Interpret::VisitExprLiteral(ExprLiteral & liter)
{
	if (liter.tok.t == Token::IDENTIFIER)
		return LookUpVariable(liter.tok, &liter);//return m_env->get(liter.value);
	return liter.value;
}

Object Interpret::VisitExprBinary(ExprBinary & binary)
{
	switch (binary.tok.t)
	{
	case Token::PLUS:
		return binary.left->accept(*this) + binary.right->accept(*this);
	case Token::MINUS:
		return binary.left->accept(*this) - binary.right->accept(*this);
	case Token::STAR:
		return  binary.left->accept(*this) * binary.right->accept(*this);
	case Token::SLASH:
		return  binary.left->accept(*this) / binary.right->accept(*this);
	case Token::GREATER:
		return Object(binary.left->accept(*this)  >  binary.right->accept(*this));
	case Token::GREATER_EQUAL:
		return Object(binary.left->accept(*this) >= binary.right->accept(*this));
	case Token::LESS:
		return Object(binary.left->accept(*this) <   binary.right->accept(*this));
	case Token::LESS_EQUAL:
		return Object(binary.left->accept(*this) <= binary.right->accept(*this));
	case Token::BANG_EQUAL:
		return Object(binary.left->accept(*this) != binary.right->accept(*this));
	case Token::EQUAL_EQUAL:
		return Object(binary.left->accept(*this) == binary.right->accept(*this));
	default:
		throw Error("execute error! invalid binary operator!");
	}
}

Object Interpret::VisitExprUnary(ExprUnary & unary)
{
	switch (unary.tok.t)
	{
	case Token::MINUS:
		return Object(-unary.expr->accept(*this));
	case Token::BANG:
		return Object(!unary.expr->accept(*this));
	default:
		throw Error("execute error! invalid unary operator!");
	}

}

Object Interpret::VisitExprGrouping(ExprGrouping & grouping)
{
	return grouping.expr->accept(*this);
}

Object Interpret::VisitExprAssign(ExprAssign & assign)
{
	if (assign.tok.t == Token::IDENTIFIER)
	{
		Object o = assign.expr->accept(*this);
		if (m_locals.find(&assign) != m_locals.end())
			m_env->assignLocal(m_locals[&assign], assign.tok.value, o);
		else
			m_globals->assign(assign.tok.value, o);
		return o;
	}
	else
		throw Error("invalid assign expression!");
}

Object Interpret::VisitExprLogic(ExprLogic & logic)
{
	Object left = logic.left->accept(*this);

	if (logic.tok.t == Token::OR)
	{
		if (left) return left;
	}
	else
	{
		if (!left) return left;
	}
	return logic.right->accept(*this);
}

Object Interpret::VisitExprFunCall(ExprFunCall & call)
{
	Object callee = call.callee->accept(*this);
	std::list<Object> args;
	if (call.args)
	{
		for (auto it = call.args->begin(); it != call.args->end(); it++)
			args.push_back((*it)->accept(*this));
	}
	std::shared_ptr<Callable> function = callee;
	if (args.size() != function->Arity())
		throw Error("Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(args.size()) + ".");

	std::shared_ptr<Env> env = std::make_shared<Env>(function->GetEnv());
	std::shared_ptr<Env> prevEnv = m_env;
	m_env = env;
	Object value = function->Call(*this, args);
	m_env = prevEnv;

	return value;
}

Object Interpret::VisitExprGet(ExprGet & get)
{
	Object obj = get.expr->accept(*this);
	if (obj.IsInstace())
		return ((std::shared_ptr<Callable>)obj)->Get(get.tok);
	else
		throw Error("Only instances have properties.");
}

Object Interpret::VisitExprSet(ExprSet & set)
{
	Object left = set.left->accept(*this);
	if (!left.IsInstace())
		throw Error("Only instances have fields.");
	Object right = set.right->accept(*this);
	((std::shared_ptr<Callable>)left)->Set(set.tok, right);
	return right;
}

Object Interpret::VisitExprThis(ExprThis & ts)
{
	return LookUpVariable(ts.tok, &ts);
}

void Interpret::VisitExprStmt(ExprStmt & expr)
{
	expr.expr->accept(*this);
}

void Interpret::VisitPrintStmt(PrintStmt & print)
{
	std::cout << print.expr->accept(*this) << std::endl;
}

void Interpret::VisitVarStmt(VarStmt & var)
{
	if(!var.expr)
		m_env->define(var.tok.value, Object(nullptr));
	m_env->define(var.tok.value, var.expr->accept(*this));	
}

void Interpret::VisitBlockStmt(BlockStmt & block)
{
	ExecuteBlock(block, std::make_shared<Env>(m_env));
}

void Interpret::ExecuteBlock(BlockStmt & block, std::shared_ptr<Env> env)
{
	if (block.blockStmts->empty())
		return;
	std::shared_ptr<Env> prevEnv = m_env;
	m_env = env;
	for (auto stmt = block.blockStmts->begin();
		stmt != block.blockStmts->end();
		stmt++)
	{
		(*stmt)->accept(*this);
	}
	m_env = prevEnv;
}

void Interpret::VisitIfStmt(IfStmt & ifstmt)
{
	if (ifstmt.expr->accept(*this))
		ifstmt.thenStmt->accept(*this);
	else if (ifstmt.elseStmt != nullptr)
		ifstmt.elseStmt->accept(*this);
}

void Interpret::VisitWhileStmt(WhileStmt & whilestmt)
{
	while (whilestmt.expr->accept(*this))
	{
		whilestmt.bodyStmt->accept(*this);
	}
}

void Interpret::VisitFunctionStmt(FunctionStmt & funstmt)
{
	std::shared_ptr<Function> fun = std::make_shared<Function>(&funstmt,m_env);
	m_env->define(funstmt.tok.value, Object(fun));
}

void Interpret::VisitReturnStmt(ReturnStmt & ret)
{
	Object value(nullptr);
	if (ret.expr != nullptr)
		value = ret.expr->accept(*this);
	throw Return(value);
}

void Interpret::VisitClassStmt(ClassStmt & cls)
{
	std::unique_ptr<std::map<std::string, std::shared_ptr<Function>>> \
		methods(new std::map<std::string, std::shared_ptr<Function>>);
	for (auto &method : *cls.methods)
	{
		methods->emplace(method->tok.value, new Function(&*method, m_env));
	}
	std::shared_ptr<Class> cs = std::make_shared<Class>(cls.tok.value, std::move(methods));
	m_env->define(cls.tok.value, Object(cs));

}
