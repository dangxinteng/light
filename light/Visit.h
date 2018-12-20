#pragma once

struct ExprStmt;
struct PrintStmt;
struct VarStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;
struct FunctionStmt;
struct ReturnStmt;
struct ClassStmt;
struct ExprLiteral;
struct ExprBinary;
struct ExprUnary;
struct ExprGrouping;
struct ExprAssign;
struct ExprLogic;
struct ExprFunCall;
struct ExprGet;
struct ExprSet;
struct ExprThis;

struct Visit
{
	Visit() = default;
	virtual ~Visit() = default;
	virtual void   VisitExprStmt(ExprStmt &expr) = 0;
	virtual void   VisitPrintStmt(PrintStmt &print) = 0;
	virtual void   VisitVarStmt(VarStmt &var) = 0;
	virtual void   VisitBlockStmt(BlockStmt &block) = 0;
	virtual void   VisitIfStmt(IfStmt &ifstmt) = 0;
	virtual void   VisitWhileStmt(WhileStmt &whilestmt) = 0;
	virtual void   VisitFunctionStmt(FunctionStmt &funstmt) = 0;
	virtual void   VisitReturnStmt(ReturnStmt &ret) = 0;
	virtual void   VisitClassStmt(ClassStmt &cls) = 0;
	virtual Object VisitExprLiteral(ExprLiteral &liter) = 0;
	virtual Object VisitExprBinary(ExprBinary &binary) = 0;
	virtual Object VisitExprUnary(ExprUnary &unary) = 0;
	virtual Object VisitExprGrouping(ExprGrouping &grouping) = 0;
	virtual Object VisitExprAssign(ExprAssign &assign) = 0;
	virtual Object VisitExprLogic(ExprLogic &logic) = 0;
	virtual Object VisitExprFunCall(ExprFunCall &call) = 0;
	virtual Object VisitExprGet(ExprGet &get) = 0; 
	virtual Object VisitExprSet(ExprSet &set) = 0;
	virtual Object VisitExprThis(ExprThis &ts) = 0;
};