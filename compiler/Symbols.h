#pragma once

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "scanner.h"

using namespace std;

extern bool IsExplType (string name);

const int INT_SIZE = 1;
const int REAL_SIZE = 1;

enum VarType{
	GLOBAL,
	LOCAL,
	PARAM,
	PARAM_VAR
};

class SyntaxNode{
public:
	virtual void print(ofstream& os, int deep){}
};

class AsmCode;

class NodeStatement: public SyntaxNode
{
public:
	virtual void Generate(AsmCode* code){}
	virtual void print(ofstream& os, int deep){}
};

class SymType;

class Symbol{
protected:
	string name;
public:
	Symbol(string _name): name(_name) {}
	virtual SymType* GetType () {return NULL;}
	virtual bool IsType() {return false;}
	virtual bool IsVar() {return false;}
	virtual bool IsConst() {return false;}
	virtual bool IsInt () {return false;}
	virtual bool IsReal () {return false;}
	virtual bool IsRec() {return false;}
	virtual bool IsArr() {return false;}
	virtual bool IsScalar() {return false;}
	virtual bool IsParamVar() {return false;}
	virtual bool IsAlias() {return false;}
	virtual void SetName(string _name) {name = _name;}
	virtual string GetName() {return name;}
	virtual bool IsSub() {return false;}
	virtual bool IsProc() {return false;}
	virtual bool IsFunc() {return false;}
	virtual void print(ofstream &os, bool IsDef){};
};

class SymType: public Symbol{
public:
	SymType (string _name): Symbol(_name) {};
	virtual bool IsType() {return true;}
	virtual int GetSize() {return 0;}
	virtual SymType* GetElemType() {return this;}
	virtual void print(ofstream &os, bool IsDef){os << name;}
};

class SymVar: public Symbol{
protected:
	int offset;
	SymType* type;
	bool wasDefined;
	VarType t;
public:
	SymVar(string _name, SymType* _type): Symbol(_name), type(_type), wasDefined(false) {t = GLOBAL;}
	int GetOffset(){return offset;}
	void SetVarType(VarType _t){t = _t;}
	VarType GetVarType() {return t;}
	virtual SymType* GetType () {return type;}
	bool WasDefined() {return wasDefined;}
	void Define() {wasDefined = true;}
	int GetSize(){return type->GetSize();}
	virtual void SetType (SymType* t) {type = t;}
	void SetOffset(int _offset){offset = _offset;}
	virtual bool IsVar() {return true;}
	virtual bool IsRecord() {return true;}
	virtual bool IsParam() {return false;}
	virtual bool IsParamVar() {return false;}
	virtual void print(ofstream &os, bool IsDef)
	{
		os <<"var "<< name << " : ";
		type->print(os, IsDef);
		os << ";";
	}
};

class SymVarConst : public SymVar{
	string value;
public:
	SymVarConst(string _name, string _value , SymType* _type): SymVar(_name, _type), value(_value) {};
	virtual SymType* GetType () {return type;}
	bool IsConst() {return true;}
	string GetValue() {return value;}
	virtual void print(ofstream &os, bool IsDef)
	{
		if (!IsDef && !IsExplType(name))
			os << "const " << name;
		else
			if (IsExplType(name))
				os << "const " << value;
			else
				os << "const " << name << " = " << value << ";"<<endl;
	}
};

class SymVarParam : public SymVar{
public:
	SymVarParam(string _name, SymType* _type): SymVar(_name, _type){t = PARAM;};
	virtual bool IsParam() {return true;}
	virtual bool IsParamVar() {return false;}
	virtual SymType* GetType (){return type;}
};

class SymVarParam_var : public SymVarParam{
public:
	SymVarParam_var(string _name, SymType* _type): SymVarParam(_name, _type) {t = PARAM_VAR;}
	virtual bool IsParamVar() {return true;}
	virtual void print(ofstream &os, bool IsDef)
	{
		os <<"var &"<< name << " : ";
		type->print(os, IsDef);
		os <<"; ";
	}
};

class SymVarGlobal : public SymVar{
public:
	SymVarGlobal(string _name, SymType* _type): SymVar(_name, _type) {};
};

class SymVarLocal : public SymVar{
public:
	SymVarLocal(string _name, SymType* _type): SymVar(_name, _type){};
};

typedef map<string, Symbol*> _Table;
typedef pair<string, Symbol*> SymElem;

class SymTable{
	int offset;
	_Table table;
	vector<pair<string, int>> var;
public:
	SymTable(): offset(0) {}
	vector<pair<string, int>>* get_var(){
		return &var;
	}
	void MakeLocals()
	{
		for(size_t i=0; i<var.size(); ++i)
			((SymVar*)(table.find(var[i].first)->second))->SetVarType(LOCAL);
	}
	int GetOffset()
	{
		return offset;
	}
	int GetOffsetByName(string name)
	{
		vector<pair<string, int>>::iterator it = var.begin();
		while(it != var.end())
		{
			if (it->first == name)
				return it->second;
			++it;
		}
		return 0;
	}
	void insert(SymElem elem){
		table.insert(elem);
		if (elem.second->IsVar())
		{
			var.push_back(pair<string, int> (elem.first, offset));
			((SymVar*)elem.second)->SetOffset(offset);
			if(((SymVar*)elem.second)->GetVarType() != PARAM_VAR)
				offset += elem.second->GetType()->GetSize();
			else
				offset += 1;
		}
	}
	//insert without recounting of offset
	void insert_vo(SymElem elem)
	{
		table.insert(elem);
		var.push_back(pair<string, int> (elem.first, offset));
		((SymVar*)elem.second)->SetOffset(offset);
	}
	_Table::iterator find(string name)
	{
		return table.find(name);
	}
	_Table::iterator end()
	{
		return table.end();
	}
	_Table::iterator begin()
	{
		return table.begin();
	}
	void delete_element(string name)
	{
		table.erase(table.find(name));
	}
};

typedef pair<string, SymType*> Symb;
typedef vector<SymTable*> TableStack;

class SymTableStack{
	TableStack Stack;
public:
	SymTable* GetTopTable() {return Stack[Stack.size()-1];}
	void PushTable(SymTable* st) {Stack.push_back(st);}
	void PopTable () {Stack.pop_back();}
	void Find(string name, Token t)
	{
		for (TableStack::iterator it = Stack.begin(); it != Stack.end(); ++it)
		{
			if ((*it)->find(name) != (*it)->end())
				throw Error("identifier already declared", t);
		}
	}
	_Table::iterator Find(string name, bool &o)
	{
		for (TableStack::iterator it = Stack.begin(); it != Stack.end(); ++it)
		{
			_Table::iterator iter = (*it)->find(name);
			if (iter != (*it)->end())
			{
				o = true;
				return iter;
			}
		}
		o = false;
		return GetTopTable()->end();
	}
};

class SymProc: public Symbol{
protected:
	vector<string>* argNames;
	SymTable* arguments;
	SymTable* local;
	NodeStatement* body;
	bool isForward;
	bool isTranslated;
public:
	SymProc (string _name, vector<string>* _argNames, SymTable* _arguments, SymTable* _local, bool _isForward): Symbol(_name), argNames(_argNames),
	                                                                                           arguments(_arguments), local(_local),
	                                                                                           isForward (_isForward) {isTranslated = false;}
	virtual void print (ofstream &os, bool IsDef)
	{
		os << "procedure " << name;
		if (!argNames->empty())
		{
			os << "(";
			for (vector<string>::const_iterator it = argNames->begin(); it != argNames->end(); ++it)
				arguments->find(*it)->second->print(os, false);
			os << ")";
		}
		os << ";" << endl;
	}
	virtual bool IsProc() {return true;}
	bool IsTranslated() {return isTranslated;}
	void SetTranslated() {isTranslated = true;}
	virtual bool IsSub() {return true;}
	bool IsForward() {return isForward;}
	void SetLocTab(SymTable* loc){local = loc;}
	void SetBody(NodeStatement* _body){body = _body;}
	NodeStatement* GetBody() {return body;}
	SymTable* GetArgTable(){return arguments;}
	SymTable* GetLocals(){return local;}
	vector<string>* GetArgNames(){return argNames;}
};

class SymFunc: public SymProc{
	SymType* Type;
public:
	SymFunc (string _name, vector<string>* _argNames, SymTable* _arguments, 
		     SymTable* _local, bool _isForward, SymType* _type): SymProc(_name, _argNames, _arguments, _local, _isForward), Type(_type) {};
	void SetType(SymType* _type){Type = _type;}
	virtual SymType* GetType () {return Type;}
	virtual bool IsFunc() {return true;}
	virtual void print(ofstream &os, bool IsDef)
	{
		os << "function " << name;
		if (!argNames->empty())
		{
			os << "(";
			for (vector<string>::const_iterator it = argNames->begin(); it != argNames->end(); ++it)
				arguments->find(*it)->second->print(os, false);
			os << ")";
		}
		os << " : ";
		Type->print(os, false);
		os << ";"<< endl;
	}
};



class SymTypeScalar: public SymType{
public:
	SymTypeScalar (string _name): SymType(_name) {};
	virtual bool IsScalar() {return true;}
};

class SymTypeInteger: public SymTypeScalar{
public:
	SymTypeInteger (string _name): SymTypeScalar(_name) {};
	virtual int GetSize() {return INT_SIZE;}
	bool IsInt () {return true;}	
};

class SymTypeFloat: public SymTypeScalar{
public:
	SymTypeFloat (string _name): SymTypeScalar(_name) {};	
	virtual int GetSize() {return REAL_SIZE;}
	virtual bool IsReal () {return true;}
};

class SymTypeArray : public SymType{
	SymType* ElemType;
	int l,r;
public:
	SymTypeArray (string _name, SymType* _ElemType, int _l, int _r): SymType(_name), ElemType(_ElemType), l(_l), r(_r) {};
	SymType* GetElType(){return ElemType;}
	virtual SymType* GetElemType() {return ElemType;}
	int GetLeftBound(){return l;}
	virtual bool IsArr(){return true;}
	virtual int GetSize() {return (r-l+1)*(GetElemType()->GetSize());}
	virtual void print(ofstream &os, bool IsDef)
	{
		if (!IsDef)
			os << name;
		else{
			if (!IsExplType(name))
				os << "type " << name << " = ";
			os<< "array [" << l << ".." << r<< "] of ";
			ElemType->print(os, IsExplType(ElemType->GetName()));
		}
	}
};

class SymTypeRecord : public SymType{
	vector<string>* fldNames;
	SymTable* fields;
	int size;
public:
	SymTypeRecord(string _name, SymTable* _fields, vector<string>* _fldNames): SymType(_name), fields(_fields), fldNames(_fldNames) 
	{
		size = 0;
		_Table::iterator it1 = fields->begin();
		vector<string>::iterator it2 = fldNames->begin();
		for (_Table::iterator it1 = fields->begin(); it1 != fields->end(); ++it1)
			size += it1->second->GetType()->GetSize();
	}
	SymTable* GetFields (){return fields;}
	int GetOffsetByName(string name){return fields->GetOffsetByName(name);}
	virtual bool IsRec() {return true;}
	virtual int GetSize() {return size;}
	void print(ofstream &os, bool IsDef)
	{
		if (!IsDef)
			os << name;
		else
		{
		    if (!IsExplType(name))
				os << "type " << name << " = " ;
			os << "record \n";
			for (_Table::iterator it = fields->begin(); it != fields->end(); ++it)
			{
				it->second->print(os,(*it->second).IsVar() && IsExplType(it->second->GetType()->GetName()));
				os <<endl;
			}
			os << "end";
		}
	}
};

class SymTypeAlias : public SymType{
	SymType* refType;
public:
	SymTypeAlias(string _name, SymType* _ref): SymType(_name), refType(_ref) {}
	SymType* GetRefType (){return refType;}
	virtual bool IsAlias() {return true;}
	virtual int GetSize(){return refType->GetSize();}
	virtual bool IsArr() {return refType->IsArr();}
	virtual bool IsRec(){return refType->IsRec();}
	virtual bool IsReal () {return refType->IsReal();}
	virtual bool IsInt () {return refType->IsInt();}
	virtual bool IsScalar () {return refType->IsScalar();}
	virtual void print(ofstream &os, bool IsDef)
	{
		if(!IsDef)
			os << name;
		else
		{
			if (!IsExplType(name))
				os << "type " << name<< " = ";
			refType->print(os , false);
		}
	}
};
