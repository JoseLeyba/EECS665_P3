#include "ast.hpp"

namespace leviathan{

/*
doIndent is declared static, which means that it can 
only be called in this file (its symbol is not exported).
*/
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}

/*
In this code, the intention is that functions are grouped 
into files by purpose, rather than by class.
If you're used to having all of the functions of a class 
defined in the same file, this style may be a bit disorienting,
though it is legal. Thus, we can have
ProgramNode::unparse, which is the unparse method of ProgramNodes
defined in the same file as DeclNode::unparse, the unparse method
of DeclNodes.
*/


void ProgramNode::unparse(std::ostream& out, int indent){
	/* Oh, hey it's a for-each loop in C++!
	   The loop iterates over each element in a collection
	   without that gross i++ nonsense. 
	 */
	for (auto global : *myGlobals){
		/* The auto keyword tells the compiler
		   to (try to) figure out what the
		   type of a variable should be from 
		   context. here, since we're iterating
		   over a list of DeclNode *s, it's 
		   pretty clear that global is of 
		   type DeclNode *.
		*/
		global->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myID->unparse(out, 0);
	out << ": ";
	this->myType->unparse(out, 0);
    if (myInit != nullptr) {
		out << " = ";
		this->myInit->unparse(out, 0);
	}
	out << ";\n";
}

void IDNode::unparse(std::ostream& out, int indent){
	out << this->name;
}

void ImmutableTypeNode::unparse(std::ostream& out, int indent){
    out << "immutable";
    myBase->unparse(out, 0);
}

void IntTypeNode::unparse(std::ostream& out, int indent){
	out << "int";
}
void BoolTypeNode::unparse(std::ostream& out, int indent){
	out << "bool";

}void FileTypeNode::unparse(std::ostream& out, int indent){
	out << "file";
}
void VoidTypeNode::unparse(std::ostream& out, int indent){
    out << "void";
}
void FalseNode::unparse(std::ostream& out, int indent){
	out << "false";
}

void TrueNode::unparse(std::ostream& out, int indent){
	out << "true";
}

void IntLitNode::unparse(std::ostream &out, int indent) {
    out << value;
}

void StrLitNode::unparse(std::ostream &out, int indent) {
	out << string;
}

void InitializerNode::unparse(std::ostream &out, int indent) {
	this->myExp->unparse(out, 0);
}


void AndNode::unparse(std::ostream& out, int indent) {
    out << "(";
    myLHS->unparse(out, 0);
    out << " and ";
    myRHS->unparse(out, 0);
    out << ")";
}
void DivideNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " / ";
    myRHS->unparse(out, 0);
    out << ")";
}

void EqualsNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " == ";
    myRHS->unparse(out, 0);
    out << ")";
}

void GreaterEqNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " >= ";
    myRHS->unparse(out, 0);
    out << ")";
}

void GreaterNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " > ";
    myRHS->unparse(out, 0);
    out << ")";
}

void LessEqNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " <= ";
    myRHS->unparse(out, 0);
    out << ")";
}

void LessNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " < ";
    myRHS->unparse(out, 0);
    out << ")";
}

void MinusNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " - ";
    myRHS->unparse(out, 0);
    out << ")";
}

void NotEqualsNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " != ";
    myRHS->unparse(out, 0);
    out << ")";
}

void OrNode::unparse(std::ostream& out, int indent) {
    out << "(";
    myLHS->unparse(out, 0);
    out << " or ";
    myRHS->unparse(out, 0);
    out << ")";
}

void PlusNode::unparse(std::ostream& out, int indent){
    out << "(";
    myLHS->unparse(out, 0);
    out << " + ";
    myRHS->unparse(out, 0);
    out << ")";
}

void TimesNode::unparse(std::ostream& out, int indent) {
    out << "(";
    myLHS->unparse(out, 0);
    out << " * ";
    myRHS->unparse(out, 0);
    out << ")";
}

void NegNode::unparse(std::ostream& out, int indent) {
    out << "(-";
    myExp->unparse(out, 0);
    out << ")";
}

void NotNode::unparse(std::ostream& out, int indent) {
    out << "(!";
    myExp->unparse(out, 0);
    out << ")";
}
void ReturnStmtNode::unparse(std::ostream& out, int indent) {
    doIndent(out, indent);
    out << "return";
    if (myExp) {
        out << " ";
        myExp->unparse(out, 0);
    }
    out << ";\n";
}



void FnDeclNode::unparse(std::ostream& out, int indent){
    doIndent(out, indent);
    myID->unparse(out, 0);
    out << " : (";

    bool first = true;
    for (auto d : *myFormals){
        if (!first) out << ", ";
        first = false;

        if (auto vd = dynamic_cast<VarDeclNode*>(d)){
            vd->id()->unparse(out, 0);
            out << ": ";
            vd->type()->unparse(out, 0);
        } else {
            d->unparse(out, 0);
        }
    }

    out << ") ";
    myRetType->unparse(out, 0);
    out << " {\n";

    for (auto s : *myBody){
        s->unparse(out, indent+1);
    }

    doIndent(out, indent);
    out << "}\n";
}

void ArrayTypeNode::unparse(std::ostream& out, int indent) {
    myElem->unparse(out, 0);
    out << "[";
    out << mySize;
    out << "]";
}

void ArrayIndexNode::unparse(std::ostream& out, int){
  myBase->unparse(out, 0);
  out << "[";
  myIndex->unparse(out, 0);
  out << "]";
}

void CallExpNode::unparse(std::ostream& out, int){
  myCallee->unparse(out, 0);
  out << "(";
  bool first = true;
  for (auto* e : *myArgs){
    if (!first) out << ", ";
    first = false;
    e->unparse(out, 0);
  }
  out << ")";
}
} // End namespace leviathan
