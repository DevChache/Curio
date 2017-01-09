#include <stdio.h>
#include <string>
#include "src/lexical.h"
#include <iostream>

using namespace std;
using namespace lexical;


int main()
{
    Lexical A;
    int a = 5;
    vector<Token> storage;
    while(a-->0)
    {
        storage.push_back(*A.pscan());
    }
    printf("%s\n","< index , value , tag >");
    for(int i = 0; i<storage.size(); i++)
    {
        switch(storage[i].GetTypia())
        {
            case 3:{
                Word* word = static_cast<Word *>(&storage[i]); 
                printf("< %d , %s , %d >\n",i,(word->ToString()).data(),word->GetTag());
                break;
            }
            case 1:{
                Number* number = static_cast<Number *>(&storage[i]);
                printf("< %d , %s , %d >\n",i,(number->ToString()).data(),number->GetTag());
            }
        }
    }
    char c;
    scanf("%c", &c);
    return 0;
}