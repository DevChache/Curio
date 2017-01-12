#include <string>
#include <vector>
#include <fstream>
#include "idtable.h"
using namespace std;
using namespace idtable;
namespace quadruple
{
    enum Operation
    {
        ASN = 0,
        ADD = 1,
        SUB = 2,
        MUL = 3,
        DIV = 4,
        JLT = 5, // Jump-LessThan
        JLE = 6, // Jump-LessOrEqual
        JGT = 7, // Jump-GreaterThan
        JGE = 8, // Jump-GreaterOrEqual
        JE = 9, // Jump-Equal
        J = 10, // Jump
        JNE = 11, // Jump-NotEqual
    };

    // Declaration of class Equ.
    class Equ
    {
    public:
        string Op; // Operator string, equal to the enum Operation. 
        string StrLeft; // Frist value.
        string StrRight; // Second value.
        string result; // Target of the quadruple.

        // Summary: default constructor.
        Equ(string,string,string,string);
    };

    class Quadruple
    {
    private:
        static int count;
        int Idx;
        Operation Opt;
        int Src;
        int Dst;
        int Addr;
        // Summary: return integer value of the given string.
        int Convert(string);
    public:
        // Summary: default constructor.
        Quadruple();

        // Summary: constructor of Quadruple, convert Equ object to Quadruple object.
        Quadruple(quadruple::Equ);

        // Summary: return index of current quadruple.
        int GetIndex();

        // Summary: return operation of current quadruple.
        Operation GetOperation();

        // Summary: return source target of current quadruple.
        int GetSrc();

        // Summary: return destination target of current quadruple.
        int GetDst();

        // Summary: return entry address of current quadruple.
        int GetAddr();
    };
    int quadruple::Quadruple::count = 0; // Initialize the static data member of Quadruple.

    class BasicBlockQuadruple
    {
    private:
        Quadruple qple;
        static int count;
        int idx;
        bool isEntrance;
    public:
        // Summary: constructor of BasicBlockQuadruple, assign value to its private member.
        BasicBlockQuadruple(Quadruple,bool);
        
        // Summary: return the index of the BasicBlockQuadruple.
        int GetIndex();

        // Summary: return the quadruple object.
        Quadruple GetQuadruple();

        // Summary: return if the current object an entrance of a basic block.
        bool IsEntrance();

        // Summary: set isEntrance property to true when the quadruple is referenced in a J** quadruple.
        void SetBasic();
    };
    int BasicBlockQuadruple::count = 0;





    quadruple::Equ::Equ(string a,string b,string c,string d):Op(a),StrLeft(b),StrRight(c),result(d){};

    int quadruple::Quadruple::Convert(string src)
    {
        const char* strv = src.data();
        int size = strlen(strv);
        int result = 0;
        for(int index(0);index<size;index++)
        {
            result = result * 10 + ((int)strv[index]-48);
        }
        return result;
    }

    // Summary: default constructor of Quadruple.
    quadruple::Quadruple::Quadruple()
    {
        Idx = -1;
        Src = -1;
        Dst = -1;
        Opt = Operation::ADD; // In default, assign an ADD operator to the Op value.
    }

    // Summary: constructor of Quadruple, convert Equ object to Quadruple object.
    quadruple::Quadruple::Quadruple(quadruple::Equ equ)
    {
        Idx = Quadruple::count;
        Opt = (quadruple::Operation)Convert(equ.Op);
        Src = Convert(equ.StrLeft);
        Dst = Convert(equ.StrRight);
        Addr = Convert(equ.result);
        Quadruple::count++;
    }

    // Summary: return index of current quadruple.
    int quadruple::Quadruple::GetIndex()
    {
        return Idx;
    }

    // Summary: return the quadruple object.
    quadruple::Quadruple quadruple::BasicBlockQuadruple::GetQuadruple()
    {
        return qple;
    }

    // Summary: return operation of current quadruple.
    quadruple::Operation quadruple::Quadruple::GetOperation()
    {
        return Opt;
    }

    // Summary: return source target of current quadruple.
    int quadruple::Quadruple::GetSrc()
    {
        return Src;
    }

    // Summary: return destination target of current quadruple.
    int quadruple::Quadruple::GetDst()
    {
        return Dst;
    }

    // Summary: return entry address of current quadruple.
    int quadruple::Quadruple::GetAddr()
    {
        return Addr;
    }



    // Summary: constructor of BasicBlockQuadruple, assign value to its private member.
    quadruple::BasicBlockQuadruple::BasicBlockQuadruple(Quadruple qpl,bool is_entrance = false)
    {
        qple = qpl;
        idx = BasicBlockQuadruple::count;
        BasicBlockQuadruple::count++;
        isEntrance = is_entrance;
    }

    // Summary: return the index of the BasicBlockQuadruple.
    int quadruple::BasicBlockQuadruple::GetIndex()
    {
        return idx;
    }

    // Summary: return if the current object an entrance of a basic block.
    bool quadruple::BasicBlockQuadruple::IsEntrance()
    {
        return isEntrance;
    }

    // Summary: set isEntrance property to true when the quadruple is referenced in a J** quadruple.
    void quadruple::BasicBlockQuadruple::SetBasic()
    {
        isEntrance = true;
    }

}



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
    
}
