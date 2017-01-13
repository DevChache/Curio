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
    bool basic::Reg_BX::used = false;
    struct Reg_DX
    {
        static bool used;
        static Symbol* latest;
    };
    Symbol* basic::Reg_BX::latest = 0;
    Symbol* basic::Reg_DX::latest = 0;
    bool basic::Reg_DX::used = false;
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
        int next_quadruple_addr= -1;
        Triple(Active,Usage);
        Triple();
    };
    int basic::Triple::count = 0;
    Triple::Triple(Active act,Usage usg):active(act),usage(usg){index = Triple::count;count++;}
    Triple::Triple(){active = Active::INACTIVE;usage = Usage::HANGING;}
    
    class Register
    {
    public:
        int index;
        Quadruple quadruple;
        Reg reg;
        Register(Quadruple,Reg,int);
        Register();
        bool force = false;
        void ForceMove();
    };

    Register::Register(Quadruple qpl,Reg r,int i):quadruple(qpl),reg(r)
    {
        index = i;
        if(reg==Reg::BX)
            Reg_BX::used = true;
        else Reg_DX::used = true;
    }
    Register::Register()
    {

    }
    void Register::ForceMove()
    {
        force = true;
    }

    vector<Register> registers;


    // Store index in symbol file and keeps the information of active status and usage.
    class SymbolReference
    {
    public:
        Active active;
        Usage usage;
        int symbol_addr;
        SymbolReference(Active,Usage,int);
        SymbolReference();
    };
    SymbolReference::SymbolReference(Active a,Usage u,int i)
    {
        active = a;
        usage = u;
        symbol_addr = i;
    }
    SymbolReference::SymbolReference()
    {
        active = Active::INACTIVE;
        usage = Usage::HANGING;
        symbol_addr = -1;
    }

    // Combine active info and usage info in one object for one quadruple.
    class CombinedInfo
    {
    public:
        int array[3];
        SymbolReference src;
        SymbolReference dst;
        SymbolReference addr;
        CombinedInfo(int,int,int);
    };
    CombinedInfo::CombinedInfo(int s,int d,int a)
    {
        array[0] = s;
        array[1] = d;
        array[2] = a;
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
    Register AllocateRegister(BasicBlockQuadruple,vector<CombinedInfo>,int);

    // Summary: generate assembly language file a.assembly.asm
    void Generate(vector<BasicBlockQuadruple>);

    // Summary: get allocated register
    string GetReg(int);


    void CloneSymbols(vector<Symbol> symbls)
    {
        basic::symbols = symbls;
    }

    void BlockDiv(vector<BasicBlockQuadruple> original_blocks)
    {
        fstream stream("a.assembly.asm",ios::out|ios::trunc);
        stream << "";
        stream.close();
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
            int Op = bblock[index].GetQuadruple().GetOperation();
            if(Op==0)
            {
                // Op: assignment, only Src and Addr is needed.
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetSrc()]);
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetAddr()]);
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                CombinedInfo info(1,0,1);
                info.src.symbol_addr = bblock[index].GetQuadruple().GetSrc();
                info.addr.symbol_addr = bblock[index].GetQuadruple().GetAddr();
                infos.push_back(info);
            }
            else if (Op>0&&Op<5)
            {
                // Op: arithmetic computing, Src, Dst and Addr ara needed.
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetSrc()]);
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetDst()]);
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetAddr()]);
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                CombinedInfo info(1,0,1);
                info.src.symbol_addr = bblock[index].GetQuadruple().GetSrc();
                info.dst.symbol_addr = bblock[index].GetQuadruple().GetDst();
                info.addr.symbol_addr = bblock[index].GetQuadruple().GetAddr();
                infos.push_back(info);
            }
            else if ((Op>4&&Op<10)||Op==11)
            {
                // Op: jump expression except unconditioned jump.
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetSrc()]);
                sub_symbols.push_back(symbols[bblock[index].GetQuadruple().GetDst()]);
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                triples.push_back(Triple(Active::INACTIVE,Usage::HANGING));
                CombinedInfo info(1,0,1);
                info.src.symbol_addr = bblock[index].GetQuadruple().GetSrc();
                info.dst.symbol_addr = bblock[index].GetQuadruple().GetDst();
                infos.push_back(info);
            }
        }
        Active lastASrc,lastADst,lastAAddr;
        lastASrc=lastADst=lastAAddr=Active::INACTIVE;
        Usage lastUSrc,lastUDst,lastUAddr;
        lastUSrc=lastUDst=lastUAddr=Usage::HANGING;
        for(int index = bb_size-1;index>=0;index--)
        {
            // in this loop compiler scan from the last quadruple to the first one the analysis the active info and usage.
            if(index == (bb_size-1))
            {
                Quadruple current = bblock[index].GetQuadruple();
                // the last expression, which may contains the export variable.
                if(current.GetOperation()==0)
                {
                    // [NOTICE] in order to show the logic, the following steps are briefable but I insist to show the whole logic.

                    // Init export value in symbol table to ACTIVE,HANGING;
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].active = Active::ACTIVE;

                    // copy symbol table values to combinedinfos of the last quadruple.
                    infos[index].src.active = Active::INACTIVE;
                    infos[index].src.usage = Usage::HANGING;
                    infos[index].addr.active = Active::ACTIVE;
                    infos[index].addr.usage = Usage::HANGING;

                    // Set Addr to INACTIVE,HANGING and set Src/Dst to ACTIVE,next_quadruple_addr = last reference (in sub_symbols).
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].active = Active::INACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].active = Active::ACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].usage = Usage::WAITING;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].next_quadruple_addr = index;
                }
                else if(current.GetOperation()<5)
                {
                    // [NOTICE] the logic here is optimized.
                    // In this section, all quadruples has 3 symbols.

                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].active = Active::ACTIVE;
                    // copy symbol table values to combinedinfos of the last quadruple.
                    infos[index].src.active = Active::INACTIVE;
                    infos[index].src.usage = Usage::HANGING;
                    infos[index].dst.active = Active::ACTIVE;
                    infos[index].dst.usage = Usage::HANGING;
                    infos[index].addr.active = Active::ACTIVE;
                    infos[index].addr.usage = Usage::HANGING;
                    // Set Addr to INACTIVE,HANGING and set Src/Dst to ACTIVE,next_quadruple_addr = last reference (in sub_symbols).
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].active = Active::INACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].active = Active::ACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].usage = Usage::WAITING;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].next_quadruple_addr = index;
                    triples[IndexInSubSymbols(symbols[current.GetDst()])].active = Active::ACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetDst()])].usage = Usage::WAITING;
                    triples[IndexInSubSymbols(symbols[current.GetDst()])].next_quadruple_addr = index;
                }
                else if(current.GetOperation()>4&&current.GetOperation()!=10)
                {
                    // I dont't think a jump expression needs to be analysised about its active status and usage.
                }
            }
            else if(index < (bb_size-1))
            {
                Quadruple current = bblock[index].GetQuadruple();
                if(current.GetOperation()==0)
                {
                    // copy symbol table values to combinedinfos of the last quadruple.
                    infos[index].src.active = triples[IndexInSubSymbols(symbols[current.GetSrc()])].active;
                    infos[index].src.usage = triples[IndexInSubSymbols(symbols[current.GetSrc()])].usage;
                    infos[index].addr.active = triples[IndexInSubSymbols(symbols[current.GetAddr()])].active;
                    infos[index].addr.usage = triples[IndexInSubSymbols(symbols[current.GetAddr()])].usage;

                    // Set Addr to INACTIVE,HANGING and set Src to ACTIVE,next_quadruple_addr = last reference (in sub_symbols).
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].active = Active::INACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].usage = Usage::HANGING;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].active = Active::ACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].usage = Usage::WAITING;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].next_quadruple_addr = index;
                }
                else if(current.GetOperation()<5)
                {
                    // In this section, all quadruples has 3 symbols.

                    // copy symbol table values to combinedinfos of the last quadruple.
                    infos[index].src.active = triples[IndexInSubSymbols(symbols[current.GetSrc()])].active;
                    infos[index].src.usage = triples[IndexInSubSymbols(symbols[current.GetSrc()])].usage;
                    infos[index].addr.active = triples[IndexInSubSymbols(symbols[current.GetAddr()])].active;
                    infos[index].addr.usage = triples[IndexInSubSymbols(symbols[current.GetAddr()])].usage;
                    infos[index].dst.active = triples[IndexInSubSymbols(symbols[current.GetDst()])].active;
                    infos[index].dst.usage = triples[IndexInSubSymbols(symbols[current.GetDst()])].usage;
                    // Set Addr to INACTIVE,HANGING and set Src/Dst to ACTIVE,next_quadruple_addr = last reference (in sub_symbols).
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].active = Active::INACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetAddr()])].usage = Usage::HANGING;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].active = Active::ACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].usage = Usage::WAITING;
                    triples[IndexInSubSymbols(symbols[current.GetSrc()])].next_quadruple_addr = index;
                    triples[IndexInSubSymbols(symbols[current.GetDst()])].active = Active::ACTIVE;
                    triples[IndexInSubSymbols(symbols[current.GetDst()])].usage = Usage::WAITING;
                    triples[IndexInSubSymbols(symbols[current.GetDst()])].next_quadruple_addr = index;
                }
            }
        }
        

        int size = 0;
        while(size<bb_size)
        {
            if(bblock[size].GetQuadruple().GetOperation()<5)
                registers.push_back(AllocateRegister(bblock[size],infos,size));
            size ++;
        }
        Reg_BX::used = false;
        Reg_DX::used = false;
        Generate(bblock);
    }

    Register AllocateRegister(BasicBlockQuadruple bquadruple,vector<CombinedInfo> infos,int index)
    {
        if(!Reg_BX::used)
        {
            // if BX is not in use, allocate register BX.
            basic::Reg_BX::latest = &symbols[bquadruple.GetQuadruple().GetAddr()];
            return Register(bquadruple.GetQuadruple(),Reg::BX,index);
        }
        else if (!Reg_DX::used)
        {
            // if DX is not in use and BX is occupied, allocate DX.
            basic::Reg_DX::latest = &symbols[bquadruple.GetQuadruple().GetAddr()];
            return Register(bquadruple.GetQuadruple(),Reg::DX,index);
        }
        else if(Reg_BX::latest==&symbols[bquadruple.GetQuadruple().GetAddr()]||(infos[index].addr.active==Active::INACTIVE&&infos[index].addr.usage==Usage::HANGING))
        {
            basic::Reg_BX::latest = &symbols[bquadruple.GetQuadruple().GetAddr()];
            return Register(bquadruple.GetQuadruple(),Reg::BX,index);
        }
        else if(Reg_DX::latest==&symbols[bquadruple.GetQuadruple().GetAddr()]||(infos[index].addr.active==Active::INACTIVE&&infos[index].addr.usage==Usage::HANGING))
        {
            basic::Reg_DX::latest = &symbols[bquadruple.GetQuadruple().GetAddr()];
            return Register(bquadruple.GetQuadruple(),Reg::DX,index);
        }
        else {
            // [NOTICE] force the variable in Register to be moved to memory, and allocate register bx.
            Reg_BX::latest = &symbols[bquadruple.GetQuadruple().GetAddr()];
            Register reg(bquadruple.GetQuadruple(),Reg::BX,index);
            reg.ForceMove();
            return reg;
        }
    }

    void Generate(vector<BasicBlockQuadruple> bbq)
    {
        stringstream ss;
        ss.clear();
        const int size  = bbq.size();
        for(int index = 0;index<size;index++)
        {
            Quadruple current = bbq[index].GetQuadruple();
            switch(current.GetOperation())
            {
                case quadruple::Operation::ASN:
                {
                    ss << "MOV " << GetReg(index) << ", [" << current.GetSrc() << "]\n";break;
                }
                case quadruple::Operation::ADD:
                {
                    string reg = GetReg(index);
                    ss << "MOV " << reg << ", [" << current.GetSrc() << "]\n";
                    ss << "ADD " << reg << ", ["<< current.GetDst() << "]\n";
                    ss << "MOV [" << current.GetAddr() << "], "<< reg <<"\n";break;
                }
                case quadruple::Operation::SUB:
                {
                    string reg = GetReg(index);
                    ss << "MOV " << reg << ", [" << current.GetSrc() << "]\n";
                    ss << "SUB " << reg << ", ["<< current.GetDst() << "]\n";
                    ss << "MOV [" << current.GetAddr() << "], "<< reg <<"\n";break;
                    break;
                }

                case quadruple::Operation::MUL:
                {
                    string reg = GetReg(index);
                    ss << "MOV " << reg << ", [" << current.GetSrc() << "]\n";
                    ss << "MUL " << reg << ", ["<< current.GetDst() << "]\n";
                    ss << "MOV [" << current.GetAddr() << "], "<< reg <<"\n";break;
                    break;
                }

                case quadruple::Operation::DIV:
                {
                    string reg = GetReg(index);
                    ss << "MOV " << reg << ", [" << current.GetSrc() << "]\n";
                    ss << "DIV " << reg << ", ["<< current.GetDst() << "]\n";
                    ss << "MOV [" << current.GetAddr() << "], "<< reg <<"\n";break;
                    break;
                }

                case quadruple::Operation::JLT:
                {
                    string reg = GetReg(index);
                    ss << "MOV " << reg << ", [" << current.GetSrc() << "]\n";
                    ss << "CMP " << reg << ", ["<< current.GetDst() << "]\n";
                    ss << "JL [" << current.GetAddr() << "], "<< reg <<"\n";break;
                    break;
                }
            }
        }
        string str = ss.str();
        
        fstream stream("a.assembly.asm",ios::out|ios::app);
        if(stream.good())
        {
            stream.clear();
            stream <<str.data();
            stream.close();
        }
    }

    string GetReg(int index)
    {
        Register reg;
        for(int idx=0;idx<registers.size();idx++)
        {
            if(registers[idx].index == index)
            {
                reg = registers[idx];
                break;
            }
        }
        return reg.reg == Reg::BX? "BX":"DX";
    }
}
