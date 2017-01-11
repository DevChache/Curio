#include <iostream>
#include "src/lexical.h"

using namespace lexical;
using namespace idtable;
int main()
{
    if(false)
    {
        lexical::Lexical A;
        A.SetFileStream("helloworld.curio");
        vector<Token> storage;
        fstream fs("helloworld.curio", ios::in);
        fs.seekg(0,ios::end);
        int num = fs.tellg();
        fs.close();
        printf("character size: %d\n",num);
        while(num>1)
        {
            storage.push_back(A.Scan());
            num -= A.GetInnerScanCount();
            printf("--------num left: %d\n",num);
        }
        // TODO
        
        printf("token length: %d\n",storage.size());
        A.Print(storage);
    }
    Symbol symbol(SymbolType::CONST,"15.5");
    printf("value: %lf\n", symbol.GetValue());
    char c;
    scanf("%c", &c);
    return 0;
}