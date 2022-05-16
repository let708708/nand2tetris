#ifndef __PARSER_H_INCLUDE__
#define __PARSER_H_INCLUDE__

#include<stdbool.h>

#define PARSER_COMMAND_MAX_SIZE 255
#define PARSER_ARGUMENT_MAX_SIZE 255    

enum parser_commandType{
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

typedef struct stream * STREAM;
typedef enum parser_commandType COMMANDTYPE;

STREAM parser_init(FILE *VMfp);
bool parser_hasMoreCommands(STREAM this_object);
void parser_advance(STREAM this_object);
COMMANDTYPE parser_commandType(STREAM this_object);
void parser_command(STREAM this_object, char command[]);
void parser_arg1(STREAM this_object, char arg1[]);
int parser_arg2(STREAM this_object);

#endif