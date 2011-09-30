#include "scanner.h"


map<string, TokenType> SpecSymb;
string diff = "><:=+-*/,; :().'[]{}";
Coordinates pos;

string IntToStr(int x)
{
	strstream st;
	st << x;
	string str;
	st >> str;
	return str;
}

bool IsSep(char c)
{
	for (size_t i=0; i<diff.length()-1; ++i)
		if (c == diff[i])
			return true;
	return false;
}

void Filling (char* file_name, TokenType T)
{
	ifstream is (file_name);
	string str;
	while (!is.eof())
	{
		is>>str;
		SpecSymb.insert(pair<string, TokenType> (str, T));
	}
}

Scanner::Scanner (char* file_name)
{
	file.open(file_name);
	Filling("keywords.txt" , keyword);
	Filling("operations.txt" , operation);
	Filling("separators.txt", separator);
	xy.first=xy.second=1;
}

State GetState (char c)
{
	if ((c >= 'a' && 'z' >= c) || (c >= 'A' && 'Z' >= c))
		return is_letter;
	if (c >= '0' && '9' >= c)
		return is_digit;
	switch (c){
		case '$' : return is_dollar;
		case '&' : return is_ampersand;
		case '#' : return is_sharp;
		case '\'': return is_quote;
		case '_' : return is_emphasize;
		case '-' : return is_minus;
		case '+' : return is_plus;
		case '*' : return is_star;
		case '/' : return is_slash;
		case '.' : return is_point;
		case ':' : return is_colon;
		case '<' : return is_less;
		case '>' : return is_more;
		case '(' : return is_round_bracket;
		case '{' : return is_figure_bracket;
		case '[' : return is_square_bracket;
		case ' ' : return is_space;
		case '\n': return is_end_of_line;
		case EOF : return is_end_of_file;
	}
	return none;
}

char GetSymb (istream& is, Coordinates& xy)
{
	char c;
	c = is.peek();
	is.ignore();
	if (c == '\n')
	{
		xy.first = 1;
		++xy.second;
	}
	else
		++xy.first;
	if (c >= 'A' && 'Z' >= c)
		return c + ('a' - 'A');
	return c;
}

bool SkipSpaces (istream& is, Coordinates& xy, Coordinates& pos)
{
	while (is.peek() == ' ' || is.peek() == '\n' || is.peek() == '\t' || is.peek() == '{')
	{
		if (is.peek() == '{')
		{
			char c='{';
			pos = xy;
			while (c != '}')
			{
				c = GetSymb(is, xy);
				if (c == EOF)
					return false;
			}
		}
		if (is.peek() == '\n')
		{
			xy.first = 1;
			++xy.second;
		}
		else
			if (is.peek() == '\t')
				xy.first += 8;
			else
				++xy.first;
		is.ignore();
	}
	return true;
}

bool IsSyntaxError (State st)
{
	return st == is_dollar || st == is_ampersand || st == is_sharp || st == is_quote;
}

TokenType ReadIdentifier(istream& is, string& str, Coordinates& xy)
{
	char c = is.peek();
	State st = GetState(is.peek());
	while (st == is_letter || st == is_digit || st == is_emphasize)
	{
		str = str + GetSymb(is, xy);
		st = GetState(is.peek());
		if (IsSyntaxError(st))
			throw Error("syntax error", pos.first, pos.second);

	}
	return identifier;
}

TokenType ReadDecNumber (istream& is, string& str, Coordinates& xy)
{
	State st = is_digit;
	TokenType T = int_const_dec;
	bool point = false;
	bool exp = false;
	while (st == is_digit)
	{
		st = GetState(is.peek());
		if (st == is_point)
		{
			if (point)
				throw Error("wrong float number", pos.first, pos.second);
			point = true;
			T = float_const;
			str = str + GetSymb(is,xy);
			st = GetState(is.peek());
			if (st == is_point)
			{
				is.unget();
				--xy.first;
				str.resize(str.size()-1);
				return int_const_dec;
			}
		}
		if (is.peek() == 'e' || is.peek() == 'E')
		{
			if (exp)
			    throw Error("wrong float number", pos.first, pos.second);
			exp = true;
			T = float_const;
			str = str + GetSymb(is, xy);
			st = GetState(is.peek());
			if (st == is_plus || st == is_minus)
			{
				str = str + GetSymb(is, xy);
				st = GetState(is.peek());
			}
		}
		st = GetState(is.peek());
		if (st == is_digit)
			str = str + GetSymb(is, xy);
		if (IsSyntaxError(st) || st == is_letter)
		    throw Error("syntax error", pos.first, pos.second);
	}
	return T;
}
bool IsHexDigit(char c)
{
	return (c >= 'a' && 'f' >= c) || (c >= '0' && '9' >= c) || (c >= 'A' && 'F' >= c);
}

TokenType ReadHexNumber(istream& is, string& str, Coordinates& xy)
{
	State st;
	char c;
	while (!is.eof())
	{
		st = GetState(is.peek());
		if (IsSyntaxError(st))
			throw Error("syntax error", pos.first, pos.second);
		if (IsHexDigit(is.peek()))
			str = str + GetSymb(is,xy);
		else
		{
			c = GetSymb(is,xy);
			if (!IsSep(c) && c != '\n' && c != EOF)
				throw Error("wrong hex number", pos.first, pos.second);
			return int_const_hex;
		}
	}
	return int_const_hex;
}

bool IsOctDigit(char c)
{
	return c>= '0' && '7' >= c;
}

TokenType ReadOctNumber(istream& is, string& str, Coordinates& xy)
{
	State st;
	char c;
	while (!is.eof())
	{
		st = GetState(is.peek());
		if (IsSyntaxError(st))
		    throw Error("syntax error", pos.first, pos.second);
		if (IsOctDigit(is.peek()))
			str = str + GetSymb(is,xy);
		else
		{
			c = GetSymb(is,xy);
			if (!IsSep(c) && c != '\n' && c != EOF)
				throw Error("wrong oct number", pos.first, pos.second);
			return int_const_oct;
		}
	}
	return int_const_hex;
}

bool GetCode (istream& is, string& str, Coordinates& xy)
{
	int code;
	is>>code;
	if (code < 0 || 255 < code)
		throw Error("wrong string const", pos.first, pos.second);
	if (code >=0 && 9 >= code)
		++xy.first;
	else
		if(code >= 10 && 99 >= code)
			xy.first += 2;
		else
			xy.first += 3;
	char sym = code;
	str = str + sym;
	return true;
}

TokenType ReadStringConst(istream& is, string& str, Coordinates& xy)
{
	char c;
	bool eos = false;
	c = str[0];
	str = "";
	while (!is.eof())
	{
		switch (c){
			case '\'' :
				while (!is.eof() && !eos)
				{
					c = is.peek();
					if (c == '\n')
						throw Error("wrong string literal", pos.first, pos.second);
					if (c == '\'')
					{
						GetSymb(is,xy);
						if (is.peek() != '\'')
							eos = true;
					}
					if (!eos)
					{
						str = str + c;
						GetSymb(is,xy);
					}
				}
				eos = false;
				break;
			case '#':
				{
					int symb = -1;
					c = is.peek();
					if (GetState(c) != is_digit)
					    throw Error("wrong string literal", pos.first, pos.second);
					is>>symb;
					if (symb < 0 || symb > 255)
					    throw Error("wrong string literal", pos.first, pos.second);
					char ch = symb;
					str = str + ch;
					break;
				}
			default : 
				{
					str = '\'' + str + '\'';
					return string_const;
				}
		}
		c = is.peek();
		if ((!IsSep(c) && c != ' ') || c == '\'')
			GetSymb(is,xy);
	}
	str = '\'' + str + '\'';
	return string_const;
}

Token Scanner::GetNextToken()
{
	TokenType T;
	string str;
	State st;
	char c;
	if (!SkipSpaces(file, xy, pos))
		throw Error("unclosed comment", pos.first, pos.second);
	pos = xy;
	c = GetSymb(file, xy);
	st = GetState(c);
	switch (st){
		case is_letter : 
			{
				str = str + c;
				T = ReadIdentifier(file, str, xy);
				map<string, TokenType>::const_iterator it = SpecSymb.begin();
				it = SpecSymb.find(str);
				if (it != SpecSymb.end())
					T = (*it).second;
			}
			break;
		case is_emphasize:
			str = str + c;
			T = ReadIdentifier(file, str, xy);
			break;
		case is_digit:
			str = str + c;
			T = ReadDecNumber(file, str, xy);
			break;
		case is_dollar:
			str = str + c;
			T = ReadHexNumber(file, str, xy);
			break;
		case is_ampersand:
			str = str + c;
			T = ReadOctNumber(file, str, xy);
			break;
		case is_quote:
			str = str + c;
			T = ReadStringConst(file, str ,xy);
			break;
		case is_sharp:
			str = str + c;
			T = ReadStringConst(file, str ,xy);
			break;
		case is_figure_bracket:
			while (c != '}')
			{
				c = GetSymb(file, xy);
				if (c == EOF)
				    throw Error("unclosed comment", pos.first, pos.second);
			}
			break;
		case is_point:
			str = str + c;
			T = separator;
			if (file.peek() == '.')
				str = str + GetSymb(file, xy);
			break;
		case is_colon:
			str = str + c;
			T = separator;
			if (file.peek() == '=')
			{
				str = str + GetSymb(file, xy);
				T = operation;
			}
			break;
		case is_more:
			str = str + c;
			T = operation;
			if (file.peek() == '=')
				str = str + GetSymb(file, xy);
			break;
		case is_less:
			str = str + c;
			T = operation;
			if (file.peek() == '>' || file.peek() == '=')
				str = str + GetSymb(file, xy);
			break;
		case is_end_of_file:
			T = end_of_file;
			str = "EOF";
			break;
		default:
			str = str + c;
			map<string, TokenType>::const_iterator it = SpecSymb.begin();
			it = SpecSymb.find(str);
			if (it != SpecSymb.end())
				T = (*it).second;
			else
				throw Error("syntax error", pos.first, pos.second);

	}
	return Token(str, T, pos);
}