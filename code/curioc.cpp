#include <stdio.h>
#include <string>
#include "src/lexical.h"
#include <iostream>

using namespace std;
using namespace lexical;


int main()
{
    Lexical A;
    A.SetFileStream("helloworld.curio");
    vector<Token> storage;
    fstream fs("helloworld.curio", ios::in);
    fs.seekg(0,ios::end);
    int num = fs.tellg();
    fs.close();
    printf("size: %d\n",num);
    // It is not possible to loop in so many times, there will be an long duration.
    // FIX;
    while(num>0)
    {
        storage.push_back(A.scan());
        num--;
    }

    
    printf("== Token Output ==\n%s\n","< index , tag >");
    A.print(storage);
    char c;
    scanf("%c", &c);
    return 0;
}