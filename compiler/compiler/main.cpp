#include <fstream>
#include <iostream>
#include "parser.h"

char* types[11] = {"identifier" , "keyword", "operation" , "separator" , "int const dec" , "int const hex",
"int const oct" , "float const" ,"string const", "end of file", "wrong token" };

void PrintFile(ifstream &is)
{
	string s;
	char c[1000000];
	while (!is.eof())
	{
		is.getline(c,10000);
		s = c;
		cout<<s<<endl;
	}
	return;
}

string GetOut(char* _str, string type)
{
	size_t i;
	string str = _str;
	for (i = str.size() -1; i>=0; --i)
		if (str[i] == '.')
			break;
	return str.substr(0, i) + type;
}

int main(int argc, char* argv[])
{
	string outname;
	if (argc==1)
	{
		cout<<"Valeev Dmitriy, 236 group"<<endl;
		cout<<"-lex to scan"<<endl;
		cout<<"-dec to parse declarations"<<endl;
		cout<<"-syn to build syntax tree"<<endl;
		return 0;		
	}
	ofstream os("str.hlp");
    if (argc == 2)
	{
		Scanner sc(argv[1]);
		outname = GetOut(argv[1],".asm");
		ofstream oss(outname.c_str());
	    try
		{
			Parser p(sc,os);
			AsmCode* code = p.GenerateCode();
			code->Print(oss);
		}
		catch(Error r)
		{
			oss << r.err <<" "<< r.X <<" "<< r.Y;
		}
		oss.close();
		return 0;
	}
	Scanner sc(argv[2]);
	string key = argv[1];
	if (key == "-lex")
	{		
		 outname = GetOut(argv[2],".lex");
		 ofstream oss(outname.c_str());
	     Token T ("", keyword, Coordinates (1,1));
			try{
				while (T.GetType() != end_of_file && T.GetType() != wrong_token)
				{
					T = sc.GetNextToken();
					string str = types[T.GetType()];
					oss<<T.GetValue()<<' '<<str<<' '<<T.GetPos().first<<' '<<T.GetPos().second<<'\n';
				}
			}
			catch (Error e)
			{
				oss << e.err << ' '<< e.X << ' '<< e.Y;
			}
		}
		else
			if (key == "-syn")
			{
				outname = GetOut(argv[2],".syn");
				ofstream oss(outname.c_str());
			    try
				{
					Parser p(sc,os);
					p.ParseDeclaration();
					NodeStatement* st = p.ParseCode();
					os<<endl<<endl;
					st->print(oss, 0);
				}
				catch(Error r)
				{
					oss << r.err <<" "<< r.X <<" "<< r.Y;
				}
			}
			else
				if (key == "-dec")
				{
					outname = GetOut(argv[2],".dec");
					ofstream oss(outname.c_str());
					try
					{
						Parser p(sc,oss);
						p.ParseDeclaration();
					}
					catch(Error r)
					{
						oss << r.err <<" "<< r.X <<" "<< r.Y;
					}
				}
	ifstream is(outname.c_str());
	PrintFile(is);
	return 0;
}