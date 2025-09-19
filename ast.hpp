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

};

class CallStmtNode : public StmtNode{

};

class IfElseStmtNode : public StmtNode{

};

class IfStmtNode : public StmtNode{

};

class PostDecStmtNode : public StmtNode{

};

class PostIncStmtNode : public StmtNode{

};

class ReadStmtNode : public StmtNode{

};

class ReturnStmtNode : public StmtNode{

};

class SinkStmtNode : public StmtNode{

};

class WhileStmtNode : public StmtNode{

};

class WriteStmtNode : public StmtNode{

};

class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class FnDeclNode : public DeclNode{

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
	void unparse(std::ostream& out, int indent);
private:
	IDNode * myID;
	TypeNode * myType;
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

};

class FalseNode : public ExpNode{
public:
	FalseNode(const Position* p) : ExpNode(p) {}
    void unparse(std::ostream& out, int indent) override;
};

class IntLitNode : public ExpNode{
public:
    IntLitNode(const Position* p, int val) : ExpNode(p), value(val) {}
    void unparse(std::ostream& out, int indent) override;
private:
    int value;
};

class StrLitNode : public ExpNode{
public:
    StrLitNode(const Position* p, const std::string& val) : ExpNode(p), value(val) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    std::string value;
};

class ThrashNode : public ExpNode{

};

class TrueNode : public ExpNode{
public:
	TrueNode(const Position* p) : ExpNode(p) {}
    void unparse(std::ostream& out, int indent) override;

};

class UnaryExpNode : public ExpNode{
protected:
    UnaryExpNode(const Position* p, ExpNode* sub)
        : ExpNode(p), mySub(sub) {}
    ExpNode* mySub;
};

class NegNode : public UnaryExpNode{
public:
    NegNode(const Position* p, ExpNode* sub)
        : UnaryExpNode(p, sub) {}
    void unparse(std::ostream& out, int indent) override;
};

class NotNode : public UnaryExpNode{
public:
    NotNode(const Position* p, ExpNode* sub)
        : UnaryExpNode(p, sub) {}
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
    ArrayIndexNode(const Position* p, ExpNode* base, ExpNode* index)
        : LocNode(p), myBase(base), myIndex(index) {}
    void unparse(std::ostream& out, int indent) override;
private:
    ExpNode* myBase;
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
	void unparse(std::ostream& out, int indent) override;
};


/** All our nodes for MathOps act similarly, we define them as BinaryExpNodes who have 2 subtrees each with their own nodes
**/


class AndNode : public BinaryExpNode{
public:
    AndNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};


class DivideNode : public BinaryExpNode{
public:
    DivideNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class EqualsNode : public BinaryExpNode{
public:
    EqualsNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class GreaterEqNode : public BinaryExpNode{
public:
    GreaterEqNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class GreaterNode : public BinaryExpNode{
public:
    GreaterNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class LessEqNode : public BinaryExpNode{
public:
    LessEqNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class LessNode : public BinaryExpNode{
public:
    LessNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class MinusNode : public BinaryExpNode{
public:
    MinusNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class NotEqualsNode : public BinaryExpNode{
public:
    NotEqualsNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class OrNode : public BinaryExpNode{
public:
    OrNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class PlusNode : public BinaryExpNode{
public:
    PlusNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};

class TimesNode : public BinaryExpNode{
public:
    TimesNode(const Position* p,ExpNode* lhs, ExpNode* rhs)
        : BinaryExpNode(p, lhs, rhs) {}
    void unparse(std::ostream& out, int indent) override;
	
private:
    ExpNode* myLHS;
    ExpNode* myRHS;
};


/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent) override;
};

class VoidTypeNode : public TypeNode{

};

class ImmutableTypeNode : public TypeNode{

};

class FileTypeNode : public TypeNode{

};

class BoolTypeNode : public TypeNode{

};

class ArrayTypeNode : public TypeNode{

};

} //End namespace leviathan

#endif
