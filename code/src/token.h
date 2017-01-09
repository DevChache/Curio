#include <string>
using namespace std;
namespace token
{
    // The tags are defined in the document '~/doc/Symbol Table.md'.
	enum Tag
	{
		NUM = 1,
		ID = 2,
		BASIC_NUM = 3,
		BASIC_BOOL = 4,
		TRUE = 5,
		FALSE = 6,
		WHILE = 7,
		IF = 8,
		ELSE = 9,
		ADD = 10,
		SUB = 11,
		MUL = 12,
		DIV = 13,
		ASN = 14,
		EQU = 15,
		GT = 16,
		LT = 17,
		LE = 18,
		GE = 19,
		NE = 20,
		AND = 21,
		OR = 22,
		NOT = 23,
		EDS = 24,
		LB = 25,
		RB = 26,
		EOL = -1,
		BGN = 27,
		END = 28,
		START = 29
	};

    // TokenNode is the base data structure of tuples that stores entry information for the symbol table.
    class Token
    {
    private:
        static int Count;
        int Index; // Only use when forming a list.
        string Name; // The name of token in the source code file.
        Tag Tag; // Internal Code of the token.
        int Address; // Entry address of identifier or constant. -1 for keywords or delimiters.
    public:
        Token(token::Tag tag);
        Token(token::Tag tag,string name);
        token::Tag GetTag();
        string GetName();
        int GetAddress();
        int GetIndex();
    };
    int Token::Count = 0;

}