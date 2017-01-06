#include "lexical.h"
using namespace lexical;
namespace token
{
    // TokenNode is the base data structure of tuples that stores basic information for the symbol table.
    class TokenNode
    {
    public:
        TokenNode(string val, lexical::Tag tag)
        {
            token = lexical::Token(tag);
            value = val;
        }
        string GetValue();
        string ToString();
        lexical::Token GetToken();
        lexical::Tag GetTokenTag();
    private:
        lexical::Token token = lexical::Token(-1);
        string value;
    };
    string token::TokenNode::GetValue()
    {
        return value;
    }
    string token::TokenNode::ToString()
    {
        stringstream ss;
        ss.clear();
        ss<<"< "<<token.GetTag()<<" , "<<value<<" >";
        return ss.str();
    }
    lexical::Token token::TokenNode::GetToken()
    {
        return token;
    }
    lexical::Tag token::TokenNode::GetTokenTag()
    {
        return token.GetTag();
    }
    class TokenList
    {
        public:
    };

}