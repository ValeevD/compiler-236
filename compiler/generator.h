#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include "Symbols.h"

using namespace std;

enum AsmReg{
	reg_al,
	reg_bl,
	reg_dl,
	reg_cl,
	reg_ax,
	reg_bx,
	reg_dx,
	reg_cx,
	reg_eax,
	reg_ebx,
	reg_ecx,
	reg_edx,
	reg_esp,
	reg_esi,
	reg_edi,
	reg_ebp
};

enum AsmCmdName{
	asm_mov,
	asm_push,
	asm_pop,
	asm_inc,
	asm_dec,
	asm_add,
	asm_sub,
	asm_idiv,
	asm_imul,
	asm_and,
	asm_or,
	asm_xor,
	asm_not,
	asm_neg,
	asm_call,
	asm_cmp,
	asm_setg,
	asm_setge,
	asm_setl,
	asm_setle,
	asm_seta,
	asm_setae,
	asm_setb,
	asm_setbe,
	asm_sete,
	asm_setne,
	asm_je,
	asm_jne,
	asm_ja,
	asm_jae,
	asm_jb,
	asm_jbe,
	asm_jg,
	asm_jl,
	asm_jmp,
	asm_finit,
	asm_fld,
	asm_fstp,
	asm_fild,
	asm_fadd,
	asm_fsub,
	asm_fmul,
	asm_fdiv,
	asm_fchs,
	asm_fcompp,
	asm_fstsw,
	asm_sahf,
	asm_fxch,
	asm_ret,
	asm_rep,
	asm_none
};

enum AsmDataSize{
	asm_db,
	asm_dw,
	asm_dd
};

static int LabelCount = 0;

class AsmOperand;

class AsmCmd{
public:
	virtual void Print(ofstream& os){}
};

class AsmCmd0: public AsmCmd{
	AsmCmdName name;
public:
	AsmCmd0(AsmCmdName _name): name(_name) {}
	virtual void Print(ofstream& os);
};

class AsmCmd1: public AsmCmd{
protected:
	AsmCmdName name;
	AsmOperand* op;
public:
	AsmCmd1(AsmCmdName _name, AsmOperand* _op): name(_name), op(_op) {}
	virtual void Print(ofstream& os);
};

class AsmCmd1_mod: public AsmCmd1{
	string prefix;
public:
	AsmCmd1_mod(AsmCmdName _name, AsmOperand* _op, string _pref): AsmCmd1(_name,_op), prefix(_pref){}
	virtual void Print(ofstream& os);
};

class AsmCmd2: public AsmCmd{
protected:
	AsmCmdName name;
	AsmOperand *op1; 
	AsmOperand *op2;
public:
	AsmCmd2(AsmCmdName _name, AsmOperand* _op1, AsmOperand* _op2): name(_name), op1(_op1), op2(_op2) {}
	virtual void Print(ofstream& os);
};

class AsmCmd2_mod: public AsmCmd2{
	string prefix;
public:
	AsmCmd2_mod(AsmCmdName _name, AsmOperand* _op1, AsmOperand* _op2, string pref): AsmCmd2(_name,_op1,_op2), prefix(pref) {}
	virtual void Print(ofstream& os);
};

class AsmData: public AsmCmd{
	AsmDataSize size;
	string name;
	string value;
public:
	AsmData(AsmDataSize _size, string _name, string _value = "0"): size(_size), name(_name), value(_value) {}
	virtual void Print(ofstream& os);
};

class AsmLabel : public AsmCmd{
	string name;
public:
	AsmLabel(): name("label_"+IntToStr(++LabelCount)) {}
	string GetName() {return name;}
	virtual void Print(ofstream& os);
};

class AsmOperand{
public:
	virtual void Print(ofstream& os){}
	virtual bool IsReg() {return false;}
	virtual bool IsConst() {return false;}
	virtual bool IsMem() {return false;}
};

class AsmOpReg: public AsmOperand{
protected:
	AsmReg reg;
public:
	AsmOpReg(AsmReg _reg): reg(_reg) {}
	virtual void Print(ofstream& os);
	virtual bool IsReg() {return true;}
};

class AsmOpRegAddr: public AsmOpReg{
	string offset;
public:
	AsmOpRegAddr(AsmReg _reg):AsmOpReg(_reg) {}
	AsmOpRegAddr(AsmReg _reg, string _ofst): AsmOpReg(_reg), offset(_ofst) {}
	virtual void Print(ofstream& os);
};

class AsmOpConst: public AsmOperand{
	string value;
public:
	AsmOpConst(string _value): value(_value) {}
	virtual void Print(ofstream& os);
	virtual bool IsConst() {return true;}
};

class AsmOpMem: public AsmOperand{
protected:
	string name;
public:
	AsmOpMem(string _name): name(_name) {}
	virtual void Print(ofstream& os);
	virtual bool IsMem() {return true;}
};

class AsmOpMemAddr: public AsmOpMem{
public:
	AsmOpMemAddr(string _name): AsmOpMem(_name) {}
	virtual void Print(ofstream& os);
};

class AsmCall;

class AsmCode{
	vector<AsmCmd*> CmdList;
	vector<AsmData*> DataList;
	vector<AsmCall*> ProcList;
public:
	AsmCode(){CmdList.push_back(new AsmCmd0(asm_finit));}
	AsmCode(int i){}
	vector<AsmData*> GetDataList() {return DataList;}
	void CompleteData(vector<AsmData*> add);
	void AddData(string name, AsmDataSize size, string value);
	void AddProc(string name, AsmCode* code);
	void GenerateData(vector<pair<string, int>>* tab);
	void AddData(string name, string value);
	void AddLabel(AsmLabel* l);
	void AddCmd(AsmCmdName name);
	void AddCmd(AsmCmdName name, AsmOperand* op);
	void AddCmd(AsmCmdName name, AsmLabel* l);
	void AddCmd(AsmCmdName name, string pref, AsmReg reg);
	void AddCmd(AsmCmdName name, string pref, AsmReg reg, string offset);
	void AddCmd(AsmCmdName name, string _const);
	void AddCmd(AsmCmdName name, AsmOperand* op1, AsmOperand* op2);
	void AddCmd(AsmCmdName name, AsmReg reg);
	void AddCmd(AsmCmdName name, AsmReg reg1, AsmReg reg2);
	void AddCmd(AsmCmdName name, AsmReg reg1, AsmOperand* op);
	void AddCmd(AsmCmdName name, AsmReg reg1, string _const);
	void AddCmd(AsmCmdName name, string pref, AsmReg reg1, AsmReg reg2);
	void AddCmd(AsmCmdName name, string pref, AsmOperand* op1, AsmOperand* op2);
	void Print(ofstream& os);
	void PrintCmdList(ofstream& os);
	void PrintProcList(ofstream& os);
};

class AsmCall{
	string name;
	AsmCode* code;
public:
	AsmCall(string _name, AsmCode* _code): name(_name), code(_code) {}
	AsmCode* GetCode() {return code;}
	void Print(ofstream& os);
};