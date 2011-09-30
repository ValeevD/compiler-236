#include "SyntaxNodes.h"

int strCount = 0;
int realCount = 0;


void NodeUnaryOp::Generate(AsmCode* Code)
{
	par->Generate(Code);
	if (par->GetType()->IsInt())
		GenerateForInt(Code);
	if (par->GetType()->IsReal())
		GenerateForReal(Code);
}

void NodeUnaryOp::GenerateForInt(AsmCode* Code)
{
	AsmCmdName cmd = asm_none;
	string name = GetName();
	if (name == "-")
		cmd = asm_neg;
	if (name == "not")
		cmd = asm_not;
	if (cmd != asm_none)
	{
		Code->AddCmd(asm_pop, reg_eax);
		Code->AddCmd(cmd, reg_eax);
		Code->AddCmd(asm_push, reg_eax);
	}
}

void NodeUnaryOp::GenerateForReal(AsmCode* Code)
{
	string name = GetName();
	if(name == "-")
	{
		Code->AddCmd(asm_fld, "dword ptr", reg_esp);
		Code->AddCmd(asm_fchs);
		Code->AddCmd(asm_fstp, "dword ptr", reg_esp);
	}
}

void NodeIntToReal::Generate(AsmCode* Code)
{
	par->Generate(Code);
	Code->AddCmd(asm_fild, "dword ptr", reg_esp);
	Code->AddCmd(asm_fstp, "dword ptr", reg_esp);
}

void NodeBinaryOp::Generate(AsmCode* Code)
{
	l->Generate(Code);
	r->Generate(Code);
	string name = GetName();
	bool isReal = l->GetType()->IsReal() || r->GetType()->IsReal();
	if (name == ">" || name == "<" || name == ">=" || name == "<=" || name == "=" || name == "<>")
	{
		Code->AddCmd(asm_xor, reg_edx, reg_edx);
		GenerateComparison(Code, name, isReal);
	}
	else
		if (isReal)
			GenerateAriphmReal(Code, name);
		else
			GenerateAriphmInt(Code, name);
}

void NodeBinaryOp::GenerateComparison(AsmCode *Code, string name, bool isReal)
{
	AsmCmdName cmd = asm_none;
	if (!isReal)
	{
		Code->AddCmd(asm_pop, reg_ebx);
		Code->AddCmd(asm_pop, reg_eax);
		Code->AddCmd(asm_cmp, reg_eax, reg_ebx);
	}
	else
	{
		Code->AddCmd(asm_fld, "dword ptr", reg_esp);
		Code->AddCmd(asm_add, reg_esp, "4");
		Code->AddCmd(asm_fld, "dword ptr", reg_esp);
		Code->AddCmd(asm_add, reg_esp, "4");
		Code->AddCmd(asm_fcompp);
		Code->AddCmd(asm_fstsw, reg_ax);
		Code->AddCmd(asm_sahf);
	}
	if(isReal)
	{
		if (name == ">")cmd = asm_seta;
		if (name == "<")cmd = asm_setb;
		if (name == ">=")cmd = asm_setae;
		if(name == "<=")cmd = asm_setbe;
	}
	else
	{
		if (name == ">")cmd = asm_setg;
		if (name == "<")cmd = asm_setl;
		if (name == ">=")cmd = asm_setge;
		if(name == "<=")cmd = asm_setle;
	}
	if(name == "=")cmd = asm_sete;
	if(name == "<>")cmd = asm_setne;
	Code->AddCmd(cmd, reg_dl);
	Code->AddCmd(asm_push, reg_edx);
	return;
}

void NodeBinaryOp::GenerateAriphmInt(AsmCode* Code, string name)
{
	AsmCmdName cmd = asm_none;
	Code->AddCmd(asm_pop, reg_ebx);
	Code->AddCmd(asm_pop, reg_eax);
	if(name == "div" || name == "mod" || name == "*")
	{
		cmd = asm_idiv;
		Code->AddCmd(asm_xor, reg_edx, reg_edx);
		if (name == "*")
			cmd = asm_imul;
		Code->AddCmd(cmd, reg_ebx);
		if (name == "mod")
			Code->AddCmd(asm_push, reg_edx);
		else
			Code->AddCmd(asm_push, reg_eax);
	}
	else
	{
		if (name == "+")
			cmd = asm_add;
		if(name == "-")
			cmd = asm_sub;
		if(name == "or")
			cmd = asm_or;
		if(name == "and")
			cmd = asm_and;
		if(name == "xor")
			cmd = asm_xor;
		Code->AddCmd(cmd, reg_eax, reg_ebx);
		Code->AddCmd(asm_push, reg_eax);
	}
}

void NodeBinaryOp::GenerateAriphmReal(AsmCode* Code, string name)
{
	AsmCmdName cmd = asm_none;
	Code->AddCmd(asm_fld, "dword ptr", reg_esp, "+4");
	Code->AddCmd(asm_fld, "dword ptr", reg_esp);
	Code->AddCmd(asm_add, reg_esp, "4");
	if (name == "+")
		cmd = asm_fadd;
	if (name == "-")
		cmd = asm_fsub;
	if (name == "*")
		cmd = asm_fmul;
	if (name == "/")
		cmd = asm_fdiv;
	Code->AddCmd(cmd);
	Code->AddCmd(asm_fstp, "dword ptr", reg_esp);
}

void NodeWriteExp::GenerateForInt(AsmCode* Code)
{
	Code->AddCmd(asm_push, "offset g_format_int");
	Code->AddCmd(asm_call, "crt_printf");
	Code->AddCmd(asm_add, reg_esp, "8");
}

void NodeWriteExp::GenerateForReal(AsmCode* Code)
{
	Code->AddCmd(asm_fld, "dword ptr", reg_esp);
	Code->AddCmd(asm_fstp, "qword ptr", reg_esp);
	Code->AddCmd(asm_push, "offset g_format_real");
	Code->AddCmd(asm_call, "crt_printf");
	Code->AddCmd(asm_add, reg_esp, "12");
}

void NodeWriteExp::Generate(AsmCode* Code)
{
	exp->Generate(Code);
	if(t == INT)
		GenerateForInt(Code);
	if(t == FLOAT)
		GenerateForReal(Code);
	if (next != NULL)
		next->Generate(Code);
}

void NodeWriteStr::Generate(AsmCode* Code)
{
	string _str = "str"+IntToStr(++strCount);
	Code->AddData(_str, par);
	Code->AddCmd(asm_push, "offset g_"+_str);
	Code->AddCmd(asm_call, "crt_printf");
	Code->AddCmd(asm_add, reg_esp, "4");
	if (next != NULL)
		next->Generate(Code);
}

void NodeWriteln::Generate(AsmCode* Code)
{
}

void NodeVar::Generate(AsmCode* Code)
{
	VarType ty = ((SymVar*)GetSym())->GetVarType();
	switch (ty) {
		case GLOBAL : 
			{
				if (!GetType()->IsScalar())
				{
					Code->AddCmd(asm_sub, reg_esp, IntToStr(GetType()->GetSize()*4));
					Code->AddCmd(asm_mov, new AsmOpReg(reg_esi), new AsmOpMemAddr(GetName()));
					Code->AddCmd(asm_mov, reg_edi, reg_esp);
					Code->AddCmd(asm_mov, reg_ecx, IntToStr(GetType()->GetSize()));
					Code->AddCmd(asm_rep, "movsd");
				}
				else
					Code->AddCmd(asm_push, new AsmOpMem(GetName()));				
				return;
			}
		case PARAM_VAR:
			{
				Code->AddCmd(asm_mov, reg_eax, new AsmOpRegAddr(reg_ebp, "+" + IntToStr((((SymVar*)GetSym())->GetOffset()+2)*4)));
				Code->AddCmd(asm_push, new AsmOpRegAddr(reg_eax));
				return;
			}
		case PARAM :
			{
				if (!GetType()->IsScalar())
				{
					Code->AddCmd(asm_sub, reg_esp, IntToStr(GetType()->GetSize()*4));
					Code->AddCmd(asm_mov, reg_esi, IntToStr(((((SymVar*)GetSym())->GetOffset())+2)*4));
					Code->AddCmd(asm_add, reg_esi, reg_ebp);
					Code->AddCmd(asm_mov, reg_edi, reg_esp);
					Code->AddCmd(asm_mov, reg_ecx, IntToStr(GetType()->GetSize()));
					Code->AddCmd(asm_rep, "movsd");
				}
				else
					Code->AddCmd(asm_push, new AsmOpRegAddr(reg_ebp, "+" + IntToStr(((((SymVar*)GetSym())->GetOffset())+2)*4))); 
				return;
			}
		case LOCAL :
			Code->AddCmd(asm_push, new AsmOpRegAddr(reg_ebp, "-" + IntToStr(((((SymVar*)GetSym())->GetOffset())+1)*4))); return;
	}
}

void NodeVar::GenerateLValue(AsmCode* Code)
{
	VarType ty = ((SymVar*)GetSym())->GetVarType();
	switch (ty){
		case GLOBAL : Code->AddCmd(asm_push, new AsmOpMemAddr(GetName())); return;
		case PARAM_VAR : Code->AddCmd(asm_push, new AsmOpRegAddr(reg_ebp, "+" + IntToStr(((((SymVar*)GetSym())->GetOffset())+2)*4))); return;
		case PARAM : Code->AddCmd(asm_mov, reg_eax, IntToStr(((((SymVar*)GetSym())->GetOffset())+2)*4)); break;
		case LOCAL : Code->AddCmd(asm_mov, reg_eax, IntToStr(((((SymVar*)GetSym())->GetOffset())+1)*(-4))); break;
	}
	Code->AddCmd(asm_add, reg_eax, reg_ebp);
	Code->AddCmd(asm_push, reg_eax);
}

void NodeConst::Generate(AsmCode* Code)
{
	if(type->IsInt())
		GenerateForInt(Code);
	if(type->IsReal())
		GenerateForReal(Code);
}

void NodeConst::GenerateForReal(AsmCode* code)
{
	string fl = "real" + IntToStr(++realCount);
	code->AddData(fl, asm_dd, ((SymVarConst*)sym)->GetValue());
	code->AddCmd(asm_push, new AsmOpMem(fl));
}

void NodeConst::GenerateForInt(AsmCode* code)
{
	code->AddCmd(asm_push, ((NodeConst*)this)->GetValue());
}

void StatementAssign::Generate(AsmCode* Code)
{
	r->Generate(Code);
	((NodeVar*)l)->GenerateLValue(Code);
	if (!r->GetType()->IsScalar())
	{
		Code->AddCmd(asm_pop, reg_edi);
		Code->AddCmd(asm_mov, reg_esi, reg_esp);
		Code->AddCmd(asm_push, reg_ecx);
		Code->AddCmd(asm_mov, reg_ecx, IntToStr(r->GetType()->GetSize()));
		Code->AddCmd(asm_rep, "movsd");
		Code->AddCmd(asm_pop, reg_ecx);
		Code->AddCmd(asm_add, reg_esp, IntToStr(r->GetType()->GetSize()*4));
		return;
	}
	Code->AddCmd(asm_pop, reg_ebx);
	Code->AddCmd(asm_pop, reg_eax);
	Code->AddCmd(asm_mov, "dword ptr", reg_ebx, reg_eax);
}

void NodeArrayAccess::GenerateBase(AsmCode* Code)
{
	((NodeVar*)l)->GenerateLValue(Code);
	r->Generate(Code);
	Code->AddCmd(asm_pop, reg_ebx);
	Code->AddCmd(asm_mov, reg_eax, IntToStr(((SymTypeArray*)l->GetType())->GetLeftBound()));
	Code->AddCmd(asm_sub, reg_ebx, reg_eax);
	Code->AddCmd(asm_imul, reg_ebx, IntToStr(GetType()->GetSize()*4));
	Code->AddCmd(asm_pop, reg_esi);
	Code->AddCmd(asm_add, reg_esi, reg_ebx);
}

void NodeArrayAccess::Generate(AsmCode* Code)
{
	GenerateBase(Code);
	Code->AddCmd(asm_push, "dword ptr", reg_esi);
}

void NodeArrayAccess::GenerateLValue(AsmCode* Code)
{
	GenerateBase(Code);
	Code->AddCmd(asm_push, reg_esi);
}

void NodeRecordAccess::Generate(AsmCode* Code)
{
	GenerateBase(Code);
	Code->AddCmd(asm_push, "dword ptr", reg_esi);
}

void NodeRecordAccess::GenerateLValue(AsmCode* Code)
{
	GenerateBase(Code);
	Code->AddCmd(asm_push, reg_esi);
}

void NodeRecordAccess::GenerateBase(AsmCode* Code)
{
	((NodeVar*)l)->GenerateLValue(Code);
	Code->AddCmd(asm_pop, reg_esi);
	Code->AddCmd(asm_add, reg_esi , IntToStr(((SymTypeRecord*)l->GetType())->GetOffsetByName(r->GetName())*4));
}

pair<AsmLabel*,AsmLabel*> StatementIf::GenerateIf(AsmCode* Code)
{
	AsmLabel* l1 = new AsmLabel();
	AsmLabel* l2 = new AsmLabel();
	con->Generate(Code);
	Code->AddCmd(asm_pop, reg_eax);
	Code->AddCmd(asm_cmp, reg_eax, "0");
	Code->AddCmd(asm_je, l1);
	then_block->Generate(Code);
	return pair<AsmLabel*,AsmLabel*> (l1, l2);
}

void StatementIf::Generate(AsmCode* Code)
{
	pair<AsmLabel*,AsmLabel*> p = GenerateIf(Code);
	Code->AddLabel(p.first);
}

void StatementIfElse::Generate(AsmCode* Code)
{
	pair<AsmLabel*,AsmLabel*> p = GenerateIf(Code);
	Code->AddCmd(asm_jmp, p.second);
	Code->AddLabel(p.first);
	else_block->Generate(Code);
	Code->AddLabel(p.second);
}
void StatementWhile::Generate(AsmCode* Code)
{
	AsmLabel* l1 = new AsmLabel();
	AsmLabel* l2 = new AsmLabel();
	Code->AddLabel(l1);
	expr->Generate(Code);
	Code->AddCmd(asm_pop, reg_eax);
	Code->AddCmd(asm_cmp, reg_eax, "0");
	Code->AddCmd(asm_je, l2);
	body->Generate(Code);
	Code->AddCmd(asm_jmp, l1);
	Code->AddLabel(l2);
}

void StatementRepeat::Generate(AsmCode* Code)
{
	AsmLabel* l1 = new AsmLabel();
	Code->AddLabel(l1);
	body->Generate(Code);
	expr->Generate(Code);
	Code->AddCmd(asm_pop, reg_eax);
	Code->AddCmd(asm_cmp, reg_eax, "0");
	Code->AddCmd(asm_je, l1);
}

void StatementFor::Generate(AsmCode* Code)
{
	AsmLabel* l1 = new AsmLabel();
	AsmLabel* l2 = new AsmLabel();
	assign->Generate(Code);
	AsmCmdName cmd1 = asm_inc, cmd2 = asm_jg;
	if (isDownto)
	{
		cmd1 = asm_dec; 
		cmd2 = asm_jl;
	}
	expr->Generate(Code);
	Code->AddCmd(asm_pop, reg_ebx);
	assign->GetLeft()->Generate(Code);
	Code->AddCmd(asm_pop, reg_ecx);
	Code->AddLabel(l1);
	Code->AddCmd(asm_cmp, reg_ecx, reg_ebx);
	Code->AddCmd(cmd2, l2);
	assign->GetLeft()->GenerateLValue(Code);
	Code->AddCmd(asm_pop, reg_eax);
	Code->AddCmd(asm_mov, "dword ptr", reg_eax, reg_ecx);
	Code->AddCmd(asm_push, reg_ebx);
	Code->AddCmd(asm_push, reg_ecx);
	body->Generate(Code);
	Code->AddCmd(asm_pop, reg_ecx);
	Code->AddCmd(asm_pop, reg_ebx);
	Code->AddCmd(cmd1, reg_ecx);
	Code->AddCmd(asm_jmp, l1);
	Code->AddLabel(l2);	
}

void StatementBlock::Generate(AsmCode* code)
{
	for (block::iterator it = stms.begin(); it != stms.end(); ++it)
		(*it)->Generate(code);
}

void NodeCall::Generate(AsmCode* Code)
{
	SymTable* args = ((SymProc*)GetSym())->GetArgTable();
	SymTable* loc = ((SymProc*)GetSym())->GetLocals();
	if(!((SymProc*)GetSym())->IsTranslated())
	{
		((SymProc*)GetSym())->SetTranslated();
		AsmCode* pCode = new AsmCode(0);
		pCode->AddCmd(asm_push, reg_ebp);
		pCode->AddCmd(asm_mov, reg_ebp, reg_esp);
		pCode->AddCmd(asm_sub, reg_esp, IntToStr(loc->GetOffset()*4));
		((SymProc*)GetSym())->GetBody()->Generate(pCode);
		pCode->AddCmd(asm_mov, reg_esp, reg_ebp);
		pCode->AddCmd(asm_pop, reg_ebp);
		pCode->AddCmd(asm_ret);
		Code->AddProc(GetName(), pCode);
		Code->CompleteData(pCode->GetDataList());
	}
	vector<string>::iterator it = ((SymProc*)GetSym())->GetArgNames()->end();
	--it;
	if (IsFunc()) 
		Code->AddCmd(asm_sub, reg_esp , IntToStr(GetType()->GetSize()*4));
	for(int i = params.size()-1; i>=0; --i)
	{
		if((args->find(*it)->second)->IsParamVar())
			params[i]->GenerateLValue(Code);
		else
			params[i]->Generate(Code);
		if (i!=0)--it;
	}
	Code->AddCmd(asm_call, GetSym()->GetName());
	Code->AddCmd(asm_add, reg_esp, IntToStr(args->GetOffset()*4));
}

void StatementExit::Generate(AsmCode* Code)
{
	Code->AddCmd(asm_mov, reg_esp, reg_ebp);
	Code->AddCmd(asm_pop, reg_ebp);
	Code->AddCmd(asm_ret);
}