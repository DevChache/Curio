#include <stdio.h> // Debuging includes

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
	Symbol GetHeaderSymbol();
	void AppendSymbol(Symbol* symbol);
	void RemoveSymbol(int index);
	Symbol* LookAhead(Symbol* seeker);
private:
	int _count;
	Symbol* _HeaderSymbol;
	Symbol* _TailSymbol;
};
int SymbolTable::GetSymbolCount()
{
	return _count;
}

Symbol SymbolTable::GetHeaderSymbol()
{
	return *_HeaderSymbol;
}

void SymbolTable::AppendSymbol(Symbol* symbol)
{
	(*symbol).next = NULL;
	(*_TailSymbol).next = symbol;
	_TailSymbol  = symbol;
}
void SymbolTable::RemoveSymbol(int index)
{
	Symbol* Seeker = _HeaderSymbol;
	while (index-- > 0)
	{
		if ((*Seeker).next == NULL)
			return;
		Seeker = (*Seeker).next;
	}
	Seeker = (*Seeker).previous;
	delete (*Seeker).next;
	Seeker->next = NULL;
}
