// // #include "lexical.h"
// namespace lexical
// {

//     // Implements of functions from class Pair.

//     bool lexical::Pair::IsDigit(char ch)
//     {
//         if (ch <= '9'&&ch >= '0')
//             return true;
//         return false;
//     }

//     // Returns whether the given character a legal identifier character or not. For details of the lexical identifier, please refer to '~/doc/Lexical Rules.md'.
//     bool lexical::Pair::IsLetter(char ch)
//     {
//         if ((ch >= 'a'&& ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || ch =='_')
//             return true;
//         return false;
//     }

//     int lexical::Pair::Digit(char ch, int numbering_system = 10)
//     {
//         switch (numbering_system)
//         {
//         case 10:return (int)ch - 48;
//         }
//         return -1;
//     }




//     // Implements of functions from class Lexical.

//     // private functions
//     void lexical::Lexical::reserve(Token w)
//     {
//         reserved.push_back(w);
//     }

//     void lexical::Lexical::readch()
//     {
//         if (!Lexical::IsFileStream)
//         {
//             scanf("%c", &peek);
//             inner_scan_count++;
//         }
//         else
//         {
//             fstream fs(Lexical::_filename.data(),ios::in);
//             if(fs.good())
//             {
//                 fs.seekg(sizeof(char)*Lexical::charcount,ios::beg);
//                 // fs.seekg(Lexical::charcount,ios::beg);
//                 fs.read(&peek,sizeof(char));
//                 Lexical::charcount++;
//                 inner_scan_count++;
//             }
//             fs.close();
//         }

//     }

//     bool lexical::Lexical::readch(char c)
//     {
//         readch();
//         if (peek != c)
//             return false;
//         peek = ' ';
//         return true;
//     }

//     // public functions.

//     lexical::Lexical::Lexical()
//     {
//         reserve(Token(Tag::IF,"if"));
//         reserve(Token(Tag::ELSE,"else"));
//         reserve(Token(Tag::WHILE,"while"));
//         reserve(Token(Tag::TRUE,"true"));
//         reserve(Token(Tag::FALSE,"false"));
//         reserve(Token(Tag::BASIC_BOOL,"bool"));
//         reserve(Token(Tag::BASIC_NUM,"number"));
//         inner_scan_count = 0;
//         peek = ' ';
//     }

//     Token lexical::Lexical::MatchReserved(string str)
//     {
//         for (int index = 0; index < reserved.size(); index++)
//         {
//             if (memcmp((reserved[index].GetName()).data(), str.data(), str.length())==0)
//             {
//                 return reserved[index];
//             }
//         }
//         return Token(Tag::EOL);// Must invoke functions from identifier table.
//     }

//     bool lexical::Lexical::SetFileStream(string filename)
//     {
//         if(filename.size()==0)
//         {
//             printf("Parameter error! Compiler is trying to parse from console.\n");
//             Lexical::IsFileStream = false;
//         }
//         else 
//         {
//             Lexical::IsFileStream = true;
//             Lexical::_filename = filename;
//         }
//         return Lexical::IsFileStream;
//     }

//     Token lexical::Lexical::Scan()
//     {
//         bool go = true;
//         while (go)
//         {
//             readch();
//             switch (peek)
//             {
//             case ' ':break;
//             case '\t':break;
//             case '\n':line++; break;
//             default: go = false;
//             }
//         }
//         switch (peek)
//         {
//         case '&':return readch('&') ? Token(Tag::AND,"&&") : Token(Tag::EOL);
//         case '|':return readch('|') ? Token(Tag::OR,"||") : Token(Tag::EOL);
//         case '=':return readch('=') ? Token(Tag::EQU,"==") : Token(Tag::ASN,"=");
//         case '!':return readch('=') ? Token(Tag::NE,"!=") : Token(Tag::NOT,"!");
//         case '<':return readch('=') ? Token(Tag::LE,"<=") : Token(Tag::LT,"<");
//         case '>':return readch('=') ? Token(Tag::GE,">=") : Token(Tag::GT,">");
//         case '(':return Token(Tag::LB,"(");
//         case ')':return Token(Tag::RB,")");
//         case '{':return Token(Tag::BGN,"{");
//         case '}':return Token(Tag::END,"}");
//         case ';':return Token(Tag::EDS,";");
//         case '+':return Token(Tag::ADD,"+");
//         case '-':return Token(Tag::SUB,"-");
//         case '*':return Token(Tag::MUL,"*");
//         case '/':return Token(Tag::DIV,"/");
//         }

//         // Get number type value from the input stream.
//         if (Pair::IsDigit(peek))
//         {
//             int result = 0;
//             do
//             {
//                 result += (10 * result + Pair::Digit(peek));
//                 readch();
//             } while (Pair::IsDigit(peek));
//             // Reached the boundary of the integer.
//             if (peek != '.')
//             {
//                 stringstream ss;
//                 ss.clear();
//                 ss<< result;
//                 return Token(Tag::NUM); // TODO: Add const item into Symbol Table.
//             }
//             float value = result;
//             float digit = 10;

//             while (true)
//             {
//                 readch();
//                 if (!Pair::IsDigit(peek))
//                     break;
//                 // Go on for a float number number.
//                 value += Pair::Digit(peek) / digit;
//                 digit *= 10;
//             }
//             // Return the matched float number.
//             return Token(Tag::NUM); // TODO: Add const item into Symbol Table.
//         }

//         // Get Identifier
//         if (Pair::IsLetter(peek))
//         {
//             stringstream ss;
//             ss.clear();
//             do {
//                 ss << peek;
//                 readch();
//             } while (Pair::IsLetter(peek));
//             string str = ss.str();
//             Token reserved_word = MatchReserved(str);
//             if((int)reserved_word.GetTag()!=-1)
//             return reserved_word;
//             reserved_word = Token(Tag::ID,str); // TODO: Add item to Symbol Table.
//             return reserved_word;
//         }
//         Token token(Tag::EOL); // MODIFY HERE.
//         peek = ' ';
//         return token;
//     }

//     void lexical::Lexical::Print(vector<Token> tokens)
//     {
//         if(Lexical::IsFileStream)
//             PrintFile(tokens);
//         if(tokens.size()==0)
//             return;
//         const int size = tokens.size();
//         for(int index= 0;index<size;index++)
//         {
//             int tag = tokens[index].GetTag();
//             printf("%d,\t %d\n",index,tag);
//         }
//     }

//     // Manually print the tokens to local file or control the target with the IsFileStream flag.
//     void lexical::Lexical::PrintFile(vector<Token> tokens)
//     {
//         if(tokens.size()==0)
//             return;
//         const int size = tokens.size();
//         std::fstream stream("a.token",ios::out|ios::trunc);
//         if(!stream)
//         {
//             printf("ERROR OPENING a.token!\n");
//             return;
//         }else{
//             stream.clear();
//             stream<<
//             "# David Moriaty (@DevChache, yangzd1996@outlook.com)\n"<<
//             "# This is the auto generated token file for lexical.h in CURIO, \n"<<
//             "# the first column of numbers are the index and the second column \n"<<
//             "# are the internal tags of each lexeme."<< endl;
//         }
//         for(int index= 0;index<size;index++)
//         {
//             int tag = tokens[index].GetTag();
//             stream<<"< "<<index<<" ,\t"<<tag<<" >\n";
//         }
//         stream.close();
//     }

//     int lexical::Lexical::GetInnerScanCount()
//     {
//         return inner_scan_count;
//     }
// }