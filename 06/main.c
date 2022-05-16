#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.h"
#include"code.h"

#define ASM_FILENAME_MAX_SIZE (255)
#define HACK_FILENAME_MAX_SIZE (ASM_FILENAME_MAX_SIZE + 1)
#define HACK_ADDRESS_SIZE (16)
#define HACK_A_COMMAND_VALUE_SIZE (15)

void assembler(FILE* asmfp, FILE* hackfp);
void assembler_COMMAND_C(PARSER parser, FILE* hackfp);
void assembler_COMMAND_A(PARSER parser, FILE* hackfp);
void string_to_binary(char* txtnum, char* binary);

/*****************************************************
    main関数の主な機能:
    [1]エラーチェックを行う.
    [2][Prog].asmを[Prog].hackに変換する.
*****************************************************/

int main(int argc, char* argv[]){
    char* asmfilename;
    char hackfilename[HACK_FILENAME_MAX_SIZE+1];
    FILE* asmfp;
    FILE* hackfp;

    /* コマンドライン引数が2つでないといけない */
    if (argc != 2){
        fprintf(stderr,"Usage: assembler [Prog].asm\n");
        exit(1);
    }

    asmfilename = argv[1];

    /* 指定ファイル名はASM_FILENAME_MAX_SIZEを超えてはいけない */
    if (strlen(asmfilename) > ASM_FILENAME_MAX_SIZE){
        fprintf(stderr, "Usage: assembler [Prog].asm\n");
        fprintf(stderr, "Your filename is too large. max size is %d.\n", ASM_FILENAME_MAX_SIZE);
        exit(1);
    }

    /* 指定ファイルの拡張子は(.asm)でないといけない */
    if (strlen(asmfilename)<strlen(".asm") ||
            strcmp(asmfilename + (strlen(asmfilename) - strlen(".asm") ),".asm") != 0){
        fprintf(stderr, "Usage: assembler [Prog].asm\n");
        fprintf(stderr, "Wrong extention.\n");
        exit(1);
    }

    strncpy(hackfilename, asmfilename, strlen(asmfilename) - strlen(".asm"));
    hackfilename[strlen(asmfilename) - strlen(".asm")] = '\0';
    strcat(hackfilename,".hack");

    /* [Prog].asmファイルを読み込みモードで開く */
    asmfp = fopen(asmfilename, "r");
    if(asmfp == NULL){
        fprintf(stderr, "(%s) is not exist.", asmfilename);
        exit(1);
    }

    /* [Prog].hackファイルを書き込みモードで開く */
    hackfp = fopen(hackfilename, "w");
    if(hackfp == NULL){
        fprintf(stderr, "(%s) is not available", hackfilename);
        exit(1);
    } 

    printf("hackfilename:%s\n", hackfilename);

    assembler(asmfp, hackfp);

    fclose(asmfp);
    fclose(hackfp);
    return 0;
 }

void assembler(FILE* asmfp, FILE* hackfp){
    PARSER parser = parser_init(asmfp);
    char symbol[PARSER_SYMBOL_MAX_SIZE+1];

    while(parser_hasmorecommands(parser)){
        parser_advance(parser);
        switch(parser_commandtype(parser)){
            case parser_COMMAND_A:
                assembler_COMMAND_A(parser, hackfp);
                break;
            case parser_COMMAND_C:
                assembler_COMMAND_C(parser, hackfp);
                break;
            case parser_COMMAND_L:
                printf("COMMANDTYPE:L\n");
                parser_symbol(parser, symbol);
                printf("%s\n", symbol);
                break;

        }
    }
}

void assembler_COMMAND_A(PARSER parser, FILE* hackfp){
    char symbol[PARSER_SYMBOL_MAX_SIZE+1];
    parser_symbol(parser, symbol);
    
    char symbolbit[HACK_A_COMMAND_VALUE_SIZE+1];

    // symbol(の1文字目が)数字である場合
    if(symbol[0]>=(int)'0'&&symbol[0]<= (int)'9'){
        int number = atoi(symbol);
        symbolbit[HACK_A_COMMAND_VALUE_SIZE] = '\0';
        for(int i=1; i<=HACK_A_COMMAND_VALUE_SIZE; i++){
            if(number % 2 == 1)
                symbolbit[HACK_A_COMMAND_VALUE_SIZE-i] = '1';
            else
                symbolbit[HACK_A_COMMAND_VALUE_SIZE-i] = '0';
            number = number / 2;
        }
    }
    
    char binary[HACK_ADDRESS_SIZE+1]="0";
    strcat(binary, symbolbit);
    fprintf(hackfp, "%s\n", binary);
}

void assembler_COMMAND_C(PARSER parser, FILE* hackfp){
    char dest[PARSER_MNEMONIC_MAX_SIZE+1];
    char comp[PARSER_MNEMONIC_MAX_SIZE+1];
    char jump[PARSER_MNEMONIC_MAX_SIZE+1];
    char destbit[CODE_DEST_BIT_SIZE+1];
    char compbit[CODE_COMP_BIT_SIZE+1];
    char jumpbit[CODE_JUMP_BIT_SIZE+1];
    
    parser_dest(parser, dest);
    code_dest(dest,destbit);
    parser_comp(parser, comp);
    code_comp(comp,compbit);
    parser_jump(parser, jump);
    code_jump(jump,jumpbit);

    char binary[HACK_ADDRESS_SIZE+1] = "111";
    strcat(binary,compbit);
    strcat(binary,destbit);
    strcat(binary,jumpbit);

    fprintf(hackfp,"%s\n",binary);
    
}