#include <stdio.h>
#include <sstream>
#include <vector>
#include <fstream>
#include "token.h"
using namespace std;
using namespace token;
namespace lexical
{
	// Pair is used to judge whether the given character a number or a letter.
	class Pair
	{
	public:
		static bool IsDigit(char ch);
		// Returns whether the given character a legal identifier character or not. For details of the lexical identifier, please refer to '~/doc/Lexical Rules.md'.
		static bool IsLetter(char ch);
		static int Digit(char ch, int numbering_system = 10);
	};


	class Lexical
	{
	private:
		static int line;
		static bool IsFileStream;
		static string _filename;
		static int charcount;
		int inner_scan_count;
		char peek = ' ';
		vector<Token> reserved;
		// Add one symbol to the reserved token array.
		void reserve(Token w);
		// Read one character from standard i/o stream and replace the peek value with it.
		void readch();
		// Judge whether the next character equals the given character.
		bool readch(char c);
	public:
		Lexical();
		// MatchReserved function returns the keyword / type word from reserved area. While there is no match, the function returns a new Word of NULL value and Tag number -1;
		Token MatchReserved(string str);
		/* SetFileStream()
		* Summary: Set the IsFileStream flag to true.
		* Result
		*/
		bool SetFileStream(string filename);

		/* Lexical::Scan()
		* Summary: 'Scan' returns one Token scaned by the lexical analysiser.
		* Return: one token object each times.
		*/
		Token Scan();

		
		/* static Lexical::Print()
		* Summary: Print the tokens to standard i/o (eg. console).
		* Parameters:
		* 	tokens: token list;
		*/
		static void Print(vector<Token> tokens);

		/* static Lexical::PrintFile()
		* Summary: Manually print the tokens to local file or control the target with the IsFileStream flag.
		* Parameters:
		* 	tokens: token list;
		*/
		static void PrintFile(vector<Token> tokens);

		/* Lexical::GetInnerScalCount()
		* Summary: return value of inner_scan_count.
		* Return: value of inner_scan_count.
		*/
		int GetInnerScanCount();
	};
}
