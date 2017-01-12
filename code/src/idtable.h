#include <string>
#include <sstream>
#include <vector>
using namespace std;
namespace idtable
{
    enum SymbolType
    {
        IDENTIFIER = 2,
        CONST = 1
    };

    class Symbol
    {
    private:
        static int count;
        int Index;
        SymbolType Type;
        string Name;
    public: 
        Symbol(SymbolType,string);
        Symbol(SymbolType,double);
        string GetName();
        double GetValue();
        int GetType();
        int GetIndex();
        void SetType(SymbolType);
    };
    int Symbol::count = 0;
    Symbol::Symbol(SymbolType type,string name)
    {
        Index = count;
        Type = type;
        Name = name;
        Symbol::count++;
    }
    void Symbol::SetType(SymbolType type)
    {
        Type = type;
    }
    Symbol::Symbol(SymbolType type,double value)
    {
        Index = count;
        Type = type;
        stringstream ss;
        ss.clear();
        ss<<value;
        Name = ss.str();
        Symbol::count++;
    }
    int Symbol::GetType()
    {
        return (int)Type;
    }
    string Symbol::GetName()
    {
        return Name;
    }
    double Symbol::GetValue()
    {
        const char* dstr = Name.data();
        double value = 0;
        int index = 0;
        int length = strlen(dstr);
        bool integerflag = true;
        int divcount = 1;
        while(index<length)
        {
            switch(dstr[index])
            {
                case '.':integerflag = false;index++; continue;
                default: break;
            }
            double curr = (int)dstr[index] - 48;
            if(integerflag)
            {
                value  = value * 10 + curr;
            }
            else
            {
                value = value + curr / (divcount * 10);
                divcount++;
            }
            index++;
        }
        return value;
    }
    int Symbol::GetIndex()
    {
        return Index;
    }
}