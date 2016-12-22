#include <stdio.h>

class Symbol
{
public:
    Symbol(int index,int identifier,char* symbol):_index(index),_identifier(identifier),_symbol(symbol){};
    class Symbol* next;
    class Symbol* previous;
    int GetIndex();
    int GetIdentifier();
    char* GetSymbolCharacters();
private:
    int _index; // Index of the symbol in symbol scanning.
    int _identifier; // ID. from Symbol Table.md.
    char* _symbol; // Name of the symbol.
};
int Symbol::GetIndex()
{
    return _index;
}
int Symbol::GetIdentifier()
{
    return _identifier;
}
char* Symbol::GetSymbolCharacters()
{
    return _symbol;
}



class SymbolTable
{
public: 
    SymbolTable();
    int GetSymbolCount();
private:
    int _count;
    Symbol* _HeaderSymbol;
    Symbol* _CurrentSymbol;
    void AppendSymbol(Symbol symbol);
    void RemoveSymbol(int index);
    Symbol LookAhead();
    Symbol* GetHeaderSymbol();
};
int SymbolTable::GetSymbolCount()
{
    return _count;
}
Symbol SymbolTable::LookAhead()
{
    return *_CurrentSymbol;
}
Symbol* SymbolTable::GetHeaderSymbol()
{
    
}
