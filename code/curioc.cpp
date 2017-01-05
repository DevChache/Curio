#include <stdio.h>
#include <string>
#include "code\src\lexical.h"
#include <iostream>
#include <atlstr.h>

using namespace std;
using namespace lexical;


void main()
{
	Lexical A;
	vector<Word> vec =	A.GetReserved();
	A.MatchReserved("sf");
	char c;
	scanf_s("%c", &c);
}