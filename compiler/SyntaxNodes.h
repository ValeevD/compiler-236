#pragma once

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include "Symbols.h"
#include "generator.h"

using namespace std;

enum OutputType{
	STR = 0,
	INT = 1,
	FLOAT = 2
};

typedef list<NodeStatement*> block;

class NodeExpression: public NodeStatement{
protected:
	Symbol* sym;
	SymType* type;
public:
	NodeExpression(Symbol* _sym): sym(_sym) {type = NULL;}
	void SetSym(Symbol* _sym){sym = _sym;}
	string GetName() {return sym->GetName();}
	virtual Symbol* GetSym() {return sym;}
	virtual void SetType(SymType* _type){type = _type;}
	virtual void Generate(AsmCode* code){}
	virtual void GenerateLValue(AsmCode* code){}
	virtual void print(ofstream &os, int deep){}
	virtual SymType* GetType(){return type;}
	virtual bool IsLValue() {return false;}
};

class NodeUnaryOp : public NodeExpression{
protected:
	NodeExpression* par;
public:
	NodeUnaryOp(Symbol* _sym, NodeExpression* _par): NodeExpression(_sym) , par(_par) {}
	virtual bool IsIntToReal(){return false;}
	virtual void Generate(AsmCode* code);
	void GenerateForInt(AsmCode* code);
	void GenerateForReal(AsmCode* code);
	virtual bool IsLValue() {return false;}
	virtual void print(ofstream &os, int deep);
};

class NodeIntToReal : public NodeUnaryOp{
public:
	NodeIntToReal(NodeExpression* _par): NodeUnaryOp(new Symbol("(Real)"), _par) {}
	virtual bool IsIntToReal(){return false;}
	virtual void Generate(AsmCode* code);
};

class NodeBinaryOp : public NodeExpression{
protected:
	NodeExpression *r, *l;
public:
	NodeBinaryOp (Symbol* _sym, NodeExpression* _l, NodeExpression* _r): NodeExpression(_sym), l(_l), r(_r) {}
	void GenerateComparison(AsmCode* code, string name, bool isReal);
	void GenerateAriphmInt(AsmCode* code, string name);
	void GenerateAriphmReal(AsmCode* code, string name);
	virtual void Generate(AsmCode* code);
	virtual bool IsLValue() {return false;}
	virtual void print(ofstream &os, int deep);
};

class NodeCall : public NodeExpression{
	vector<NodeExpression*> params;
public:
	NodeCall(Symbol* _sym, vector<NodeExpression*> _params): NodeExpression(_sym), params(_params) {}
	virtual void Generate(AsmCode* code);
	bool IsFunc(){return sym->IsFunc();}
	virtual bool IsLValue() {return false;}
	virtual void print(ofstream &os, int deep);
};

class NodeWrite : public NodeStatement{
protected:
	OutputType t;
	NodeWrite* next;
public:
	NodeWrite(OutputType _t): t(_t) {next = NULL;}
	void SetNext(NodeWrite* _next){next = _next;}
	virtual void Generate(AsmCode* code){}
	virtual void print(ofstream &os, int deep){}
	virtual bool IsStrWrite(){return false;}
};

class NodeWriteln : public NodeWrite{
public:
	NodeWriteln(): NodeWrite(STR) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream &os, int deep);
};

class NodeWriteStr : public NodeWrite{
	string par;
public:
	NodeWriteStr(OutputType _t, string _par): NodeWrite(_t), par(_par) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream &os, int deep);
	virtual bool IsStrWrite(){return true;}
};

class NodeWriteExp : public NodeWrite{
	NodeExpression* exp;
public:
	NodeWriteExp(OutputType _t, NodeExpression* _exp): NodeWrite(_t), exp(_exp) {}
	void GenerateForInt(AsmCode* code);
	void GenerateForReal(AsmCode* code);
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream &os, int deep);
	virtual bool IsStrWrite(){return false;}
};

class NodeVar : public NodeExpression{	
public:
	NodeVar(Symbol* _sym): NodeExpression(_sym) {}
	virtual void Generate(AsmCode* code);
	virtual void GenerateLValue(AsmCode* code);
	virtual bool IsLValue() {return true;}
	virtual void print(ofstream &os, int deep);
};

class NodeConst : public NodeExpression{
public:
	NodeConst(Symbol* _sym): NodeExpression(_sym) {}
	string GetValue() {return ((SymVarConst*)GetSym())->GetValue();}
	virtual void Generate(AsmCode* code);
	void GenerateForReal(AsmCode* code);
	void GenerateForInt(AsmCode* code);
	virtual bool IsLValue() {return false;}
	virtual void print(ofstream &os, int deep);
};

class NodeArrayAccess: public NodeBinaryOp{
public:
	NodeArrayAccess(Symbol* _sym, NodeExpression* _l,NodeExpression* _r): NodeBinaryOp(_sym, _l, _r) {}
	virtual void Generate(AsmCode* code);
	virtual void GenerateLValue(AsmCode* code);
	void GenerateBase(AsmCode* code);
	virtual bool IsLValue() {return true;}
};

class NodeRecordAccess: public NodeBinaryOp{
public:
	NodeRecordAccess(Symbol* _sym, NodeExpression* _l,NodeExpression* _r): NodeBinaryOp(_sym, _l, _r) {}
	NodeExpression* GetRight() {return r;}
	virtual void Generate(AsmCode* code);
	virtual void GenerateLValue(AsmCode* code);
	void GenerateBase(AsmCode* code);
	void SetRight(NodeExpression* newR) {r = newR;}
	void SetLeft(NodeExpression* newL) {l = newL;}
	virtual bool IsLValue() {return true;}
};

//STATEMENTS

class StatementAssign : public NodeStatement{
	NodeExpression *r, *l;
	SymType* Type;
public:
	StatementAssign (NodeExpression* _l, NodeExpression* _r): r(_r), l(_l) {Type = _l->GetType();}
	SymType* GetType() {return Type;}
	NodeExpression* GetLeft() {return l;}
	virtual void Generate(AsmCode* code);
	virtual bool IsLValue() {return false;}
	virtual void print(ofstream &os, int deep);
};

class StatementBlock: public NodeStatement{
	block stms;
public:
	StatementBlock(block _stms): stms(_stms) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
	list<NodeStatement*> GetStatements() {return stms;}
};

class StatementCycle: public NodeStatement{
protected:
	NodeExpression* expr;
	NodeStatement* body;
public:
	StatementCycle(NodeExpression* _expr, NodeStatement* _body): expr(_expr), body (_body) {}
};

class StatementFor: public StatementCycle{
	StatementAssign* assign;
	bool isDownto;
public:
	StatementFor(NodeExpression* _expr ,NodeStatement* _body, StatementAssign* _assign, bool _isDownto): 
	              StatementCycle(_expr, _body), assign(_assign), isDownto(_isDownto) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
};

class StatementWhile: public StatementCycle{
public:
	StatementWhile(NodeExpression* _expr, NodeStatement* _body): StatementCycle(_expr, _body) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
};

class StatementRepeat: public StatementCycle{
public:
	StatementRepeat(NodeExpression* _expr, NodeStatement* _body): StatementCycle(_expr, _body) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
};

class StatementIf: public NodeStatement{
protected:
	NodeExpression* con;
	NodeStatement* then_block;
public:
	StatementIf(NodeExpression* _con, NodeStatement* _then): con(_con), then_block(_then)  {}
	pair<AsmLabel*,AsmLabel*> GenerateIf(AsmCode* code);
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
};

class StatementIfElse: public StatementIf{
	NodeStatement* else_block;
public:
	StatementIfElse(NodeExpression* _con, NodeStatement* _then, NodeStatement* _else): StatementIf(_con, _then), else_block(_else) {}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
};

class StatementExit: public NodeStatement{
	bool o;
public:
	StatementExit(bool _o): o(_o){}
	virtual void Generate(AsmCode* code);
	virtual void print(ofstream& os, int deep);
};