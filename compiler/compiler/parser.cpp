#include "parser.h"

Token t(" ", wrong_token, Coordinates(0,0));

bool IsExplType (string name)
{
	return name[0] >= '0' && name[0] <= '9'; 
}

int NotExplType = 0;

void Check (string name, SymTable* st)
{
	if (st->find(name) != st->end())
		throw Error("identifier already declared", t);
}

Token Parser::RequireToken(string _t, string err)
{
	if (t.GetValue() != _t)
		throw Error(err, t);
	return sc.GetNextToken();
}
//~~~~~Declaration_Parsing~~~~~~~~~~~

SymTable* Parser::ParseArguments(vector<string>* &arguments)
{
	SymTable* argTable = new SymTable;
	if (t.GetValue() != "(")
		return argTable;
	vector<string> argNames;
	bool isVar = false;
	do
	{
		t = sc.GetNextToken();
		if (t.GetValue() == "var")
		{
			isVar = true;
			t = sc.GetNextToken();
		}
		if (t.GetType() != identifier)
			throw Error("incorrect name of argument", t);
		argNames.push_back(t.GetValue());
		arguments->push_back(t.GetValue());
		t = sc.GetNextToken();
		if (t.GetValue() == ",")
			continue;
		if (t.GetValue() == ":")
		{
			SymType* Type = ParseType(false);
			if (IsExplType(Type->GetName()))
				throw Error("arguments type mismatch", t);
			t = sc.GetNextToken();
			for(vector<string>::iterator it= argNames.begin(); it != argNames.end(); ++it)
			{
				if (argTable->find(*it) != argTable->end())
					throw Error("identifier already declared",t);
				if (isVar)
					argTable->insert(pair<string, SymVar*> (*it, new SymVarParam_var(*it, Type)));
				else
					argTable->insert(pair<string, SymVar*> (*it, new SymVarParam(*it, Type)));
			}
			isVar = false;
			argNames.clear();
			if (t.GetValue() != ";" && t.GetValue() != ")")
				throw Error("incorrect function defenition", t);
		}
	}
	while(t.GetValue() != ")");
	t = sc.GetNextToken();
	return argTable;
}

bool SymComp (Symbol* l, Symbol* r)
{
	return (l->GetName() == r->GetName() && l->GetType() == r->GetType());
}

SymProc* Parser::ParseProcDecl(string name, vector<string>* argNames, SymTable* argTable, bool isFunc, SymType* Type)
{
	SymTable* locTab = new SymTable;
	TableStack.PushTable(locTab);
	table = TableStack.GetTopTable();
	SymProc* proc = NULL;
	if (isFunc)
	{
		argTable->insert_vo(pair<string, Symbol*>("result", new SymVarParam("result", Type)));
		proc = new	SymFunc(name, argNames, argTable, NULL, false, Type);
	}
	else
		proc = new SymProc(name, argNames, argTable, NULL, false);
	proc->print(os, false);
	if (mainTable->find(name) != mainTable->end())
		mainTable->delete_element(name);
	mainTable->insert(pair<string, SymProc*> (name, proc));
	ParseDecl();
	locTab->MakeLocals();
	proc->SetBody(ParseBlock());
	proc->SetLocTab(locTab);
	TableStack.PopTable();
	TableStack.PopTable();
	table = TableStack.GetTopTable();
	return proc;
}

SymProc* Parser::ProcFunc(bool isFunc)
{
	t = sc.GetNextToken();
	SymType* Type = NULL;
	SymProc* sub = NULL;
	if (t.GetType() != identifier)
		throw Error("incorrect procedure name", t);
	SymTable* argTable = NULL;
	vector<string>* argNames = new vector<string>;
	_Table::iterator it = table->find(t.GetValue());
	string name = t.GetValue();
	t = sc.GetNextToken();
	argTable = ParseArguments(argNames);
	TableStack.PushTable(argTable);
	if (isFunc)
	{
		if (t.GetValue() != ":")
			throw Error("type expected", t);
		Type = ParseType(false);
		t = sc.GetNextToken();
	}
	t = RequireToken(";", "\";\" expected");
	if (it != table->end())
	{
		if (!(it->second->IsProc()) || !(((SymProc*)it->second)->IsForward()))
			throw Error("incorrect procedure definition", t);
		SymTable* argTableF = ((SymProc*)it->second)->GetArgTable();
		vector<string>* argNamesF = ((SymProc*)it->second)->GetArgNames();
		for (size_t i=0; i<argNames->size(); ++i)
		{
			if ((*argNames)[i] != (*argNamesF)[i] ||
				!SymComp(argTable->find((*argNames)[i])->second , argTableF->find((*argNamesF)[i])->second))
				throw Error("Header does not match previouse definition", t);
		}
		if (isFunc)
			sub = ParseProcDecl(name, argNames, argTable, true, Type);
		else
			sub = ParseProcDecl(name, argNames, argTable, false, Type);
		t = RequireToken(";", "\";\" expected");
	}
	else
	{
		if (t.GetValue() == "forward")
		{
			t = sc.GetNextToken();
			if (t.GetValue() != ";")
				throw Error("\";\" expected", t);
			if (isFunc)
			{
				sub = new SymFunc(name, argNames, argTable, NULL, true, NULL);
				((SymFunc*)sub)->SetType(Type);
			}
			else
				sub = new SymProc(name, argNames, argTable, NULL, true);
			sub->print(os, false);
			table->insert(pair<string, SymProc*> (name, sub));
			TableStack.PopTable();
		}
		else
		{
			sub = ParseProcDecl(name, argNames, argTable, isFunc, Type);
			if (isFunc)
				((SymFunc*)sub)->SetType(Type);
			sub->GetBody()->print(os, 0);
		}
		t = sc.GetNextToken();
	}
	return sub;
}

SymProc* Parser::ParseProc()
{
	return (SymProc*)ProcFunc(false);
}

SymFunc* Parser::ParseFunc()
{
	return (SymFunc*)ProcFunc(true);
}

SymType* Parser::ParseType(bool IsInTypeBlock)
{
	t = sc.GetNextToken();
	bool found = false;
	_Table::iterator it = TableStack.Find(t.GetValue(), found);
	if (found){
		if (IsInTypeBlock)
			return new SymTypeAlias(t.GetValue(), (SymType*)it->second);
		return (SymType*)it->second;
	}
	if (t.GetValue() == "record")
	{
		vector<string>* flds = new vector<string>;
		return new SymTypeRecord(IntToStr(NotExplType++), ParseRecDecl(flds), flds);
	}
	if (t.GetValue() == "array")
		return ParseArray();
	throw Error("unknown symbol in type statement", t);
}

void Parser::ParseTypeDecl()
{
	t = sc.GetNextToken();
	while (true){
		Token t1 = t;
		t = sc.GetNextToken();		
		if (t1.GetType() != identifier || t.GetValue() != "=")
			throw Error("incorrect type declaration", t);
		TableStack.Find(t1.GetValue(), t);
		SymType* Type = ParseType(true);
		Type->SetName(t1.GetValue());
		table->insert(Symb(t1.GetValue(), Type));
		Type->print(os, true);
		os << ";" << endl;
		t = sc.GetNextToken();
		if (t.GetValue() != ";")
			throw Error("\";\" expected", t);
		t = sc.GetNextToken();
		if (t.GetValue() == "var" || t.GetValue() == "const" || t.GetValue() == "procedure" || t.GetValue() == "function" 
			|| t.GetValue() == "begin" || t.GetValue() == "EOF")
			return;
		if (t.GetValue() == "type")
			t = sc.GetNextToken();
	}
}

SymVarConst* Parser::ParseConst()
{
	SymType* type = NULL;
	string Value = "";
	while (true)
	{
		if(t.GetValue() == "+" || t.GetValue() == "-")
		{
			Value = t.GetValue() + Value;
			t = sc.GetNextToken();
		}
		else
			if(t.GetType() == int_const_dec)
				return new SymVarConst(Value + t.GetValue(), Value + t.GetValue(), (SymType*)mainTable->find("integer")->second);
			else
				if (t.GetType() == float_const)
					return new SymVarConst(Value + t.GetValue(), Value + t.GetValue(), (SymType*)mainTable->find("real")->second);
				else
				{
					bool found = false;
					_Table::iterator it = TableStack.Find(t.GetValue(), found);
					if (t.GetType() != identifier)
						throw Error("wrongConst", t);
					if (!found)
						throw Error("NotConst", t);
					if (!it->second->IsConst())
						throw Error("wrongConstType",t);
					return new SymVarConst(t.GetValue(), Value + ((SymVarConst*)it->second)->GetValue(), ((SymVarConst*)it->second)->GetType());
				}
	}
}

void Parser::ParseConstDecl()
{
	t = sc.GetNextToken();
	while(true)
	{
		Token t1 = t;
		t = sc.GetNextToken();
		if (t1.GetType() != identifier || t.GetValue() != "=")
			throw Error("wrong Const Decl", t);
		TableStack.Find(t1.GetValue(), t);
		t = sc.GetNextToken();
		SymVarConst* Type = ParseConst();
		Type->SetName(t1.GetValue());
		table->insert(pair<string, SymVarConst*> (t1.GetValue(), Type));
		Type->print(os,true);
		t = sc.GetNextToken();
		t = RequireToken(";", "; expected");
		if (t.GetValue() == "var" || t.GetValue() == "type" || t.GetValue() == "procedure" || t.GetValue() == "function" 
			|| t.GetValue() == "begin" || t.GetValue() == "EOF")
			return;
		if (t.GetValue() == "const")
			t = sc.GetNextToken();
	}
}

SymTypeArray* Parser::ParseArray()
{
	t = sc.GetNextToken();
	if (t.GetValue() != "[")
		throw Error ("incorrect array declaration",t);
	vector<pair<int, int>> range;
    do
	{
		t = sc.GetNextToken();
		SymVarConst* l = ParseConst();
		t = sc.GetNextToken();
		t = RequireToken(".." , "incorrect array declaration");
		SymVarConst* r = ParseConst();
		if(!r->GetType()->IsInt() || !l->GetType()->IsInt())
			throw Error("incorrect bounds in array declaration", t);
		int ll = atoi(l->GetValue().c_str());
		int rr = atoi(r->GetValue().c_str());
		t = sc.GetNextToken();
		range.push_back(pair<int, int>(ll,rr));
	}
	while(t.GetValue() != "]");
	t =sc.GetNextToken();
	if (t.GetValue() != "of")
		throw Error("incorrect array declaration : \"of\" exprected", t);
	SymType* Type = ParseType(false);
	SymTypeArray* arrType = NULL;
	for (vector<pair<int, int>>::iterator it = range.begin(); it != range.end(); ++it)
	{
		arrType = new SymTypeArray(IntToStr(NotExplType++), Type, (*it).first, (*it).second);
		Type = arrType;
	}
	return arrType;
}

SymTable* Parser::ParseRecDecl(vector<string>* &flds)
{
	vector<string> names;
	Token t1 = sc.GetNextToken();
	t = t1;
	SymTable* fldTable = new SymTable();
	while(t.GetValue() != "end")
	{
		do
		{
			t1 = t;
			t = sc.GetNextToken();
			if (t1.GetType() != identifier && t1.GetValue() != "," && t1.GetValue() != ":")
				throw Error("incorrect record declaration", t);
			if(t1.GetType() == identifier)
			{
				Check(t1.GetValue(), fldTable);
				names.push_back(t1.GetValue());
			}
		}
		while(t.GetValue() != ":");
		SymType* type = ParseType(false);
		for (vector<string>::iterator it = names.begin(); it != names.end(); ++it)
		{
			if (fldTable->find(*it) != fldTable->end())
				throw Error("incorrect record declaration", t);
			fldTable->insert(SymElem (*it, new SymVarLocal(*it, type)));
			flds->push_back((*it));
		}
		names.clear();
		t = sc.GetNextToken();
		t = RequireToken(";" , "\";\" expected");
	}
	return fldTable;
}

void Parser::ParseVarDecl()
{
	t = sc.GetNextToken();
	while (true){
		vector<string> names;
		do
		{
			Token t1 = t;
			t = sc.GetNextToken();
			if (t1.GetType() != identifier && t1.GetValue() != "," && t1.GetValue() != ":")
				throw Error("incorrect variable declaration", t);
			if (t1.GetType() == identifier)
				names.push_back(t1.GetValue());
		}
		while(t.GetValue() != ":");
		SymType* Type = ParseType(false);
		for (vector<string>::iterator it = names.begin(); it != names.end(); ++it)
		{
			TableStack.Find((*it), t);
			SymVarGlobal* s = new SymVarGlobal(*it, Type);
			table->insert(pair<string, SymVarGlobal*>(*it, s));
			s->print(os, IsExplType(Type->GetName()));
			os<<endl;
		}
		names.clear();
		t = sc.GetNextToken();
		t = RequireToken(";" , "\";\" expected");
		if (t.GetValue() == "type" || t.GetValue() == "const" || t.GetValue() == "procedure" || t.GetValue() == "function" 
			|| t.GetValue() == "begin" || t.GetValue() == "EOF")
			return;
		if (t.GetValue() == "var")
			t = sc.GetNextToken();
	}
}

void Parser::ParseDecl()
{
	while (true)
	{
		if (t.GetValue() == "var")
			ParseVarDecl();
		else
		if(t.GetValue() == "type")
			ParseTypeDecl();
		else
		if(t.GetValue() == "const")
			ParseConstDecl();
		else
		if(t.GetValue() == "procedure")
			ParseProc();
		else
		if(t.GetValue() == "function")
			ParseFunc();
		else
			break;
	}
}

void Parser::ParseDeclaration()
{
	t = sc.GetNextToken();
	ParseDecl();
}
//~~~~~Declaration_Parsing~~~~~~~~~~~

SymType* GetRootType(SymType* sym)
{
	while (sym->IsAlias())
		sym = ((SymTypeAlias*)sym)->GetRefType();
	return sym;
}


void CheckTypes(NodeExpression* &l, NodeExpression* &r, bool IsOneSide)
{
	SymType* left = l->GetType();
	SymType* right = r->GetType();
	if (!(left->IsScalar() && right->IsScalar()))	
	{
		if (GetRootType(left) != GetRootType(right))
			throw Error("impossible cast types", t);
	}
	else
	if (IsOneSide)
	{
		if (left->IsInt() && right->IsReal())				
			throw Error("impossible cast types", t);
		if (!(left->IsInt() && right->IsInt()) && !(left->IsReal() && right->IsReal()))
		{
			r = new NodeIntToReal(r);
			r->SetType(GetRootType(left));
		}
	}
	else
	{
		if (!(left->IsInt() && right->IsInt()) && !(left->IsReal() && right->IsReal()))
		{
			if (left->IsReal() && right->IsInt())
			{
				r = new NodeIntToReal(r);
				r->SetType(GetRootType(left));
			}
			else
			{
				l = new NodeIntToReal(l);
				l->SetType(GetRootType(right));
			}
		}
	}
}

NodeCall* Parser::ParseSub(SymProc* sym)
{
	vector<NodeExpression*> params;
	int size = sym->GetArgNames()->size();
	if (size != 0)
	{
		for (int i=0; i<size && t.GetValue() != ")"; ++i)
		{
			t = sc.GetNextToken();
			NodeExpression* p = ParseExpression();
			SymTable* st = sym->GetArgTable();
			SymVarParam* h = (SymVarParam*)(st->find((*(sym->GetArgNames()))[i])->second);
			NodeExpression* help = new NodeExpression(h);
			help->SetType(h->GetType());
			if (p->GetType()->IsScalar() && h->GetType()->IsScalar())
				CheckTypes(help, p, true);
			else
				if (GetRootType(p->GetType()) != GetRootType(help->GetType()))
					throw Error("impossible cast types", t);
			
			params.push_back(p);
		}
		t = RequireToken(")" , "\")\" expected");
	}
	if (params.size() != size)
		throw Error("incorrect number of parameters", t);
	NodeCall* func = new NodeCall(sym, params);
	if (sym->IsFunc())
		func->SetType(sym->GetType());
	return func;
}

NodeRecordAccess* Parser::ParseRec(NodeExpression* r, SymVar* &name)
{
	SymType* Type = name->GetType();
	if (t.GetValue() == ".")
		while (true)
		{	
			SymTable* tab = ((SymTypeRecord*)(name->GetType()))->GetFields();
			if (t.GetValue() == ".")
				t = sc.GetNextToken();
			_Table::const_iterator _it = tab->find(t.GetValue());
			if (_it == tab->end())
				throw Error("unknown identifier", t);
			name = (SymVar*)(_it->second);
			r = new NodeRecordAccess(new Symbol("."), r, new NodeVar(name));	
			t = sc.GetNextToken();
			if (t.GetValue() == ".")
			{
				Type = name->GetType();
				if (!Type->IsRec())
					throw Error("invalid record access", t);
			}
			else
			{
				Type = name->GetType();
				break;
			}
		}
	r->SetType(Type);
	return (NodeRecordAccess*)r;
}

NodeArrayAccess* Parser::ParseArr(NodeExpression* r, SymVar* &name)
{
	SymType* Type = name->GetType();
	string arrName = t.GetValue();
	if (t.GetValue() == "[")
	{
		while (t.GetValue() != "]")
		{
			t = sc.GetNextToken();
			NodeExpression* index = ParseExpression();
			if (!index->GetType()->IsInt())
				throw Error("not integer index in array declaration", t);
			r = new NodeArrayAccess(new Symbol("[]"), r, index);
			//Type = ((SymTypeArray*)Type)->GetElType();
			Type = Type->GetElemType();
			r->SetType(Type);
			if (t.GetValue() == ",")
			{
				if (!Type->IsArr())
					throw Error("invalid number of indexes", t);
				if (t.GetValue() == "]")
					throw Error("Unexpected \"]\"", t);
			}
		}
		name = new SymVar("some", Type);
		t = sc.GetNextToken();
	}
	//r->SetType(Type);
	return (NodeArrayAccess*)r;
}


NodeExpression* Parser::ParseFactor()
{
	NodeExpression* r = NULL;
	if (t.GetValue() == "-" || t.GetValue() == "+" || t.GetValue() == "not")
	{
		string op = t.GetValue();
		t = sc.GetNextToken();
		NodeExpression* _r = ParseFactor();
		r = new NodeUnaryOp(new Symbol(op), _r);
		r->SetType(_r->GetType());
	}
	else
	{
		if (t.GetValue() != "(" && t.GetType() != identifier && t.GetType() != int_const_dec && t.GetType() != float_const)
			throw Error("Syntax error : unexpected \""+t.GetValue()+"\";", t);
		if(t.GetValue() == "(")
		{
			t = sc.GetNextToken();
			r = ParseComparision();
			t = RequireToken(")" , "\")\" expected");
		}
		switch(t.GetType()){
			case int_const_dec : case float_const:
			{
				SymVarConst* _const = new SymVarConst(t.GetValue(), t.GetValue(), NULL);
				if (t.GetType() == int_const_dec)
					_const->SetType((SymType*)mainTable->find("integer")->second);
				else
					_const->SetType((SymType*)mainTable->find("real")->second);
				t = sc.GetNextToken();
				r = new NodeConst(_const);
				r->SetType(_const->GetType());
				return r;
				break;
			}
			case identifier:
			{
				Token t1 = t;
				bool found = false;
				_Table::iterator it = TableStack.Find(t1.GetValue(), found);
				if (!found)
					throw Error("unknown identifier", t);
				t = sc.GetNextToken();
				SymVar* name = (SymVar*)it->second;
				SymType* Type = name->GetType();
				if (t.GetValue() == "(")
				{
					if (!it->second->IsFunc())
						throw Error("unexpected identifier", t);
					if (((SymFunc*)it->second)->IsForward())
						throw Error("undefined forward ("+it->second->GetName()+")", t);
					r = ParseSub((SymFunc*)it->second);
				}
				else
					if(name->GetType()->IsRec() || name->GetType()->IsArr())
					{
						r = new NodeVar(name);
						r->SetType(Type);
						while(name->GetType()->IsRec() || name->GetType()->IsArr())
						{
							SymVar* name2 = name;
							if (name->GetType()->IsRec())
								r = ParseRec(r, name);
							if (name->GetType()->IsArr())
								r = ParseArr(r,name);
							if (name2 == name)
								break;
						}
					}
					else
					{
						if (name->IsConst())
							r = new NodeConst(it->second);
						else
							r = new NodeVar(it->second);
						r->SetType(Type);
					}
				break;
			}
		}
	}
	return r;
}


//**************************************
NodeExpression* Parser::ParseTerm()
{
	NodeExpression* r = ParseFactor();
	while(t.GetValue() == "*" || t.GetValue() == "/" || t.GetValue() == "div"|| t.GetValue() == "mod" 
		  || t.GetValue() == "and" || t.GetValue()== "xor")
	{
		Token _t = t;
		string op = t.GetValue();
		t = sc.GetNextToken();
		NodeExpression* rr = ParseFactor();
		if (_t.GetValue() == "/")
		{
			if(!(r->GetType()->IsScalar() && rr->GetType()->IsScalar()))
				throw Error("type mismatch", _t);
			if(!r->GetType()->IsReal())
			{
				r = new NodeIntToReal(r);
				r->SetType((SymType*)(table->find("real")->second));
			}
			//r = new NodeUnaryOp(new Symbol("(Real)"), r);
			//r->SetType((SymType*)(table->find("real")->second));
			if(!rr->GetType()->IsReal())
			{
				rr = new NodeIntToReal(rr);
				rr->SetType((SymType*)(table->find("real")->second));
			}
			//rr = new NodeUnaryOp(new Symbol("(Real)"), rr);
			
		}
		else
			if (t.GetValue() == "div"|| t.GetValue() == "mod" || t.GetValue() == "and" || t.GetValue()== "xor")
			{
				if (!r->GetType()->IsInt() || !rr->GetType()->IsInt())
					throw Error("type mismatch", t);
			}
			else
				CheckTypes(r, rr, false);
		r = new NodeBinaryOp(new Symbol(op), r, rr);
		r->SetType(rr->GetType());
	}
	return r;
}
NodeExpression* Parser::ParseExpression()
 {
	NodeExpression* r = ParseTerm();
	while(t.GetValue() == "+" || t.GetValue() == "-" || t.GetValue() == "or")
	{
		Token _t = t;
		string op = t.GetValue();
		t = sc.GetNextToken();
		NodeExpression* rr = ParseTerm();
		if (_t.GetValue() == "or")
		{
			if (!r->GetType()->IsInt() || !rr->GetType()->IsInt())
				throw Error("type mismatch", _t);
		}
		else
			CheckTypes(r, rr, false);
		r = new NodeBinaryOp(new Symbol(op), r, rr);
		r->SetType(rr->GetType());
	}
	return r;
}

NodeExpression* Parser::ParseComparision()
{
	NodeExpression* r = ParseExpression();
	while(t.GetValue() == "<" || t.GetValue() == "<=" || t.GetValue() == ">" || t.GetValue() == ">=" || t.GetValue() == "=" || t.GetValue() == "<>")
	{
		Token _t = t;
		string op = t.GetValue();
		t = sc.GetNextToken();
		NodeExpression* rr = ParseExpression();
		CheckTypes(r, rr, false);
		r = new NodeBinaryOp(new Symbol(op), r, rr);
		r->SetType((SymType*)((mainTable->find("integer"))->second));
	}
	return r;
}

StatementAssign* Parser::ParseAssignment()
{
	NodeExpression* left = ParseExpression();
	if (!left->IsLValue())
		throw Error("incorrect assign operation", t);
	t = RequireToken(":=" , "\":=\" expected");
	NodeExpression* right = ParseComparision();
	CheckTypes(left, right, true);
	return new StatementAssign(left, right);
}

NodeStatement* Parser::ParseElement()
{
	if (t.GetValue() == "begin")
		return ParseBlock();
	if (t.GetValue() == "if") 
		return ParseIf();
	if (t.GetValue() == "while")
		return ParseWhile();
	if (t.GetValue() == "repeat")
		return ParseRepeat();
	if (t.GetValue() == "for")
		return ParseFor();
	if (t.GetValue() == "write")
		return ParseWrite(true, false);
	if (t.GetValue() == "writeln")
		return ParseWrite(true, true);
	if (t.GetValue() == "exit")
	{
		t = sc.GetNextToken();
		return new StatementExit(true);
	}
	if (t.GetType() == identifier)
	{
		bool found = false;
		_Table::iterator it = TableStack.Find(t.GetValue(), found);
		if (found)
		{
			if (it->second->IsProc() || it->second->IsFunc())
			{
				t = sc.GetNextToken();
				return ParseSub((SymProc*)it->second);
			}
		}
		return ParseAssignment();
	}
	if (t.GetValue() == "(")
	{
		return ParseAssignment();
	}
	throw Error("Statement error", t);
}

NodeWrite* Parser::ParseWrite(bool isFirst, bool isWriteln)
{
	NodeWrite* ans = NULL;
	t = sc.GetNextToken();
	if (isFirst)
		t = RequireToken("(", "\"(\" expected");
	if (t.GetType() == string_const)
	{
		ans = new NodeWriteStr(STR, "\""+t.GetValue().substr(1, t.GetValue().size()-2)+"\"");
		t = sc.GetNextToken();
	}
	else
	{
		NodeExpression* exp = ParseComparision();
		OutputType T;
		if (exp->GetType()->IsInt())
			T = INT;
		else
			T = FLOAT;
		ans = new NodeWriteExp(T, exp);
	}
	if (t.GetValue() == ",")
		ans->SetNext(ParseWrite(false, isWriteln));
	else
	{
		t = RequireToken(")", "\")\" expected");
		if (isWriteln)
			ans->SetNext(new NodeWriteln());
	}
	return ans;
}

StatementBlock* Parser::ParseBlock()
{
	block StmsList;
	t = sc.GetNextToken();
	while (t.GetValue() != "end")
	{
		StmsList.push_back(ParseElement());
		if (t.GetValue() != "end")
			t = RequireToken(";", "\";\" expected");
	}
	t = sc.GetNextToken();
	return new StatementBlock(StmsList);
}

StatementIf* Parser::ParseIf()
{
	NodeExpression* con = NULL;
	NodeStatement* then_block = NULL;
	NodeStatement* else_block = NULL;
	t = sc.GetNextToken();
	con = ParseComparision();
	if (!con->GetType()->IsInt())
		throw Error("Ordinal expression expected", t);
	t = RequireToken("then", "\"then\" expected");
	then_block = ParseElement();
	if (t.GetValue() != "else")
		return new StatementIf(con, then_block);
	t = sc.GetNextToken();
	else_block = ParseElement();
	return new StatementIfElse(con, then_block, else_block);
}

StatementWhile* Parser::ParseWhile()
{
	NodeExpression* expr = NULL;
	NodeStatement* body = NULL;
	t = sc.GetNextToken();
	expr = ParseComparision();
	if (!expr->GetType()->IsInt())
		throw Error("Ordinal expression expected", t);
	t = RequireToken("do" , "\"do\" expected");
	body = ParseElement();
	return new StatementWhile(expr, body);
}

StatementRepeat* Parser::ParseRepeat()
{
	NodeExpression* expr = NULL;
	NodeStatement* body = NULL;
	block StmsList;
	t = sc.GetNextToken();
	while (t.GetValue() != "until")
	{
		StmsList.push_back(ParseElement());
		if (t.GetValue() != "until")
			t = RequireToken(";", "statement error");
	}
	body = new StatementBlock(StmsList);
	t = sc.GetNextToken();
	expr = ParseComparision();
	if (!expr->GetType()->IsInt())
		throw Error("Ordinal expression expected", t);
	return new StatementRepeat(expr, body);
}

StatementFor* Parser::ParseFor()
{
	NodeExpression* expr;
	NodeStatement* body;
	StatementAssign* assign;
	bool isDownto = false;
	t = sc.GetNextToken();
	assign = ParseAssignment();
	if (!assign->GetType()->IsInt())
		throw Error("Ordinal expression expected", t);
	if (t.GetValue() == "downto")
		isDownto = true;
	else
		if (t.GetValue() != "to")
			throw Error("\"to\" or \"downto\" expected", t);
	t = sc.GetNextToken();
	expr = ParseComparision();
	if (!expr->GetType()->IsInt())
		throw Error("ordinal expression expected", t);
	t = RequireToken("do", "\"do\" expected");
	body = ParseElement();
	return new StatementFor(expr, body, assign, isDownto);
}
NodeStatement* Parser::ParseCode()
{
	NodeStatement* ret = NULL;
	ret = ParseBlock();
	if (t.GetValue() != ".")
		throw Error("\".\" expected", t);
	return ret;
}

AsmCode* Parser::GenerateCode()
{
	ParseDeclaration();
	NodeStatement* st = ParseCode();
	vector<pair<string, int>> onlyVars;
	for(vector<pair<string, int>>::iterator it = mainTable->get_var()->begin(); it != mainTable->get_var()->end(); ++it)
	{
		if(!((*mainTable->find(it->first)).second)->IsConst())
			onlyVars.push_back(pair<string, int> (it->first, ((*mainTable->find(it->first)).second)->GetType()->GetSize()));
	}
	Code->GenerateData(&onlyVars);
	st->Generate(Code);
	return Code;
}