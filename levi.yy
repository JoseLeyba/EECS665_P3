%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{leviathan}
%define api.parser.class {Parser}
%define api.value.type variant
//%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	#include "ast.hpp"
	namespace leviathan {
		class Scanner;
	}

//The following definition is required when 
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { leviathan::Scanner &scanner }
%parse-param { leviathan::ProgramNode** root}

%code {
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

//%define parse.assert

/* Terminals */


%token                     END   0 "end file"
%token	<leviathan::Token *>       AND
%token	<leviathan::Token *>       ASSIGN
%token	<leviathan::Token *>       BOOL
%token	<leviathan::Token *>       COLON
%token	<leviathan::Token *>       COMMA
%token	<leviathan::Token *>       DASH
%token	<leviathan::Token *>       ELSE
%token	<leviathan::Token *>       SINK
%token	<leviathan::Token *>       EQUALS
%token	<leviathan::Token *>       FALSE
%token	<leviathan::Token *>       FILE
%token	<leviathan::Token *>       OUTPUT
%token	<leviathan::Token *>       GREATER
%token	<leviathan::Token *>       GREATEREQ
%token	<leviathan::IDToken *>     ID
%token	<leviathan::Token *>       IF
%token	<leviathan::Token *>       INT
%token	<leviathan::IntLitToken *> INTLITERAL
%token	<leviathan::Token *>       IMMUTABLE
%token	<leviathan::Token *>       LCURLY
%token	<leviathan::Token *>       LESS
%token	<leviathan::Token *>       LESSEQ
%token	<leviathan::Token *>       LPAREN
%token	<leviathan::Token *>       NOT
%token	<leviathan::Token *>       NOTEQUALS
%token	<leviathan::Token *>       OR
%token	<leviathan::Token *>       CROSS
%token	<leviathan::Token *>       POSTDEC
%token	<leviathan::Token *>       POSTINC
%token	<leviathan::Token *>       RETURN
%token	<leviathan::Token *>       RCURLY
%token	<leviathan::Token *>       RPAREN
%token	<leviathan::Token *>       SEMICOL
%token	<leviathan::Token *>       SLASH
%token	<leviathan::Token *>       STAR
%token	<leviathan::StrToken *>    STRINGLITERAL
%token	<leviathan::Token *>       INPUT
%token	<leviathan::Token *>       LBRACKET
%token	<leviathan::Token *>       RBRACKET
%token	<leviathan::Token *>       THRASH
%token	<leviathan::Token *>       TRUE
%token	<leviathan::Token *>       VOID
%token	<leviathan::Token *>       WHILE

%type <leviathan::ProgramNode *> program
%type <std::list<leviathan::DeclNode *> *> globals
%type <leviathan::DeclNode *> decl
%type <leviathan::VarDeclNode *> varDecl
%type <leviathan::TypeNode *> type
%type <leviathan::TypeNode *> dataType
%type <leviathan::TypeNode *> primType
%type <leviathan::LocNode *> loc
%type <leviathan::IDNode *> name
%type <leviathan::ExpNode *> literal
%type <leviathan::InitializerNode *> initializer

/* NOTE: Make sure to add precedence and associativity 
 * declarations
 */
%right ASSIGN
%left OR
%left AND
%nonassoc LESS GREATER LESSEQ GREATEREQ EQUALS NOTEQUALS
%left DASH CROSS
%left STAR SLASH
%left NOT 

%%

program		: globals
		  {
		  $$ = new ProgramNode($1);
		  *root = $$;
		  }

globals		: globals decl
		  {
		  $$ = $1;
		  DeclNode * declNode = $2;
		  $$->push_back(declNode);
		  }
		| /* epsilon */
		  {
		  $$ = new std::list<DeclNode *>();
		  }

decl		: varDecl SEMICOL
		  {
		  $$ = $1;
		  }
		| fnDecl
		  {
		  }

varDecl		: name COLON type
		  {
		  const Position * p;
		  p = new Position($1->pos(), $3->pos());
		  $$ = new VarDeclNode(p,$1, $3);
		  }
		| name COLON type ASSIGN initializer
		  {
			const Position * p;
		  	p = new Position($1->pos(), $4->pos());
		  	$$ = new VarDeclNode(p,$1, $3, $5);
		  }

type		: IMMUTABLE dataType
		  {
		  }
		| dataType
		  {
		  $$ = $1;
		  }

dataType	: primType LBRACKET INTLITERAL RBRACKET
		  {
		  }
		| primType
		  {
		  $$ = $1;
		  }

primType	: INT
		  {
		  $$ = new IntTypeNode($1->pos());
		  }
		| BOOL
		  {
		  }
		| FILE
		  {
		  }
		| VOID
		  {
		  }

fnDecl 		: name COLON LPAREN maybeFormals RPAREN type LCURLY stmtList RCURLY
		  {
		  }

maybeFormals	: /* epsilon */
		  {
		  }
		| formalList
		  {
		  }

formalList	: formalDecl
		  {
		  }
		| formalList COMMA formalDecl
		  {
		  }

formalDecl	: name COLON type
		  {
		  }
		| name COLON type ASSIGN initializer
		  {
		  }

stmtList	: /* epsilon */
		  {
		  }
		| stmtList stmt SEMICOL
		  {
		  }
		| stmtList blockStmt
		  {
		  }

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
		  {
		  }

stmt		: varDecl
		  {
		  }
		| loc ASSIGN exp
		  {
		  }
		| callExp
		  {
		  }
		| loc POSTDEC
		  {
		  }
		| loc POSTINC
		  {
		  }
		| loc OUTPUT exp
		  {
		  }
		| loc INPUT loc
		  {
		  }
		| SINK name
		  {
		  }
		| RETURN exp
		  {
		  }
		| RETURN
		  {
		  }

exp		: exp DASH exp
	  	  {
		  }
		| exp CROSS exp
	  	  {
		  }
		| exp STAR exp
	  	  {
		  }
		| exp SLASH exp
	  	  {
		  }
		| exp AND exp
	  	  {
		  }
		| exp OR exp
	  	  {
		  }
		| exp EQUALS exp
	  	  {
		  }
		| exp NOTEQUALS exp
	  	  {
		  }
		| exp GREATER exp
	  	  {
		  }
		| exp GREATEREQ exp
	  	  {
		  }
		| exp LESS exp
	  	  {
		  }
		| exp LESSEQ exp
	  	  {
		  }
		| NOT exp
	  	  {
		  }
		| DASH term
	  	  {
		  }
		| term
	  	  {
		  }


callExp		: loc LPAREN RPAREN
		  {
		  }
		| loc LPAREN actualsList RPAREN
		  {
		  }

actualsList	: exp
		  {
		  }
		| actualsList COMMA exp
		  {
		  }

term 		: loc
		  { }
		| literal
		  { }
		| THRASH 
		  { /*TODO $$ = new StrLitNode($1->pos(), $1->str());*/ }
		| LPAREN exp RPAREN
		  {}
		| callExp
		  {}

initializer	: literal
		  {
			const Position * p;
			p = new Position($1->pos(), $1->pos());
			$$ = new InitializerNode(p, $1);
		  } 
		| LBRACKET litList RBRACKET
		  { } 

litList		: literal
		  { }
		| literal COMMA litList
		  { } 

literal		: TRUE
		  {
			const Position * p;
			p = new Position($1->pos(), $1->pos());
			$$ = new TrueNode(p);
		  }
		| FALSE
		  { 
			const Position * p;
			p = new Position($1->pos(), $1->pos());
			$$ = new FalseNode(p);
		  }
		| INTLITERAL
		  {
			const Position * p;
			p = new Position($1->pos(), $1->pos());
			$$ = new IntLitNode(p, $1->num());
		  }
		| STRINGLITERAL
		  {
			const Position * p;
			p = new Position($1->pos(), $1->pos());
			$$ = new StrLitNode(p, $1->str());
		  }

loc		: name
		  {
		  $$ = $1;
		  }
		| loc LBRACKET exp RBRACKET
		  {
		  }

name		: ID
		  {
		  const Position * pos = $1->pos();
		  $$ = new IDNode(pos, $1->value());
		  }
	
%%

void leviathan::Parser::error(const std::string& msg){
	//std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
