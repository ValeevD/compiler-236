#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <strstream>
#include "error.h"
#include <stdlib.h>

using namespace std;

typedef pair<int,int> Coordinates;

extern string IntToStr(int x);

typedef enum{
	none,
	is_letter,
	is_digit,
	is_dollar,
	is_ampersand,
	is_sharp,
	is_quote,
	is_emphasize,
	is_minus,
	is_plus,
	is_star,
	is_slash,
	is_point,
	is_colon,
	is_less,
	is_more,
	is_round_bracket,
	is_figure_bracket,
	is_square_bracket,
	is_space,
	is_end_of_line,
	is_end_of_file,
}State;

typedef enum{
	identifier,
	keyword,
	operation,
	separator,
	int_const_dec,
	int_const_hex,
	int_const_oct,
	float_const,
	string_const,
	end_of_file,
	wrong_token
}TokenType;



class Token{
	string value;
	TokenType type;
	Coordinates xy;
public:
	Token (string _value, TokenType _type, Coordinates _xy): value(_value), type(_type), xy(_xy){}
	string GetValue() {return value;}
	TokenType GetType() {return type;}
	Coordinates GetPos() {return xy;}
};

class Scanner{
	ifstream file;
	Coordinates xy;
public:
	Scanner(char* file_name);
	Token GetNextToken();
};

class Error{
public:
	string err;
	int X, Y;
	Error(string _err, Token t) : err(_err) , X(t.GetPos().first), Y(t.GetPos().second) {}
	Error(string _err, int _X, int _Y) : err(_err) , X(_X), Y(_Y) {}
	
};
