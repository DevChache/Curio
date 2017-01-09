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
        storage.push_back(A.scan());
    }
    printf("== Token Output ==\n%s\n","< index , tag >");
    for(int i = 0; i<storage.size(); i++)
    {
        printf("< %d , %d >\n",i,storage[i].GetTag());
    }
    printf("== END ==\n");
    char c;
    scanf("%c", &c);
    return 0;
}