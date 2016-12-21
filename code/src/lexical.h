#include <stdio.h>
void fun()
{
    printf("Hello");
}
typedef struct Symbol{
    int index=0;
    int Identifier=0;
    char* Symbol=0;
    struct Symbol* next=0;
    struct Symbol* previous = 0;
};
typedef struct SymbolTable{
    Symbol* HeadSymbol;
} SBT;