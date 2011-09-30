#include "scanner.h"
#include "Symbols.h"
#include "SyntaxNodes.h"

using namespace std;

class Parser{
	SymTableStack TableStack;
	Scanner& sc;
	SymTable* table;
	SymTable* mainTable;
	SyntaxNode* Node;
	AsmCode* Code;
	void ParseVarDecl();
	void ParseTypeDecl();
	void ParseConstDecl();
	SymProc* ParseProc();
	SymFunc* ParseFunc();
	SymProc* ProcFunc(bool isFunc);
	SymTable* ParseArguments(vector<string>* &arguments);
	SymTable* ParseRecDecl(vector<string>* &flds);
	SymType* ParseType(bool IsInTypeBlock);
	SymTypeArray* ParseArray();
	SymVarConst* ParseConst();
	StatementBlock* ParseBlock();
	NodeStatement* ParseElement();
	StatementIf* ParseIf();
	StatementWhile* ParseWhile();
	StatementRepeat* ParseRepeat();
	StatementFor* ParseFor();
	StatementAssign* ParseAssignment();
	NodeWrite* ParseWrite(bool isFirst, bool isWriteln);
	NodeExpression* ParseComparision();
	NodeExpression* ParseExpression();
	NodeExpression* ParseTerm();
	NodeExpression* ParseFactor();
	NodeArrayAccess* ParseArr(NodeExpression* r, SymVar* &name);
	NodeCall* ParseSub(SymProc* sym);
	NodeRecordAccess* ParseRec(NodeExpression* r, SymVar* &name);
	Token RequireToken(string _t, string err);
	void ParseDecl();
	SymProc* ParseProcDecl(string name, vector<string>* argNames, SymTable* argTable, bool isFunc, SymType* Type);
	ofstream& os;
public:
	Parser(Scanner& _sc, ofstream& _os): sc(_sc), os(_os)
	{
		SymTypeInteger *i = new SymTypeInteger("integer");
		SymTypeFloat *r = new SymTypeFloat("real");
		Code = new AsmCode();
		table = new SymTable;
		table->insert(pair<string, SymType*> ("integer", new SymTypeInteger("integer"))); 
		table->insert(pair<string, SymType*> ("real", new SymTypeFloat("real")));
		TableStack.PushTable(table);
		mainTable = table;
	}
	void ParseDeclaration();
	AsmCode* GenerateCode();
	NodeStatement* ParseCode();
};

