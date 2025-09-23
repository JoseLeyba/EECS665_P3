#ifndef LEVI_AST_HPP
#define LEVI_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>
#include <string>


/* You'll probably want to add a bunch of ASTNode subclasses */

namespace leviathan{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;
class ExpNode;
class LocNode;
class CallExpNode;

/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(const Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	const Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	const Position * myPos = nullptr;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

/** 
 * \class InitializerNode
 **/
class InitializerNode : public ASTNode{
public:
	InitializerNode(const Position * p, std::list<ExpNode*>* exprs) : ASTNode(p), myExprs(exprs) {}
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<ExpNode*>* myExprs;
};

class StmtNode : public ASTNode{
public:
	StmtNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
};

/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/

class AssignStmtNode : public StmtNode{
public:
	AssignStmtNode(const Position * p, LocNode* loc, ExpNode* exp) : StmtNode(p), myLoc(loc), myExp(exp) {}
	void unparse(std::ostream& out, int indent) override;
private:
    LocNode* myLoc;
	ExpNode* myExp;

};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(const Position * p, CallExpNode* cen) : StmtNode(p), myCall(cen) {}
	void unparse(std::ostream& out, int indent) override;
private:
    CallExpNode* myCall;
};

class IfElseStmtNode : public StmtNode{

};

class IfStmtNode : public StmtNode{

};

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(const Position * p, LocNode* loc) : StmtNode(p), myLoc(loc) {assert(loc);}
	void unparse(std::ostream& out, int indent) override;
private:
    LocNode* myLoc;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(const Position * p, LocNode* loc) : StmtNode(p), myLoc(loc) {assert(loc);}
	void unparse(std::ostream& out, int indent) override;
private:
    LocNode* myLoc;
};

class ReadStmtNode : public StmtNode{
public:
	ReadStmtNode(const Position * p, LocNode* loc, ExpNode* exp) : StmtNode(p), myLoc(loc), myExp(exp) { assert(loc && exp);}
	void unparse(std::ostream& out, int indent) override;
private:
    LocNode* myLoc;
	ExpNode* myExp;

};

class ReturnStmtNode : public StmtNode{
public:
    ReturnStmtNode(const Position* p, ExpNode* e = nullptr)
        : StmtNode(p), myExp(e) {}

    void unparse(std::ostream& out, int indent) override;

private:
    ExpNode* myExp;
};

class SinkStmtNode : public StmtNode{
public:
	SinkStmtNode(const Position * p, IDNode* id) : StmtNode(p), myID(id) {}
	void unparse(std::ostream& out, int indent) override;
private:
    IDNode* myID;
};

class WhileStmtNode : public StmtNode{

};

class WriteStmtNode : public StmtNode{
public:
	WriteStmtNode(const Position * p, LocNode* loc, ExpNode* exp) : StmtNode(p), myLoc(loc), myExp(exp) { assert(loc && exp);}
	void unparse(std::ostream& out, int indent) override;
private:
    LocNode* myLoc;
    ExpNode* myExp;


};

class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class FnDeclNode : public DeclNode{
public:
    FnDeclNode(const Position* p, IDNode* id, std::list<DeclNode*>* formals,
               TypeNode* retType, std::list<StmtNode*>* body)
        : DeclNode(p), myID(id), myFormals(formals),
          myRetType(retType), myBody(body) {}
    void unparse(std::ostream& out, int indent) override;
private:
    IDNode* myID;
    std::list<DeclNode*>* myFormals;
    TypeNode* myRetType;
    std::list<StmtNode*>* myBody;
};

/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType) 
	: DeclNode(p), myID(inID), myType(inType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}

	VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType, InitializerNode * initType)
	: DeclNode(p), myID(inID), myType(inType), myInit(initType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
	void unparse(std::ostream& out, int indent);
    IDNode* id() const { return myID; }
    TypeNode* type() const { return myType; }
private:
	IDNode * myID;
	TypeNode * myType;
    InitializerNode * myInit;
};

class FormalDeclNode : public VarDeclNode{  

};


/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
	ExpNode(const Position * p) : ASTNode(p){ }
public:
    void unparse(std::ostream& out, int indent) override = 0;
};

class CallExpNode : public ExpNode{
public:
  CallExpNode(const Position* p, LocNode* callee, std::list<ExpNode*>* args)
    : ExpNode(p), myCallee(callee), myArgs(args) { assert(callee && args); }
  void unparse(std::ostream& out, int indent) override;
private:
  LocNode* myCallee;
  std::list<ExpNode*>* myArgs;
};

class FalseNode : public ExpNode{
public:
	FalseNode(const Position * p) : ExpNode(p) {}
	void unparse(std::ostream& out, int indent) override;
};

class IntLitNode : public ExpNode{
public:
	IntLitNode(const Position * p, int val) : ExpNode(p), value(val) {}
	void unparse(std::ostream& out, int indent) override;
private:
	int value;
};

class StrLitNode : public ExpNode{
public:
	StrLitNode(const Position * p, std::string string) : ExpNode(p), string(string) {}
	void unparse(std::ostream& out, int indent) override;
private:
	std::string string;
};

class ThrashNode : public ExpNode{
public:
  ThrashNode(const Position* p) : ExpNode(p) {}
  void unparse(std::ostream& out, int) override;
};

class TrueNode : public ExpNode{
public:
	TrueNode(const Position * p) : ExpNode(p) {}
	void unparse(std::ostream& out, int indent) override;
};

class UnaryExpNode : public ExpNode{
public:
    UnaryExpNode(const Position* p, ExpNode* exp)
        : ExpNode(p), myExp(exp) {}

protected:
    ExpNode* myExp;
};

class NegNode : public UnaryExpNode{
public:
    NegNode(const Position* p, ExpNode* exp) 
        : UnaryExpNode(p, exp) {}
    void unparse(std::ostream& out, int indent) override;
};


class NotNode : public UnaryExpNode{
public:
    NotNode(const Position* p, ExpNode* exp) 
        : UnaryExpNode(p, exp) {}
    void unparse(std::ostream& out, int indent) override;
};

/** A memory location. LocNodes subclass ExpNode
 * because they can be used as part of an expression. 
**/
class LocNode : public ExpNode{
public:
	LocNode(const Position * p)
	: ExpNode(p) {}
	void unparse(std::ostream& out, int indent) = 0;
};

class ArrayIndexNode : public LocNode{
public:
  ArrayIndexNode(const Position* p, LocNode* base, ExpNode* index)
    : LocNode(p), myBase(base), myIndex(index) { assert(base && index); }
  void unparse(std::ostream& out, int indent) override;
private:
  LocNode* myBase;
  ExpNode* myIndex;
};

/** An identifier. Note that IDNodes subclass
 * LocNode because they are a type of memory location. 
**/
class IDNode : public LocNode{
public:
	IDNode(const Position * p, std::string nameIn) 
	: LocNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent) override;
private:
	/** The name of the identifier **/
	std::string name;
};

class BinaryExpNode : public ExpNode{
protected:
    BinaryExpNode(const Position* p, ExpNode* lhs, ExpNode* rhs)
        : ExpNode(p), myLHS(lhs), myRHS(rhs) {}
    ExpNode* myLHS = nullptr;
    ExpNode* myRHS = nullptr;
};


/** All our nodes for MathOps act similarly, we define them as BinaryExpNodes who have 2 subtrees each with their own nodes
**/


class AndNode : public BinaryExpNode{
public:
    AndNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;

};


class DivideNode : public BinaryExpNode{
public:
    DivideNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;

};

class EqualsNode : public BinaryExpNode{
public:
    EqualsNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;

};

class GreaterEqNode : public BinaryExpNode{
public:
    GreaterEqNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
};

class GreaterNode : public BinaryExpNode{
public:
    GreaterNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
};

class LessEqNode : public BinaryExpNode{
public:
    LessEqNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
};

class LessNode : public BinaryExpNode{
public:
    LessNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
};

class MinusNode : public BinaryExpNode{
public:
    MinusNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
};

class NotEqualsNode : public BinaryExpNode{
public:
    NotEqualsNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
};

class OrNode : public BinaryExpNode{
public:
    OrNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;

};

class PlusNode : public BinaryExpNode{
public:
    PlusNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;

};

class TimesNode : public BinaryExpNode{
public:
    TimesNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	

};


/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){}
    
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent) override;
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent) override;
};

class ImmutableTypeNode : public TypeNode{
public:
public:
    ImmutableTypeNode(const Position* p, TypeNode* base)
      : TypeNode(p), myBase(base) { assert(base); }
    void unparse(std::ostream& out, int indent) override;
    TypeNode* base() const { return myBase; }
private:
    TypeNode* myBase;
};

class FileTypeNode : public TypeNode{
public:
	FileTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent) override;
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent) override;
};

class ArrayTypeNode : public TypeNode{
public:
    ArrayTypeNode(const Position* p, TypeNode* elem, int size)
      : TypeNode(p), myElem(elem), mySize(size) { assert(elem); }
    void unparse(std::ostream& out, int indent) override;
    TypeNode* elem() const { return myElem; }
    int size() const { return mySize; }
private:
    TypeNode* myElem;
    int mySize;
};

} //End namespace leviathan

#endif