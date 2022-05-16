#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

#include "parser.h"
#include "codewriter.h"

#define VMFILE_NAME_MAX_SIZE 255
#define ASMFILE_NAME_MAX_SIZE (VMFILE_NAME_MAX_SIZE + 1)

bool isVMfile(char source_name[]);
bool hasDotInitial(char string[]);
void writeCode(char VMfile_name[], CODEWRITER codewriter);
void nameOutPutFile(char asmfile_name[], char* source_name);

/*************************************************************************************
    ディレクトリ名を指定された場合, カレントディレクトリの直下にあることを想定している.
    VMファイルは255字以内であることを想定している.
    ディレクトリを指定された場合, [directory/[Prog].vm]が255字以内でないとならない.
*************************************************************************************/

int main (int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Usage: VMtranslater [Source]\n");
        exit(1);
    }

    char* source_name = argv[1];
    FILE* asmfp;
    char VMfile_name[VMFILE_NAME_MAX_SIZE + 1];
    char asmfile_name[ASMFILE_NAME_MAX_SIZE + 1];

    nameOutPutFile(asmfile_name, source_name);
    
    asmfp = fopen(asmfile_name, "w");
    if(asmfp == NULL){
        fprintf(stderr, "Cannot Open %s.\n", asmfile_name);
        exit(1);
    }

    CODEWRITER codewriter = codewriter_init(asmfp);

    if(isVMfile(source_name)){
        //VMファイルを指定された場合の処理
        strcpy(VMfile_name, source_name);
        writeCode(VMfile_name, codewriter);
    }else{
        //ディレクトリを指定された場合の処理
        DIR *VMdp;
        struct dirent *VMdirst;
        VMdp = opendir(source_name);

        if(VMdp == NULL){
            fprintf(stderr, "Cannot open the folder %s.", source_name);
            exit(1);
        }

        VMdirst = readdir(VMdp);
        while(VMdirst != NULL){
            if(hasDotInitial(VMdirst->d_name)){
                VMdirst = readdir(VMdp);
                continue;
            }

            if(isVMfile(VMdirst->d_name)){
                strcpy(VMfile_name, source_name);
                strcat(VMfile_name, "/");
                strcat(VMfile_name, VMdirst->d_name);
                writeCode(VMfile_name, codewriter);
            }
            VMdirst = readdir(VMdp);
        }
        closedir(VMdp);
    }
    fclose(asmfp);

    return 0;
}

bool isVMfile(char source_name[]){
    if( strlen(source_name) < 3 ){
        return false;
    }
    if( strcmp(".vm",source_name + (strlen(source_name)-strlen(".vm"))) == 0){
        return true;
    }
    return false;
}

void writeCode(char VMfile_name[], CODEWRITER codewriter){
    FILE* VMfp;
    
    VMfp = fopen(VMfile_name, "r");
    
    if(VMfp == NULL){
        fprintf(stderr, "Cannot open %s.\n", VMfile_name);
        exit(1);
    }

    STREAM stream = parser_init(VMfp);       
    codewriter_setFileName(codewriter, VMfile_name);

    char command[PARSER_COMMAND_MAX_SIZE + 1];
    char arg1[PARSER_ARGUMENT_MAX_SIZE + 1];
    int arg2;

    codewriter_writeInit(codewriter);

    while(parser_hasMoreCommands(stream)){
        parser_advance(stream);

        parser_command(stream, command);
        parser_arg1(stream, arg1);
        arg2 = parser_arg2(stream);

        switch(parser_commandType(stream)){
            case C_PUSH:
                codewriter_writePush(codewriter, arg1, arg2);
                break;
            case C_POP:
                codewriter_writePop(codewriter, arg1, arg2);
                break;
            case C_ARITHMETIC:
                codewriter_writeArithmetic(codewriter, command);
                break;
            case C_GOTO:
                codewriter_writeGoto(codewriter, arg1);
                break;
            case C_LABEL:
                codewriter_writeLabel(codewriter, arg1);
                break;
            case C_IF:
                codewriter_writeIf(codewriter, arg1);
                break;
            case C_RETURN:
                codewriter_writeReturn(codewriter);
                break;
            case C_FUNCTION:
                codewriter_writeFunction(codewriter, arg1, arg2);
                break;
            case C_CALL:
                codewriter_writeCall(codewriter, arg1, arg2);
                break;
        }
    }

    fclose(VMfp);
}

void nameOutPutFile(char asmfile_name[], char* source_name){
    if(isVMfile(source_name)){
        strncpy(asmfile_name, source_name, strlen(source_name)-strlen(".vm"));
        asmfile_name[strlen(source_name)-strlen(".vm")] = '\0';
        strcat(asmfile_name, ".asm");
    }else{
        strcpy(asmfile_name, source_name);
        strcat(asmfile_name,".asm");
    }
}

bool hasDotInitial(char string[]){
    if(string[0] == '.')
        return true;
    else
        return false;
}