#include <fstream>
#include "grammar.h"
using namespace std;
using namespace idtable;
using namespace quadruple;

namespace generator
{
    // Summary: input normal quadruples and analysis them to make an array of basic blocks.
    vector<quadruple::BasicBlockQuadruple> InitialBasicBlocks(vector<quadruple::Quadruple>);

    // Summary: print the quadruples to local file called a.quadruples.
    void PrintQuadruplesFile(vector<quadruple::Quadruple>);

    // Summary: print the basic blocks to local file called a.basicblocks.
    void PrintBasicBlocksFile(vector<quadruple::Quadruple>);

    // Summary: print the basic blocks to local file called a.basicblocks.
    void PrintBasicBlocksFile(vector<quadruple::BasicBlockQuadruple>);


    


    // Summary: input normal quadruples and analysis them to make an array of basic blocks.
    vector<quadruple::BasicBlockQuadruple> generator::InitialBasicBlocks(vector<quadruple::Quadruple> original)
    {
        int size = original.size();
        vector<quadruple::BasicBlockQuadruple> target;
        int temp_index = -1; // The temp_index records the next quadruple of J** ones.
        for(int index=0;index<size;index++)
        {
            if(index==0||index==temp_index)
            {
                target.push_back(quadruple::BasicBlockQuadruple(original[index],true));
                temp_index=-1;
                continue;
            }

            target.push_back(quadruple::BasicBlockQuadruple(original[index],false));
            int opt = (int)original[index].GetOperation();
            if(opt>4&&opt<12)
            {
                target[original[index].GetAddr()].SetBasic();
                temp_index = index+1;
            }
        }
        return target;
    }

    // Summary: print the quadruples to local file called a.quadruples.
    void generator::PrintQuadruplesFile(vector<quadruple::Quadruple> src)
    {
        if(src.size()==0)
            return;
        const int size = src.size();
        std::fstream stream("a.quadruples",ios::out|ios::trunc);
        if(!stream)
        {
            printf("ERROR OPENING a.quadruples!\n");
            return;
        }else{
            stream.clear();
            stream<<
            "# David Moriaty (@DevChache, yangzd1996@outlook.com)\n"<<
            "# This is the auto generated quadruples file for generator.h in CURIO, \n"<<
            "< idx , opt , src , dst , addr >"<< endl;
        }
        for(int index= 0;index<size;index++)
        {
            stream<<"< "<<index<<" ,\t "<<src[index].GetOperation()<<" ,\t "<<src[index].GetSrc()<<" ,\t "<<src[index].GetDst()<<" ,\t "<<src[index].GetAddr()<<" >\n";
        }
        stream.close();
    }
    
    // Summary: print the basic blocks to local file called a.basicblocks.
    void generator::PrintBasicBlocksFile(vector<quadruple::Quadruple> src)
    {
        PrintBasicBlocksFile(InitialBasicBlocks(src));
    }

    // Summary: print the basic blocks to local file called a.basicblocks.
    void generator::PrintBasicBlocksFile(vector<quadruple::BasicBlockQuadruple> src)
    {
        if(src.size()==0)
            return;
        const int size = src.size();
        std::fstream stream("a.bblocks",ios::out|ios::trunc);
        if(!stream)
        {
            printf("ERROR OPENING a.bblocks!\n");
            return;
        }else{
            stream.clear();
            stream<<
            "# David Moriaty (@DevChache, yangzd1996@outlook.com)\n"<<
            "# This is the auto generated basic_blocks file for generator.h in CURIO, \n"<<
            "< idx , opt , src , dst , addr , is_entry >"<< endl;
        }
        for(int index= 0;index<size;index++)
        {
            quadruple::Quadruple qple = src[index].GetQuadruple();
            stream<<"< "<<index<<" ,\t "<<qple.GetOperation()<<" ,\t "<<qple.GetSrc()<<" ,\t "<<qple.GetDst()<<" ,\t "<<qple.GetAddr()<<" ,\t "<<(src[index].IsEntrance()?"true":"false")<<" >\n";
        }
        stream.close();
    }
}


namespace basic
{   
    vector<Symbol> symbols;
    vector<Symbol> sub_symbols;
    vector<BasicBlockQuadruple> blocks;
    bool BX = false;
    bool DX = false;
    enum Active
    {
        ACTIVE = 0,
        INACTIVE = 1
    };
    enum Usage
    {
        WAITING = 0,
        HANGING = 1
    };
    // Summary: copy external symbols to symbols(storage);
    void CloneSymbols(vector<Symbol>);
    
    // Summary: divide combined basic blocks into pieces and invoke BlockScan() to append usage and active information. 
    void BlockDiv(vector<BasicBlockQuadruple>);

    // Summary: append usage and active information. Reset two registers before return.
    void BlockScan(vector<BasicBlockQuadruple>);

    // Summary: allocate register for each SRC variable.
    void AllocateRegister(BasicBlockQuadruple);


    void CloneSymbols(vector<Symbol> symbls)
    {
        basic::symbols = symbls;
    }

    void BlockDiv(vector<BasicBlockQuadruple> original_blocks)
    {
        int size = original_blocks.size();
        vector<BasicBlockQuadruple> temp;
        int begin = 0;
        int end = 1;
        while(end<size)
        {
            while(end<size)
            {
                if(!original_blocks[end].IsEntrance())
                    end++;
                else 
                    break;
            }
            while(begin<end)
            {
                temp.push_back(original_blocks[begin]);
                begin++;
            }
            end++;
            BlockScan(temp); // invoking.
            temp.clear();
        }
    }

    void BlockScan(vector<BasicBlockQuadruple> bblock)
    {
        const int bb_size = bblock.size();
        sub_symbols.clear();
        for(int index=0;index<bb_size;index++)
        {
            int Op = bblock[0].GetQuadruple().GetOperation();
        }



        int size = bb_size-1;
        while(size>=0)
        {
            AllocateRegister(bblock[size]);
            size --;
        }
        BX = DX = false;
    }
    void AllocateRegister(BasicBlockQuadruple quadruple)
    {

    }
}
