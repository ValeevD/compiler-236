#include "SyntaxNodes.h"

string OutType[3] = {"%s" , "%d" , "%f"};

string GetSpaces (int d)
{
	string spaces = "";
	if (!d) return spaces;
	for (int i=0; i<d-4; ++i)
	{
		spaces = spaces + ' ';
	}
	spaces = spaces + "~~~~";
	return spaces;
}

string CutTildas(string s)
{
	return s.substr(0, s.length()-5) + "    ";
}

void NodeBinaryOp::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << this->sym->GetName() << '\n';
	this->l->print(os, deep+4); 
	this->r->print(os,deep+4);
}

void StatementAssign::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << ":=" << '\n';
	this->l->print(os, deep+4); 
	this->r->print(os,deep+4);
}

void StatementBlock::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os<< spaces << "[block]"<< '\n';
	for (block::const_iterator it = stms.begin(); it != stms.end(); ++it)
		(*it)->print(os, deep+4);
}

void StatementIf::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os<< spaces << "[if]"<< '\n';
	this->con->print(os,deep+4);
	spaces = spaces.substr(0,spaces.size()-4) + "    ";
	os<<spaces<< "[then]"<< '\n';
	this->then_block->print(os,deep+4);
}

void StatementIfElse::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os<< spaces << "[if]"<< '\n';
	this->con->print(os,deep+4);
	spaces = spaces.substr(0, spaces.length()-4) + "    ";
	os<<spaces<< "[then]"<< '\n';
	this->then_block->print(os,deep+4);
	os<<spaces<< "[else]"<< '\n';
	this->else_block->print(os,deep+4);
}

void StatementWhile::print(ofstream& os, int deep)
{
	string spaces = GetSpaces(deep);
	os<< spaces << "[while]"<< '\n';
	this->expr->print(os, deep+4);
	spaces = spaces.substr(0, spaces.length()-4) + "    ";
	os<< spaces << "[do]"<< '\n';
	this->body->print(os,deep+4);
}

void StatementRepeat::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os<< spaces << "[repeat]"<< '\n';
	this->body->print(os, deep+4);
	spaces = spaces.substr(0,spaces.size()-4) + "    ";
	os<<spaces<< "[until]"<< '\n';
	this->expr->print(os,deep+4);
}

void StatementFor::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os<< spaces << "[for]"<< '\n';
	this->assign->print(os, deep + 4);
	spaces = spaces.substr(0,spaces.size()-4) + "    ";
	os<< spaces <<(isDownto ? "[downto]" : "[to]") << '\n';
	this->expr->print(os, deep + 4);
	os<< spaces << "[do]"<< '\n';
	this->body->print(os, deep + 4);

}
void NodeUnaryOp::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << this->sym->GetName() << '\n';
	this->par->print(os, deep+4);
}

void NodeConst::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << this->sym->GetName() << '\n';
}

void NodeVar::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << this->sym->GetName() << '\n';
}

void NodeCall::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << this->sym->GetName() << '\n';
	for (vector<NodeExpression*>::const_iterator it = this->params.begin(); it != this->params.end(); ++it)
		(*it)->print(os, deep+4);
}

void NodeWriteStr::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << "[write]" << '\n';
	spaces = "    " + spaces;
	os << spaces << OutType[t]<< '\n' << spaces << '\"' << par << '\"' << '\n';
	if (next != NULL)
		next->print(os, deep);
}

void NodeWriteExp::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << "[write]" << '\n';
	spaces = "    " + spaces;
	os << spaces << OutType[t] << '\n';
	exp->print(os, deep+4);
	if (next != NULL)
		next->print(os, deep);
}

void NodeWriteln::print(ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << "[writeln]" << '\n';
}

void StatementExit::print(std::ofstream &os, int deep)
{
	string spaces = GetSpaces(deep);
	os << spaces << "[exit]" << '\n';
}