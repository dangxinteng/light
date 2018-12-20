/*
Copyright: @2018-~ www.ojisuan.com
Created  : 2018/10/5
Author   : Ooo
*/


#include<fstream>
#include"Resolver.h"

void Main(const std::string &path);

int main(int argc, char *argv[])
{
	try
	{
		Main(argv[1]);
	}
	catch (Error& e)
	{
		std::cout << "Error caught:";
		std::cout << e.what() << std::endl;
		std::cin.get();
		return -1;
	}
	catch (std::exception& e)
	{
		std::cout << "Exception caught:";
		std::cout << e.what() << std::endl;
		std::cin.get();
		return -1;
	}
	return 0;
}


void Main(const std::string &path)
{
	std::ifstream in(path, std::ifstream::binary);

	if (!in)
		throw Error("Couldn't open " + path + ".");

	std::string code;
	auto start = in.tellg();
	in.seekg(std::ios::beg, std::ios::end);
	auto end = in.tellg();
	code.resize(int(end - start));
	in.seekg(std::ios::beg, std::ios::beg);
	in.read(&code[0], int(end - start));
	Scanner scanner(code);

	auto &tokens = scanner.ScanTokens();
	Parse parse(tokens);
	auto stmts = parse.Program();
	if (stmts->empty())
		throw Error("stmts is null!");
	Interpret interprete;
	Resolver resolve(interprete);
	resolve.Resolve(*stmts);
	for (auto it = stmts->begin(); it != stmts->end(); it++)
		(*it)->accept(interprete);
	
	std::cin.get();
}

/*
grammar:
program        ¡ú declar EOF ;

declar         ¡ú classDecl
				| funDecl
				| varDecl
				| stmt ;

classDecl      ¡ú "class" IDENTIFIER "{" function* "}" ;
funDecl		   ¡ú "fun" function ;
function       ¡ú IDENTIFIER "(" parameters? ")" block ;
parameters     ¡ú IDENTIFIER ( "," IDENTIFIER )* ; 

varDecl        ¡ú "var" IDENTIFIER ( "=" expression )? ";" ;

stmt           ¡ú exprStmt
				| ifStmt
				| printStmt
				| returnStmt
				| whileStmt
				| forStmt
				| block ;

returnStmt     ¡ú "return" expression? ";" ;
whileStmt      ¡ú "while" "(" expr ")" stmt ;
forStmt		   ¡ú "for" "(" ( varDecl | exprStmt | ";" )expr? ";" expr? ")" stmt ;
ifStmt         ¡ú "if" "(" expr ")" stmt ( "else" stmt )? ;
block          ¡ú "{" declar* "}" ;

exprStmt       ¡ú expr ";" ;
printStmt      ¡ú "print"  expr  ";" ;

expr           ¡ú assign;
assign   	   ¡ú (fun_call ".")? IDENTIFIER "=" assign
				| logic_or ;
logic_or       ¡ú logic_and ( "or" logic_and )* ;
logic_and      ¡ú equal ( "and" equal )* ;
equal          ¡ú compar (("!=" | "==") compar)*;
compar         ¡ú add (( ">" | ">=" | "<" | "<=" ) add)* ;
add            ¡ú mul (( "-" | "+" ) mul )* ;
mul            ¡ú unary (( "/" | "*" ) unary)* ;
unary          ¡ú ( "!" | "-" ) unary | fun_call;
fun_call       ¡ú primary ("(" args? ")"| "." IDENTIFIER )* ;
args           ¡ú expr ( "," expr )* ;
primary        ¡ú NUMBER | STRING  | IDENTIFIER | "false" | "true" | "nil"
                | "(" expr ")";
*/