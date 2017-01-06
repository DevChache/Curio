#include <stdio.h>
#include <string>
#include "src/lexical.h"
#include <iostream>

using namespace std;
using namespace lexical;


int main()
{
    Lexical A;
    int a = 10;
    vector<Token> storage;
    while(a-->0)
    {
        storage.push_back(*A.pscan());
    }
    for(int i = 0; i<storage.size(); i++)
    {
        Token* token = &storage[i];
		printf("word type length: %d\n",((Word*)token)->ToString().length());
		// printf("number type length: %d\n",((Number*)token)->ToString().length());
		// printf("token type length: %d\n",((Token*)token)->ToString().length());
        // if(((Word*)token)->ToString().length()!=0)
        //     printf("%s\t%d\n",(((Word*)token)->ToString()).data(),((Word*)token)->GetTag());
        // else if(((Number*)token)->ToString().length()!=0)
        //     printf("%s\t%d\n",(((Number*)token)->ToString()).data(),((Number*)token)->GetTag());
        // else if(token->ToString().length()!=0)
        //     printf("%s\t%d\n",(token->ToString()).data(),token->GetTag());
    }
    char c;
    scanf("%c", &c);
    return 0;
}