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
program        �� declar EOF ;

declar         �� classDecl
				| funDecl
				| varDecl
				| stmt ;

classDecl      �� "class" IDENTIFIER "{" function* "}" ;
funDecl		   �� "fun" function ;
function       �� IDENTIFIER "(" parameters? ")" block ;
parameters     �� IDENTIFIER ( "," IDENTIFIER )* ; 

varDecl        �� "var" IDENTIFIER ( "=" expression )? ";" ;

stmt           �� exprStmt
				| ifStmt
				| printStmt
				| returnStmt
				| whileStmt
				| forStmt
				| block ;

returnStmt     �� "return" expression? ";" ;
whileStmt      �� "while" "(" expr ")" stmt ;
forStmt		   �� "for" "(" ( varDecl | exprStmt | ";" )expr? ";" expr? ")" stmt ;
ifStmt         �� "if" "(" expr ")" stmt ( "else" stmt )? ;
block          �� "{" declar* "}" ;

exprStmt       �� expr ";" ;
printStmt      �� "print"  expr  ";" ;

expr           �� assign;
assign   	   �� (fun_call ".")? IDENTIFIER "=" assign
				| logic_or ;
logic_or       �� logic_and ( "or" logic_and )* ;
logic_and      �� equal ( "and" equal )* ;
equal          �� compar (("!=" | "==") compar)*;
compar         �� add (( ">" | ">=" | "<" | "<=" ) add)* ;
add            �� mul (( "-" | "+" ) mul )* ;
mul            �� unary (( "/" | "*" ) unary)* ;
unary          �� ( "!" | "-" ) unary | fun_call;
fun_call       �� primary ("(" args? ")"| "." IDENTIFIER )* ;
args           �� expr ( "," expr )* ;
primary        �� NUMBER | STRING  | IDENTIFIER | "false" | "true" | "nil"
                | "(" expr ")";
*/