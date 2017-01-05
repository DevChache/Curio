#include <stdio.h>
#include <string>
using namespace std;
namespace lexical
{
	// The tags are defined in the document '~/doc/Symbol Table.md'.
	enum Tag
	{
		STR = 1,
		NUM = 2,
		BOOL = 3,
		ID = 4,
		BASIC_STR = 5,
		BASIC_NUM = 6,
		BASIC_BOOL = 7,
		TRUE = 8,
		FALSE = 9,
		WHILE = 10,
		IF = 11,
		ELSE = 12,
		BASIC_VOID = 13,
		ADD = 14,
		SUB = 15,
		MUL = 16,
		DIV = 17,
		ASN = 18,
		EQU = 19,
		GT = 20,
		LT = 21,
		LE = 22,
		GE = 23,
		NE = 24,
		AND = 25,
		OR = 26,
		ESC = 27
	};

	class Token
	{
	public:
		int tag;
		Token(Tag tg)
		{
			tag = tg;
		}
		string toString()
		{
			return "" + tag;
		}
	};

	class Number :public Token
	{
	public:
		int value;
		Number(int val):Token(Tag.NUM)
		{
			value = val;
		}
	};



}
