#ifndef __PARSER_H_INCLUDE__
#define __PARSER_H_INCLUDE__

#include<stdbool.h>

#define PARSER_SYMBOL_MAX_SIZE (255)
#define PARSER_MNEMONIC_MAX_SIZE (3)

enum parser_commandtype{
    parser_COMMAND_A,
    parser_COMMAND_C,
    parser_COMMAND_L
};

typedef struct parserstr* PARSER;
typedef enum parser_commandtype PARSER_COMMANDTYPE;

PARSER parser_init(FILE* asmfp);
bool parser_hasmorecommands(PARSER thisobject);
void parser_advance(PARSER thisobject);
PARSER_COMMANDTYPE parser_commandtype(PARSER thisobject);
void parser_symbol(PARSER thisobject, char symbol[]);
void parser_dest(PARSER thisobject, char dest[]);
void parser_comp(PARSER thisobject, char comp[]);
void parser_jump(PARSER thisobject, char jump[]);

#endif