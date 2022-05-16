#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

struct stream{
    FILE* VMfp;
    char command[PARSER_COMMAND_MAX_SIZE + 1];
    char arg1[PARSER_ARGUMENT_MAX_SIZE + 1];
    char arg2[PARSER_ARGUMENT_MAX_SIZE + 1];
};

bool isSpace(FILE *VMfp);
bool isEOL(FILE *VMfp);
bool isEOF(FILE *VMfp);
bool isComment(FILE *VMfp);
void skipSpace(FILE *VMfp);
void skipEOL(FILE *VMfp);
void skipComment(FILE *VMfp);
void nextAdvance(FILE *VMfp);

STREAM parser_init(FILE *VMfp){
    static struct stream this_object_inner;
    this_object_inner.VMfp = VMfp;
    strcpy(this_object_inner.command,"");
    strcpy(this_object_inner.arg1,"");
    strcpy(this_object_inner.arg2,"");
    nextAdvance(this_object_inner.VMfp);
    return &this_object_inner;
}

bool parser_hasMoreCommands(STREAM this_object){
    if(isEOF(this_object->VMfp))
        return false;
    else
        return true;
}

void parser_advance(STREAM this_object){
    int i=0;
    //コマンドの抽出
    while(!(isComment(this_object->VMfp)||isEOF(this_object->VMfp)||isEOL(this_object->VMfp))){
        if(i>=PARSER_COMMAND_MAX_SIZE){
            fprintf(stderr, "Max Command size is %d.\n", PARSER_COMMAND_MAX_SIZE);
            exit(1);
        }
        if(isSpace(this_object->VMfp)){
            skipSpace(this_object->VMfp);
            break;
        }
        this_object->command[i] = (char) fgetc(this_object->VMfp);
        i++;
    }
    this_object->command[i] = '\0';

    //第一引数の抽出
    i=0;
    while(!(isComment(this_object->VMfp)||isEOF(this_object->VMfp)||isEOL(this_object->VMfp))){
        if(i>=PARSER_ARGUMENT_MAX_SIZE){
            fprintf(stderr, "Max argument size is %d.\n", PARSER_ARGUMENT_MAX_SIZE);
            exit(1);
        }
        if(isSpace(this_object->VMfp)){
            skipSpace(this_object->VMfp);
            break;
        }
        this_object->arg1[i] = (char) fgetc(this_object->VMfp);
        i++;
    }
    this_object->arg1[i] = '\0';

    //第二引数の抽出
    i=0;
    while(!(isComment(this_object->VMfp)||isEOF(this_object->VMfp)||isEOL(this_object->VMfp))){
        if(i>=PARSER_ARGUMENT_MAX_SIZE){
            fprintf(stderr, "Max argument size is %d.\n", PARSER_ARGUMENT_MAX_SIZE);
            exit(1);
        }
        if(isSpace(this_object->VMfp)){
            skipSpace(this_object->VMfp);
            break;
        }
        this_object->arg2[i] = (char) fgetc(this_object->VMfp);
        i++;
    }
    this_object->arg2[i] = '\0';

    nextAdvance(this_object->VMfp);
}


COMMANDTYPE parser_commandType(STREAM this_object){
    if(strcmp(this_object->command,     "push") == 0) return C_PUSH;
    if(strcmp(this_object->command,      "pop") == 0) return C_POP;
    if(strcmp(this_object->command,    "label") == 0) return C_LABEL;
    if(strcmp(this_object->command,     "goto") == 0) return C_GOTO;
    if(strcmp(this_object->command,  "if-goto") == 0) return C_IF;
    if(strcmp(this_object->command, "function") == 0) return C_FUNCTION;
    if(strcmp(this_object->command,     "call") == 0) return C_CALL;
    if(strcmp(this_object->command,   "return") == 0) return C_RETURN;
    else                                              return C_ARITHMETIC;
}

void parser_command(STREAM this_object, char command[]){
    strcpy(command, this_object->command);
}

void parser_arg1(STREAM this_object, char arg1[]){
    strcpy(arg1, this_object->arg1);
}

int parser_arg2(STREAM this_object){
    return atoi(this_object->arg2);
}


bool isSpace(FILE *VMfp){
    char c = fgetc(VMfp);
    ungetc(c, VMfp);
    if(c == ' '||c == '\t')
        return true;
    else
        return false;
}

bool isEOL(FILE *VMfp){
    char c = fgetc(VMfp);
    ungetc(c,VMfp);
    if(c == '\n')
        return true;
    else
        return false;
}

bool isEOF(FILE *VMfp){
    char c = fgetc(VMfp);
    ungetc(c,VMfp);
    if(c == EOF)
        return true;
    else 
        return false;
}

bool isComment(FILE *VMfp){
    char c1 = fgetc(VMfp);
    if(c1 == '/'){
        char c2 = fgetc(VMfp);
        if(c2 == '/'){
            ungetc(c2, VMfp);
            ungetc(c1, VMfp);
            return true;
        }else{
            ungetc(c2, VMfp);
            ungetc(c1, VMfp);
            return false;
        }
        
    }else{
        ungetc(c1, VMfp);
        return false;
    }
}

void skipSpace(FILE *VMfp){
    while(isSpace(VMfp))
        fgetc(VMfp);
}

void skipEOL(FILE *VMfp){
    while(isEOL(VMfp))
        fgetc(VMfp);
}

void skipComment(FILE *VMfp){
    if(isComment(VMfp)){
        while(!(isEOL(VMfp)||isEOF(VMfp)))
            fgetc(VMfp);
    }
}

void nextAdvance(FILE *VMfp){
    while(isComment(VMfp)||isEOL(VMfp)||isSpace(VMfp)){
        skipComment(VMfp);
        skipEOL(VMfp);
        skipSpace(VMfp);
    }
}