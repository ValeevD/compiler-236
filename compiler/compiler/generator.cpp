#include "generator.h"


const string StrAsmReg[] = {"al", "bl", "dl", "cl", "ax", "bx", "dx", "cx", "eax", "ebx", "ecx", "edx", "esp", "esi", "edi", "ebp"};
const string StrAsmCmd[] = {"mov", "push", "pop", "inc", "dec", "add", "sub", "idiv", "imul", "and", "or", "xor", "not", "neg", "call"
                                 , "cmp", "setg", "setge", "setl", "setle", "seta",  "setae", "setb", "setbe", "sete", "setne", "je", "jne", "ja", "jae", "jb", "jbe"
								 , "jg", "jl", "jmp", "finit" ,"fld", "fstp", "fild", "fadd", "fsub", "fmul", "fdiv", "fchs", "fcompp"
								 , "fstsw", "sahf", "fxch", "ret", "rep", "none"};

const string StrAsmSize[] = {"db", "dw", "dd"};


void AsmCode::AddData(string name, AsmDataSize size, string value)
{
	DataList.push_back(new AsmData(size, name, value));
}

void AsmCode::AddData(string name, string value)
{
	DataList.push_back(new AsmData(asm_db, name, value+" , 0"));
}

void AsmCode::AddCmd(AsmCmdName name)
{
	CmdList.push_back(new AsmCmd0(name));
}

void AsmCode::AddCmd(AsmCmdName name, AsmOperand* op)
{
	CmdList.push_back(new AsmCmd1(name, op));		
}

void AsmCode::AddLabel(AsmLabel* l)
{
	CmdList.push_back(l);
}

void AsmCode::AddCmd(AsmCmdName name, AsmLabel* l)
{
	CmdList.push_back(new AsmCmd1 (name, new AsmOpConst(l->GetName())));
}

void AsmCode::AddCmd(AsmCmdName name, string pref, AsmReg reg)
{
	CmdList.push_back(new AsmCmd1_mod(name, new AsmOpRegAddr(reg), pref));
}

void AsmCode::AddCmd(AsmCmdName name, string pref, AsmReg reg, string offset)
{
	CmdList.push_back(new AsmCmd1_mod(name, new AsmOpRegAddr(reg, offset), pref));
}

void AsmCode::AddCmd(AsmCmdName name, AsmOperand* op1, AsmOperand* op2)
{
	CmdList.push_back(new AsmCmd2(name, op1, op2));
}

void AsmCode::AddCmd(AsmCmdName name, string pref, AsmOperand* op1, AsmOperand* op2)
{
	CmdList.push_back(new AsmCmd2_mod(name, op1, op2, pref));
}

void AsmCode::AddCmd(AsmCmdName name, AsmReg reg)
{
	AsmOpReg* _reg = new AsmOpReg(reg);
	AddCmd(name, _reg);
}
void AsmCode::AddCmd(AsmCmdName name, string _const)
{
	AsmOpConst* c = new AsmOpConst(_const);
	AddCmd(name, c);
}

void AsmCode::AddCmd(AsmCmdName name, AsmReg reg1, AsmReg reg2)
{
	AsmOpReg* _reg1 = new AsmOpReg(reg1);
	AsmOpReg* _reg2 = new AsmOpReg(reg2);
	AddCmd(name, _reg1, _reg2);
}

void AsmCode::AddCmd(AsmCmdName name, AsmReg reg, AsmOperand* op)
{
	AsmOpReg* _reg = new AsmOpReg(reg);
	AddCmd(name, _reg, op);
}

void AsmCode::AddCmd(AsmCmdName name, AsmReg reg, string _const)
{
	AsmOpConst* c = new AsmOpConst(_const);
	AddCmd(name, reg, c);
}

void AsmCode::AddCmd(AsmCmdName name, string pref, AsmReg reg1, AsmReg reg2)
{
	AsmOpRegAddr* _reg1 = new AsmOpRegAddr(reg1);
	AsmOpReg* _reg2 = new AsmOpReg(reg2);
	AddCmd(name, pref, _reg1, _reg2);
}

void AsmCmd0::Print(ofstream& os)
{
	os << '\t' << StrAsmCmd[name]<< endl;
}

void AsmCmd1::Print(ofstream& os)
{
	os << '\t' << StrAsmCmd[name] << ' ';
	op->Print(os);
	os << endl;
}
void AsmCmd1_mod::Print(ofstream& os)
{
	os << '\t' << StrAsmCmd[name] << ' ' << prefix << ' ';
	op->Print(os);
	os<< endl;
}
void AsmCmd2::Print(ofstream& os)
{
	os << '\t' << StrAsmCmd[name] << ' ';
	op1->Print(os);
	os<<", ";
	op2->Print(os);
	os << endl;
}

void AsmCmd2_mod::Print(ofstream& os)
{
	os << '\t' << StrAsmCmd[name] << ' ';
	os<<prefix<<" ";
	op1->Print(os);
	os<<", ";
	op2->Print(os);
	os << endl;
}

void AsmLabel::Print(ofstream& os)
{
	os << name+":\n";
}

void AsmOpReg::Print(ofstream& os)
{
	os << StrAsmReg[reg];
}

void AsmOpRegAddr::Print(ofstream& os)
{
	os << "[" << StrAsmReg[reg] <<( offset=="0" ?"]" : offset+"]");
}

void AsmOpConst::Print(ofstream& os)
{
	os << value;
}

void AsmOpMem::Print(ofstream& os)
{
	os << "g_"+name;
}

void AsmOpMemAddr::Print(ofstream& os)
{
	os << "offset g_"+name;
}

void AsmData::Print(ofstream& os)
{
	os << '\t' << "g_"+name << ' ' << StrAsmSize[size]<< ' ' << value << endl;
}

void AsmCode::GenerateData(vector<pair<string, int>>* s)
{
	for(vector<pair<string, int>>::iterator it = s->begin(); it != s->end(); ++it)
	{
		if (it->second == 1)
			AddData(it->first, asm_dd, "0");
		else
			AddData(it->first, asm_dd, IntToStr(it->second)+" dup(0)");
	}
}

void AsmCode::AddProc(string name, AsmCode* code)
{
	ProcList.push_back(new AsmCall(name, code));
}

void AsmCode::CompleteData(vector<AsmData*> add)
{
	for(int i=0; i<add.size(); ++i)
		DataList.push_back(add[i]);
}

void AsmCode::Print(ofstream& os)
{
	os << ".686" << endl;
	os << "include \\masm32\\include\\masm32rt.inc" << endl << endl;
	os << ".data" << endl;
	AddData("format_int", "\"%d\"");
	AddData("format_real", "\"%f\"");
	AddData("new_line", "\"\\n\"");
	for (vector<AsmData*>::iterator it = DataList.begin(); it != DataList.end(); ++it)
		(*it)->Print(os);
	os << ".code" << endl;
	PrintProcList(os);
	os  << "start:" << endl;
	PrintCmdList(os);
	/*for (vector<AsmCmd*>::iterator it = CmdList.begin(); it != CmdList.end(); ++it)
		(*it)->Print(os);*/
	os <<"\tINVOKE ExitProcess, 0"<<endl;
	os << "end start";
}

void AsmCode::PrintCmdList(ofstream& os)
{
	for (vector<AsmCmd*>::iterator it = CmdList.begin(); it != CmdList.end(); ++it)
		(*it)->Print(os);
}

void AsmCode::PrintProcList(ofstream& os)
{
	for(vector<AsmCall*>::iterator it = ProcList.begin(); it != ProcList.end(); ++it)
	{
		(*it)->Print(os);
		(*it)->GetCode()->PrintProcList(os);
	}
}

void AsmCall::Print(ofstream& os)
{
	os << name + " proc\n";
	code->PrintCmdList(os);
	os << name + " endp\n\n";
}