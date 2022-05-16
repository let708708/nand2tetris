#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "parser.h"

#define PARSER_ADVANCE_MAX_SIZE (PARSER_SYMBOL_MAX_SIZE + 2)

struct parserstr{
    FILE *asmfp;
    char advance[PARSER_ADVANCE_MAX_SIZE+1];
};

bool isSpace(FILE *asmfp);
bool isEOF(FILE *asmfp); 
bool isEOL(FILE *asmfp);
bool isComment(FILE *asmfp);
void skipSpace(FILE *asmfp);
void skipEOL(FILE *asmfp);
void skipComment(FILE *asmfp);
void nextAdvance(FILE *asmfp);

PARSER parser_init(FILE *asmfp){
    static struct parserstr initial;

    initial.asmfp = asmfp;
    fseek(initial.asmfp, 0, SEEK_SET);
    strcpy(initial.advance,"");
    nextAdvance(initial.asmfp);

    return &initial;
}

bool parser_hasmorecommands(PARSER thisobject){
    if(isEOF(thisobject->asmfp))
        return false;
    else
        return true;
}

void parser_advance(PARSER thisobject){
    int i=0;

    strcpy(thisobject->advance, "");
    while(!(isComment(thisobject->asmfp)||isEOF(thisobject->asmfp)||isEOL(thisobject->asmfp))){
        if(!isSpace(thisobject->asmfp)){
            i++;
            if(i>PARSER_ADVANCE_MAX_SIZE){
                fprintf(stderr, "Some Command is too laege.\n");
                fprintf(stderr, "Max size is %d", PARSER_ADVANCE_MAX_SIZE);
                exit(1);
            }
            thisobject->advance[i-1] = (char)fgetc(thisobject->asmfp);
        }
        else{
            fgetc(thisobject->asmfp);
        }
    }
    thisobject->advance[i]='\0';

    nextAdvance(thisobject->asmfp);

    printf("Command:%s\n",thisobject->advance);
}

PARSER_COMMANDTYPE parser_commandtype(PARSER thisobject){
    if(thisobject->advance[0]=='@')
        return parser_COMMAND_A;
    if(thisobject->advance[0]=='(')
        return parser_COMMAND_L;
    else
        return parser_COMMAND_C;
}

void parser_symbol(PARSER thisobject, char symbol[]){
    if(parser_commandtype(thisobject)==parser_COMMAND_A){
        strcpy(symbol, thisobject->advance + 1);
    }
    if(parser_commandtype(thisobject)==parser_COMMAND_L){
        strcpy(symbol, thisobject->advance + 1);
        symbol[strlen(thisobject->advance)-1]='\0';
    }
}

void parser_dest(PARSER thisobject, char dest[]){
    if(strchr(thisobject->advance,(int) '=') == NULL)
        dest[0] = '\0';
    else{
        strncpy(dest,thisobject->advance,(strchr(thisobject->advance,(int)'=') - thisobject->advance)/sizeof(char));
        dest[(strchr(thisobject->advance,(int)'=') - thisobject->advance)/sizeof(char)] = '\0';
    }
}

void parser_comp(PARSER thisobject, char comp[]){
    char* startplace;
    char* endplace;
    
    if(strchr(thisobject->advance,(int)'=')==NULL)
        startplace = thisobject->advance;
    else
        startplace = strchr(thisobject->advance,(int)'=') + 1;
    
    if(strchr(thisobject->advance,(int)';')==NULL)
        endplace = thisobject->advance + strlen(thisobject->advance)-1;
    else
        endplace = strchr(thisobject->advance,(int)';') -1;

    strncpy(comp,startplace,(endplace-startplace+1)/sizeof(char));
    comp[(endplace-startplace+1)/sizeof(char)]='\0';
}

void parser_jump(PARSER thisobject, char jump[]){
    if(strchr(thisobject->advance, (int)';')==NULL)
        jump[0]='\0';
    else
        strcpy(jump,strchr(thisobject->advance, (int)';')+1);
}

bool isSpace(FILE *asmfp){
    int c = fgetc(asmfp);
    ungetc(c,asmfp);
    if ((char)c == ' ' || (char)c == '\b')
        return true;
    else 
        return false;
}

bool isEOF(FILE *asmfp){
    int c = fgetc(asmfp);
    ungetc(c, asmfp);
    if(c == EOF)
        return true;
    else
        return false;
}

bool isEOL(FILE *asmfp){
    int c = fgetc(asmfp);
    ungetc(c, asmfp);
    if((char)c == '\n')
        return true;
    else
        return false;    
}

bool isComment(FILE *asmfp){
    int c1 = fgetc(asmfp);
    if((char)c1 != '/'){
        ungetc(c1, asmfp);
        return false;
    }
    int c2 = fgetc(asmfp);
    if((char)c2 != '/'){
        ungetc(c2, asmfp);
        ungetc(c1, asmfp);
        return false;
    }
    ungetc(c2, asmfp);
    ungetc(c1, asmfp);
    return true;
}

void skipSpace(FILE *asmfp){
    while(isSpace(asmfp)){
        fgetc(asmfp);
    }
}

void skipEOL(FILE *asmfp){
    while(isEOL(asmfp)){
        fgetc(asmfp);
    }
}

void skipComment(FILE *asmfp){
    if(isComment(asmfp))
        do{
            fgetc(asmfp);
        }while(!(isEOL(asmfp)||isEOF(asmfp)));  
}

void nextAdvance(FILE *asmfp){
    while(!isEOF(asmfp)){
        if(!(isSpace(asmfp)||isEOL(asmfp)||isComment(asmfp)))
            break;
        skipEOL(asmfp);
        skipSpace(asmfp);
        skipComment(asmfp);
    }
}