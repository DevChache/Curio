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
        int temp_index = 0; // The temp_index records the next quadruple of J** ones.
        for(int index=0;index<size;index++)
        {
            if(index==0||index==temp_index)
            {
                target.push_back(quadruple::BasicBlockQuadruple(original[index],true));
                int opt = (int)original[index].GetOperation();
                if(opt>4&&opt<12)
                {
                    target[original[index].GetAddr()].SetBasic();
                    temp_index = index + 1;
                }
                continue;
            }

            target.push_back(quadruple::BasicBlockQuadruple(original[index],false));
            int opt = (int)original[index].GetOperation();
            if(opt>4&&opt<12)
            {
                target[original[index].GetAddr()].SetBasic();
                temp_index = index + 1;
                printf("test index: %d\n",index);
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
    struct Reg_BX
    {
        static bool used;
        static Symbol* latest;
    };
    bool Reg_BX::used = false;
    struct Reg_DX
    {
        static bool used;
        static Symbol* latest;
    };
    bool Reg_DX::used = false;
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
    enum Reg
    {
        BX= 0,
        DX=1,
        NONE= -1
    };
    class Triple
    {
    public:
        static int count;
        int index;
        Active active;
        Usage usage;
        Triple(Active,Usage);
        Triple();
    };
    int Triple::count = 0;
    Triple::Triple(Active act,Usage usg):active(act),usage(usg){index = Triple::count;count++;}
    Triple::Triple(){active = Active::INACTIVE;usage = Usage::HANGING;}
    
    class Register
    {
    public:
        static int count;
        int index;
        Quadruple quadruple;
        Reg reg;
        Register(Quadruple,Reg);
    };
    int Register::count = 0;
    Register::Register(Quadruple qpl,Reg r):quadruple(qpl),reg(r){index = Register::count;count++;}

    // Combine active info and usage info in one object for one quadruple.
    class CombinedInfo
    {
    public:
        int array[3];
        Triple src;
        Triple dst;
        Triple addr;
        CombinedInfo(int,int,int);
    };
    CombinedInfo::CombinedInfo(int s,int d,int a)
    {
        array = {s,d,a};
    }



    // Summary: copy external symbols to symbols(storage);
    void CloneSymbols(vector<Symbol>);
    
    // Summary: divide combined basic blocks into pieces and invoke BlockScan() to append usage and active information. 
    void BlockDiv(vector<BasicBlockQuadruple>);

    // Summary: find index of a given symbol in the sub_symbols array.
    int IndexInSubSymbols(Symbol);

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

    int IndexInSubSymbols(Symbol symbol)
    {
        for(int index = 0;index<sub_symbols.size();index++)
        {
            if(sub_symbols[index].GetName()==symbol.GetName())
                return index;
        }
        return -1;
    }

    void BlockScan(vector<BasicBlockQuadruple> bblock)
    {
        const int bb_size = bblock.size();
        // if(bb_size==1&&((int)bblock[0].GetQuadruple().GetOperation()>4))
        //     return;
        sub_symbols.clear();
        vector<Triple> triples; // prepare to add usage and active information for sub_symbols.
        vector<CombinedInfo> infos; // array to store active and usage information for each quadruple.
        int count = 0;
        for(int index=0;index<bb_size;index++)
        {
            // In this loop, all symbols in the current basic block will be cloned into another vector<Symbol>.
            int Op = bblock[0].GetQuadruple().GetOperation();
            if(Op==0)
            {
                // Op: assignment, only Src and Addr is needed.
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetSrc()]);
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetAddr()]);
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                infos.push_back(CombinedInfo(1,0,1));
            }
            else if (Op>0&&Op<5)
            {
                // Op: arithmetic computing, Src, Dst and Addr ara needed.
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetSrc()]);
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetDst()]);
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetAddr()]);
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                infos.push_back(CombinedInfo(1,1,1));
            }
            else if ((Op>4&&Op<10)||Op==11)
            {
                // Op: jump expression except unconditioned jump.
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetSrc()]);
                sub_symbols.push_back(symbols[bblock[0].GetQuadruple().GetDst()]);
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                infos.push_back(CombinedInfo(1,1,0));
            }
        }
        Active lastASrc,lastADst,lastAAddr;
        lastASrc=lastADst=lastAAddr=Active::INACTIVE;
        Usage lastUSrc,lastUDst,lastUAddr;
        lastUSrc=lastUDst=lastUAddr=Usage::HANGING;
        for(int index = bb_size-1;index>=0;index--)
        {
            if(index == (bb_size-1))
            {
                // the last expression, which may contains the export variable.
                if(bblock[index].GetQuadruple().GetOperation()==0)
                {
                    triples[IndexInSubSymbols(symbols[bblock[index].GetQuadruple().GetAddr()])].active = Active::ACTIVE;
                    infos[index].src.active = Active::INACTIVE;
                    infos[index].src.usage = Usage::HANGING;
                    infos[index].addr.active = Active::ACTIVE;
                    infos[index].addr.usage = Usage::HANGING;
                }
                else 
                {

                }
            }
        }



        int size = bb_size-1;
        while(size>=0)
        {
            AllocateRegister(bblock[size]);
            size --;
        }
        Reg_BX::used = false;
        Reg_DX::used = false;
    }
    void AllocateRegister(BasicBlockQuadruple quadruple)
    {

    }
}
