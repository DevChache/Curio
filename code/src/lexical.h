#include <stdio.h>
#include <vector>
#include <fstream>
#include "token.h"
using namespace token;
using namespace idtable;
namespace lexical
{
	// Pair is used to judge whether the given character a number or a letter.
	class Pair
	{
	public:
		static bool IsDigit(char);
		// Returns whether the given character a legal identifier character or not. For details of the lexical identifier, please refer to '~/doc/Lexical Rules.md'.
		static bool IsLetter(char);
		static int Digit(char, int);
	};


	class Lexical
	{
	private:
		static int line;
		static bool IsFileStream;
		static string _filename;
		static int charcount;
		int inner_scan_count;
		char peek;
		vector<Symbol> SymbolTable;
		vector<Token> reserved;
		// Add one symbol to the reserved token array.
		void reserve(Token);
		// Read one character from standard i/o stream and replace the peek value with it.
		void readch();
		// Judge whether the next character equals the given character.
		bool readch(char);

		void add_symbol(Symbol);
	public:
		Lexical();
		// MatchReserved function returns the keyword / type word from reserved area. While there is no match, the function returns a new Word of NULL value and Tag number -1;
		Token MatchReserved(string);
		
		vector<Symbol> GetSymbols();
		
		// Summary: Set the IsFileStream flag to true.
		// Result
		bool SetFileStream(string);

		// Summary: 'Scan' returns one Token scaned by the lexical analysiser.
		// Return: one token object each times.
		Token Scan();

		int AddressRegisteredSymbol(string,SymbolType );
		
		// Summary: Print the tokens to standard i/o (eg. console).
		static void Print(vector<Token>);

		// Summary: Manually print the tokens to local file or control the target with the IsFileStream flag.
		static void PrintFile(vector<Token>);

		static void PrintSymbols(vector<Symbol>);
		static void PrintSymbolsFile(vector<Symbol>);

		// Summary: return value of inner_scan_count.
		// Return: value of inner_scan_count.
		int GetInnerScanCount();
	};

    // static members of class Lexical.
    int lexical::Lexical::line = 1;
    int lexical::Lexical::charcount = 0;
    bool lexical::Lexical::IsFileStream = false;
    string lexical::Lexical::_filename = "";


    // Implements of functions from class Pair.
    bool lexical::Pair::IsDigit(char ch)
    {
        if (ch <= '9'&&ch >= '0')
            return true;
        return false;
    }

    // Returns whether the given character a legal identifier character or not. 
	// For details of the lexical identifier, please refer to '~/doc/Lexical Rules.md'.
    bool lexical::Pair::IsLetter(char ch)
    {
        if ((ch >= 'a'&& ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || ch =='_')
            return true;
        return false;
    }


    int lexical::Pair::Digit(char ch, int numbering_system = 10)
    {
        switch (numbering_system)
        {
        case 10:return (int)ch - 48;
        }
        return -1;
    }




    // Implements of functions from class Lexical.

    // private functions
	void lexical::Lexical::add_symbol(Symbol sym)
	{
		SymbolTable.push_back(sym);
	}

    void lexical::Lexical::reserve(Token w)
    {
        reserved.push_back(w);
    }

    void lexical::Lexical::readch()
    {
        if (!Lexical::IsFileStream)
        {
            scanf("%c", &peek);
            inner_scan_count++;
        }
        else
        {
            fstream fs(Lexical::_filename.data(),ios::in);
            if(fs.good())
            {
                fs.seekg(sizeof(char)*Lexical::charcount,ios::beg);
                // fs.seekg(Lexical::charcount,ios::beg);
                fs.read(&peek,sizeof(char));
                Lexical::charcount++;
                inner_scan_count++;
            }
            fs.close();
        }

    }

    bool lexical::Lexical::readch(char c)
    {
        readch();
        if (peek != c)
            return false;
        peek = ' ';
        return true;
    }

    // public functions.

    lexical::Lexical::Lexical()
    {
		reserve(Token(Tag::IF,"if"));
		reserve(Token(Tag::ELSE,"else"));
		reserve(Token(Tag::WHILE,"while"));
		reserve(Token(Tag::TRUE,"true"));
		reserve(Token(Tag::FALSE,"false"));
		reserve(Token(Tag::BASIC_BOOL,"bool"));
		reserve(Token(Tag::START,"program"));
		reserve(Token(Tag::BASIC_NUM,"number"));
		reserve(Token(Tag::DO,"do"));
		reserve(Token(Tag::THEN,"then"));
		reserve(Token(Tag::VAR,"var"));
		inner_scan_count = 0;
		peek = ' ';
    }

    Token lexical::Lexical::MatchReserved(string str)
    {
        for (int index = 0; index < reserved.size(); index++)
        {
            if (memcmp((reserved[index].GetName()).data(), str.data(), str.length())==0)
            {
                return reserved[index];
            }
        }
		Token nomatch(Tag::EOL);
		nomatch.SetAddress(-1);

        return nomatch;// Must invoke functions from identifier table.
    }

	// Summary: Find address of registered symbol and return -1 if none exists.
	// Return: Address of matched symbol or -1 for no match.
	int lexical::Lexical::AddressRegisteredSymbol(string name,SymbolType type)
	{
		for (int index = 0; index < SymbolTable.size(); index++)
        {
			if (memcmp((SymbolTable[index].GetName()).data(), name.data(), name.length()) == 0 && SymbolTable[index].GetType()==type)
            {
				return index;
			}
		}
		return -1;
	}

    bool lexical::Lexical::SetFileStream(string filename)
    {
        if(filename.size()==0)
        {
            printf("Parameter error! Compiler is trying to parse from console.\n");
            Lexical::IsFileStream = false;
        }
        else 
        {
            Lexical::IsFileStream = true;
            Lexical::_filename = filename;
        }
        return Lexical::IsFileStream;
    }
	void lexical::Lexical::PrintSymbols(vector<Symbol> syms)
	{
		if(Lexical::IsFileStream)
			PrintSymbolsFile(syms);
		if(syms.size()==0)
			return;
		const int size = syms.size();
		for(int index= 0;index<size;index++)
		{
			printf("( %d , %d , %s )\n",index,syms[index].GetType(),syms[index].GetName().data());
		}
	}

	// Manually print the symbols to local file or control the target with the IsFileStream flag.
    void lexical::Lexical::PrintSymbolsFile(vector<Symbol> syms)
    {
        if(syms.size()==0)
            return;
        const int size = syms.size();
        std::fstream stream("a.symbols",ios::out|ios::trunc);
        if(!stream)
        {
            printf("ERROR OPENING a.symbols!\n");
            return;
        }else{
            stream.clear();
            stream<<
            "# David Moriaty (@DevChache, yangzd1996@outlook.com)\n"<<
            "# This is the auto generated symbol file for idtable.h in CURIO, \n"<<
            "# the first column of numbers are the index and the second column \n"<<
            "# are the internal tags of each lexeme.\n\n"<<
			"< idx , type , name >"<< endl;
        }
        for(int index= 0;index<size;index++)
        {
			stream<<"< "<<index<<" ,\t "<<syms[index].GetType()<<" ,\t "<<syms[index].GetName()<<" >\n";
        }
        stream.close();
    }

    Token lexical::Lexical::Scan()
    {
		inner_scan_count = 0;
        bool go = true;
        while (go)
        {
            readch();
            switch (peek)
            {
            case ' ':break;
            case '\t':break;
            case '\n':line++; break;
            default: go = false;
            }
        }
        switch (peek)
        {
        case '&':return readch('&') ? Token(Tag::AND,"&&") : Token(Tag::EOL);
        case '|':return readch('|') ? Token(Tag::OR,"||") : Token(Tag::EOL);
        case '=':return readch('=') ? Token(Tag::EQU,"==") : Token(Tag::ASN,"=");
        case '!':return readch('=') ? Token(Tag::NE,"!=") : Token(Tag::NOT,"!");
        case '<':return readch('=') ? Token(Tag::LE,"<=") : Token(Tag::LT,"<");
        case '>':return readch('=') ? Token(Tag::GE,">=") : Token(Tag::GT,">");
        case '(':return Token(Tag::LB,"(");
        case ')':return Token(Tag::RB,")");
        case '{':return Token(Tag::BGN,"{");
        case '}':return Token(Tag::END,"}");
        case ';':return Token(Tag::EDS,";");
        case '+':return Token(Tag::ADD,"+");
        case '-':return Token(Tag::SUB,"-");
        case '*':return Token(Tag::MUL,"*");
        case '/':return Token(Tag::DIV,"/");
		case ',':return Token(Tag::COMMA,",");
		case ':':return Token(Tag::COLON,":");
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
            {
                stringstream ss;
                ss.clear();
                ss<< result;
				int registered = AddressRegisteredSymbol(ss.str(),SymbolType::CONST);
				if(registered==-1)
				{
					Symbol symbol(SymbolType::CONST,result);
					add_symbol(symbol);
					Token token(Tag::NUM,symbol.GetName()); // TODO: Add const item into Symbol Table.
					token.SetAddress(symbol.GetIndex());
				}
				Token token(Tag::NUM,ss.str());
				token.SetAddress(registered);
				return token;
            }
            double value = result;
            double digit = 10;

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
			stringstream ss;
			ss.clear();
			ss<< value;
			int registered = AddressRegisteredSymbol(ss.str(),SymbolType::CONST);
			if(registered==-1)
			{
				Symbol symbol(SymbolType::CONST,value);
				add_symbol(symbol);
            	Token token(Tag::NUM,symbol.GetName()); // TODO: Add const item into Symbol Table.
				token.SetAddress(symbol.GetIndex());
			}
			Token token(Tag::NUM,ss.str());
			token.SetAddress(registered);
			return token;
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
            Token reserved_word = MatchReserved(str);
            if((int)reserved_word.GetTag() != -1)
				return reserved_word;
			int registered_index = AddressRegisteredSymbol(str,SymbolType::IDENTIFIER);

			if(registered_index == -1)
			{
				Symbol symbol(SymbolType::IDENTIFIER, str);
				registered_index = symbol.GetIndex();
                printf("symbol type: %d, name: %s\n",symbol.GetType(),symbol.GetName().data());
				add_symbol(symbol);
			}
            reserved_word = Token(Tag::ID,str);
			reserved_word.SetAddress(registered_index);
			reserve(reserved_word);
            return reserved_word;
        }
        Token token(Tag::EOL); // MODIFY HERE.
        peek = ' ';
        return token;
    }

    void lexical::Lexical::Print(vector<Token> tokens)
    {
        if(Lexical::IsFileStream)
            PrintFile(tokens);
        if(tokens.size()==0)
            return;
        const int size = tokens.size();
        for(int index= 0;index<size;index++)
        {
            int tag = tokens[index].GetTag();
            printf("%d,\t %d\n",index,tag);
        }
    }

    // Manually print the tokens to local file or control the target with the IsFileStream flag.
    void lexical::Lexical::PrintFile(vector<Token> tokens)
    {
        if(tokens.size()==0)
            return;
        const int size = tokens.size();
        std::fstream stream("a.token",ios::out|ios::trunc);
        if(!stream)
        {
            printf("ERROR OPENING a.token!\n");
            return;
        }else{
            stream.clear();
            stream<<
            "# David Moriaty (@DevChache, yangzd1996@outlook.com)\n"<<
            "# This is the auto generated token file for lexical.h in CURIO, \n"<<
            "# the first column of numbers are the index and the second column \n"<<
            "# are the internal tags of each lexeme.\n\n"<<
			"< idx ,\t tag ,\t name ,\t addr >"<< endl;
        }
        for(int index= 0;index<size;index++)
        {
			stream<<"< "<<index<<" ,\t "<<tokens[index].GetTag()<<" ,\t "<<tokens[index].GetName()<<" ,\t "<<tokens[index].GetAddress()<<" >\n";
        }
        stream.close();
    }
	
	vector<Symbol> Lexical::GetSymbols()
	{
		return SymbolTable;
	}

    int lexical::Lexical::GetInnerScanCount()
    {
        return inner_scan_count;
    }
}
