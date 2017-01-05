#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
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
		Token(int tg)
		{
			tag = tg;
		}
		string ToString()
		{
			stringstream ss;
			ss.clear();
			ss << tag;
			return ss.str();
		}
		Tag GetTag()
		{
			Tag t;
			switch (tag)
			{
			case 1: t = Tag::STR; break;
			case 2: t = Tag::NUM; break;
			case 3: t = Tag::BOOL; break;
			case 4: t = Tag::ID; break;
			case 5: t = Tag::BASIC_STR; break;
			case 6: t = Tag::BASIC_NUM; break;
			case 7: t = Tag::BASIC_BOOL; break;
			case 8: t = Tag::TRUE; break;
			case 9: t = Tag::FALSE; break;
			case 10: t = Tag::WHILE; break;
			case 11: t = Tag::IF; break;
			case 12: t = Tag::ELSE; break;
			case 13: t = Tag::BASIC_VOID; break;
			case 14: t = Tag::ADD; break;
			case 15: t = Tag::SUB; break;
			case 16: t = Tag::MUL; break;
			case 17: t = Tag::DIV; break;
			case 18: t = Tag::ASN; break;
			case 19: t = Tag::EQU; break;
			case 20: t = Tag::GT; break;
			case 21: t = Tag::LT; break;
			case 22: t = Tag::LE; break;
			case 23: t = Tag::GE; break;
			case 24: t = Tag::NE; break;
			case 25: t = Tag::AND; break;
			case 26: t = Tag::OR; break;
			case 27: t = Tag::ESC; break;
			}
			return t;
		}
	private:
		int tag;
	};

	// Number class, inherited form Token class and stores the actual value of a number type.
	class Number :public Token
	{
	public:
		// Constructor function, initialize a number object with the given number value.
		Number(int val) :Token(lexical::Tag::NUM)
		{
			value = val;
		}

		// Return the number value.
		int GetValue()
		{
			return value;
		}

		// Return the number value as a string variable.
		string ToString()
		{
			stringstream ss;
			ss.clear();
			ss << value;
			return ss.str();
		}
	private:
		int value;
	};


	// Word class, inherited from Token class and stores the lexeme information of the current context.
	class Word :public Token
	{
	public:
		// Initialize word object with lexeme s and tag code t.
		Word(string s, int t) :Token(t)
		{
			lexeme = s;
		}
		// Return the lexeme value of current keyword.
		string ToString()
		{
			return lexeme;
		}
	private:
		// lexeme is the lexical unit of the current 'word'.
		string lexeme = "";
	};

	// Boolean values are kept in the object.
	class Boolean :public Token
	{
	public:
		// Initialize bool value with a boolean argument.
		Boolean(bool value) :Token(lexical::Tag::BOOL)
		{
			_bool = value;
		}
		// Return boolean value of _bool property.
		string ToString()
		{
			return _bool ? "True" : "False";
		}
	private:
		bool _bool;
	};

	// A structure that reserves the operators and two boolean.
	struct Keyword
	{
		Word AND = Word("&&", Tag::AND)
			, EQU = Word("==", Tag::EQU)
			, OR = Word("||", Tag::OR)
			, LT = Word("<", Tag::LT)
			, GT = Word(">", Tag::GT)
			, TRUE = Word("true", Tag::TRUE)
			, FALSE = Word("false", Tag::FALSE)
			, LE = Word("<=", Tag::LE)
			, GE = Word(">=", Tag::GE)
			, NE = Word("!=", Tag::NE);
	} struct_keyword;

	// A sturcture that reserves the base types.
	struct Type
	{
		Word NUM = Word("number", Tag::BASIC_NUM)
			, BOOL = Word("bool", Tag::BASIC_BOOL)
			, STR = Word("string", Tag::BASIC_STR)
			, VOID = Word("void", Tag::BASIC_VOID);
	} struct_type;

	// Pair is used to judge whether the given character a number or a letter.
	class Pair
	{
	public:
		static bool IsDigit(char ch)
		{
			if (ch <= '9'&&ch >= '0')
				return true;
			return false;
		}
		static bool IsLetter(char ch)
		{
			if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
				return true;
			return false;
		}
		static bool IsUnderscore(char ch)
		{
			return ch == '_' ? true : false;
		}
		static int Digit(char ch, int numbering_system = 10)
		{
			switch (numbering_system)
			{
			case 10:return (int)ch - 48;
			}
		}
	};


	class Lexical
	{
	private:
		static int line;
		static bool IsFileStream;
		char peek = ' ';
		vector<Word> reserved;
		void reserve(Word w)
		{
			reserved.push_back(w);
		}
		// Read one character from standard i/o stream and replace the peek value with it.
		void readch()
		{
			// 'scanf' means only keyboard input is allowed. This should be updated.
			// TODO: update this function to support a file stream version.
			if (!Lexical::IsFileStream)
				scanf("%c", &peek);
		}
		// Judge whether the next character equals the given character.
		bool readch(char c)
		{
			readch();
			if (peek != c)
				return false;
			peek = ' ';
			return true;
		}
	public:
		Lexical()
		{
			reserve(Word("if", Tag::IF));
			reserve(Word("else", Tag::ELSE));
			reserve(Word("while", Tag::WHILE));
			reserve(struct_keyword.TRUE);
			reserve(struct_keyword.FALSE);
			reserve(struct_type.BOOL);
			reserve(struct_type.NUM);
			reserve(struct_type.STR);
			reserve(struct_type.VOID);
		}
		vector<Word> GetReserved()
		{
			return reserved;
		}
		Word MatchReserved(string str)
		{
			for (int index = 0; index < reserved.size(); index++)
			{
				if (memcmp((reserved[index].ToString()).data(), str.data(), str.length()))
				{
					printf("success\n");
					return reserved[index];
				}
			}
			return Word("LLL", 10);
		}
		bool SetFileStream(bool filestream)
		{
			Lexical::IsFileStream = filestream;
		}
		Token scan()
		{
			bool go = true;
			while (go)
			{
				switch (peek)
				{
				case ' ':;
				case'\t':continue;
				case '\n':line++; continue;
				default: go = false;
				}
			}
			switch (peek)
			{
			case '&':return readch('&') ? struct_keyword.AND : Token('&');
			case '|':return readch('|') ? struct_keyword.OR : Token('|');
			case '=':return readch('=') ? struct_keyword.EQU : Token('=');
			case '!':return readch('=') ? struct_keyword.NE : Token('!');
			case '<':return readch('=') ? struct_keyword.LE : Token('<');
			case '>':return readch('=') ? struct_keyword.GE : Token('>');
			}

			// Get number type value from the input stream.
			if (Pair::IsDigit(peek))
			{
				int result = 0;
				do
				{
					result += (10 * result + Pair::Digit(peek));
					readch();
				} while (Pair::IsDigit(peek));
				// Reached the boundary of the integer.
				if (peek != '.')
					return Number(result);
				float value = result;
				float digit = 10;

				while (true)
				{
					readch();
					if (!Pair::IsDigit(peek))
						break;
					// Go on for a float number number.
					value += Pair::Digit(peek) / digit;
					digit *= 10;
				}
				// Return the matched float number.
				return Number(value);
			}

			// Get Identifier
			if (Pair::IsLetter(peek))
			{
				stringstream ss;
				ss.clear();
				do {
					ss << peek;
					readch();
				} while (Pair::IsLetter(peek));
				string str = ss.str();
			}


		}
	};
	int Lexical::line = 1;
	bool Lexical::IsFileStream = false;

}
