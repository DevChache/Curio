#include <stdio.h>
#include <string.h>
#include "src/lexical.h"
#include "src/syntax.h"
#include "src/generator.h"
#include "src/semantic.h"

#define COMPILER_NAME "Curio"
#define VERSION "0.0.0"
#define ERROR_SYNTAX "Error: syntax error.\n"
#define ERROR_PARAMETER "Error: parameter error.\n"


char* TARGET_FILE = 0;
char* SOURCE_FILE = 0;


int main(int argc,char* argv[])
{
    if(2==argc)
    {
        int len = strlen(argv[1]);
        SOURCE_FILE = new char[len*sizeof(char)];
        strcpy(SOURCE_FILE, argv[1]);
        char* str = new char[sizeof(char)*strlen(SOURCE_FILE)];
        strncpy(str,SOURCE_FILE,strlen(SOURCE_FILE)-6);
        str[strlen(SOURCE_FILE)-6]='\0';
        TARGET_FILE = strcat(str,".run");
    }
    else if(4==argc && strcmp(argv[2],"-o")==0)
    {
        TARGET_FILE = argv[3];
        SOURCE_FILE = argv[1];
    }
    else
    {
        printf(ERROR_PARAMETER);
        return 1;
    }
    if(strstr(SOURCE_FILE,".curio")<=0) {
        printf(ERROR_SYNTAX);
        return 1;
    }
    printf("=> Compile %s to %s ...\n",SOURCE_FILE, TARGET_FILE);
    // TODO: Add operations for lexical analysis and semantic analysis, syntax analysis.
    return 0;
}