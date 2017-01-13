#include <iostream>
#include "src/generator.h"

using namespace lexical;
using namespace quadruple;
using namespace basic;

int main()
{
    if(true)
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
        A.PrintFile(storage);
        Test T(storage,A.GetSymbols());
        vector<Equ> temp = T.GetEqus();
        vector<Quadruple> quls;
        int size = temp.size();
        for(int index=0;index<size;index++)
        {
            quls.push_back(Quadruple(temp[index]));
        }
        A.PrintSymbolsFile(T.ReturnSymbols());
        generator::PrintQuadruplesFile(quls);
        generator::PrintBasicBlocksFile(quls);
        
        basic::CloneSymbols(T.ReturnSymbols());
        basic::BlockDiv(generator::InitialBasicBlocks(quls));
    }


    string anc_token,anc_symbol,anc_quadruple,anc_basicblock,anc_assembly;
    anc_token = "- Tokens file: a.token\n";
    anc_symbol = "- Symbols file: a.symbols\n";
    anc_quadruple = "- Quadruples file: a.quadruples\n";
    anc_basicblock = "- Basic Blocks file: a.bblocks\n";
    anc_assembly = "- Assembly file: a.assembly.asm\n";
    printf("\n:) Done with success!\n\n### OUTPUT FILES ###\n%s%s%s%s%s\n",anc_token.data(),anc_symbol.data(),anc_quadruple.data(),anc_basicblock.data(),anc_assembly.data());
    return 0;
}