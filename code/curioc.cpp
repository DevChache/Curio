#include <iostream>
#include "src/lexical.h"

using namespace std;
using namespace lexical;


int main()
{
    lexical::Lexical A;
    A.SetFileStream("helloworld.curio");
    vector<Token> storage;
    fstream fs("helloworld.curio", ios::in);
    fs.seekg(0,ios::end);
    int num = fs.tellg();
    fs.close();
    while(num>1)
    {
        storage.push_back(A.Scan());
        num -= A.GetInnerScanCount();
    }
    
    printf("token length: %d\n",storage.size());
    A.Print(storage);
    char c;
    scanf("%c", &c);
    return 0;
}