#include <stdio.h>
#include <string.h>
#include "codewriter.h"


struct codewriter{
    FILE *asmfp;
    char file_name[CODEWRITER_VMFILE_NAME_MAX_SIZE+1];
    int count_arithmetic_eq;
    int count_arithmetic_lt;
    int count_arithmetic_gt;
};

CODEWRITER codewriter_init(FILE* asmfp){
    static struct codewriter this_object_inner;
    this_object_inner.asmfp = asmfp;
    strcpy(this_object_inner.file_name,"");
    this_object_inner.count_arithmetic_eq = 0;
    this_object_inner.count_arithmetic_lt = 0;
    this_object_inner.count_arithmetic_gt = 0;
    return &this_object_inner;
}

void codewriter_setFileName(CODEWRITER this_object, char VMfile_name[]){
    char *temp = strrchr(VMfile_name,'/');
   if(temp==NULL){
       temp = VMfile_name;
   }else{
       temp = temp + 1;
   }

    strncpy(this_object->file_name,temp,strlen(temp)-strlen(".vm"));
    this_object->file_name[strlen(temp)-strlen(".vm")] = '\0';
}

void codewriter_writePush(CODEWRITER this_object, char arg1[], int arg2){
    if(strcmp(arg1, "constant")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",arg2);
    }
    if(strcmp(arg1, "local")==0){
        fprintf(this_object->asmfp,"@LCL\nD=M\n@%d\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",arg2);
    }
    if(strcmp(arg1, "argument")==0){
        fprintf(this_object->asmfp,"@ARG\nD=M\n@%d\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",arg2);
    }
    if(strcmp(arg1, "this")==0){
        fprintf(this_object->asmfp,"@THIS\nD=M\n@%d\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",arg2);
    }
    if(strcmp(arg1, "that")==0){
        fprintf(this_object->asmfp,"@THAT\nD=M\n@%d\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",arg2);
    }
    if(strcmp(arg1, "static")==0){
        fprintf(this_object->asmfp,"@%s.%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",this_object->file_name,arg2);
    }
    if(strcmp(arg1, "pointer")==0){
        fprintf(this_object->asmfp,"@%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",CODEWRITER_POINTER_FIRST_NUMBER+arg2);
    }
    if(strcmp(arg1, "temp")==0){
        fprintf(this_object->asmfp,"@%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",CODEWRITER_TEMP_FIRST_NUMBER+arg2);
    }
}

void codewriter_writePop(CODEWRITER this_object, char arg1[], int arg2){
    if(strcmp(arg1, "local")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@LCL\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",arg2);
    }
    if(strcmp(arg1, "argument")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@ARG\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",arg2);
    }
    if(strcmp(arg1, "this")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@THIS\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",arg2);
    }
    if(strcmp(arg1, "that")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@THAT\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",arg2);
    }
    if(strcmp(arg1, "static")==0){
        fprintf(this_object->asmfp,"@%s.%d\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",this_object->file_name, arg2);
    }
    if(strcmp(arg1, "pointer")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",CODEWRITER_POINTER_FIRST_NUMBER+arg2);
    }
    if(strcmp(arg1, "temp")==0){
        fprintf(this_object->asmfp,"@%d\nD=A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n",CODEWRITER_TEMP_FIRST_NUMBER+arg2);
    }
}

void codewriter_writeArithmetic(CODEWRITER this_object, char command[]){
    if(strcmp(command, "add")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nM=D+M\n");
    }
    if(strcmp(command, "sub")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nM=M-D\n");
    }
    if(strcmp(command, "neg")==0){
        fprintf(this_object->asmfp,"@SP\nA=M-1\nM=-M\n");
    }
    if(strcmp(command,  "or")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nM=D|M\n");
    }
    if(strcmp(command, "and")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nM=D&M\n");
    }
    if(strcmp(command, "not")==0){
        fprintf(this_object->asmfp,"@SP\nA=M-1\nM=!M\n");
    }
    if(strcmp(command,  "eq")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nD=M-D\n");
        fprintf(this_object->asmfp,"@ARITHMETIC_EQ_%d_BEGIN\n",this_object->count_arithmetic_eq);
        fprintf(this_object->asmfp,"D;JEQ\n@SP\nA=M-1\nM=0\n");
        fprintf(this_object->asmfp,"@ARITHMETIC_EQ_%d_FINISH\n",this_object->count_arithmetic_eq);
        fprintf(this_object->asmfp,"0;JMP\n");
        fprintf(this_object->asmfp,"(ARITHMETIC_EQ_%d_BEGIN)\n",this_object->count_arithmetic_eq);
        fprintf(this_object->asmfp,"@SP\nA=M-1\nM=-1\n");
        fprintf(this_object->asmfp,"(ARITHMETIC_EQ_%d_FINISH)\n",this_object->count_arithmetic_eq);
        this_object->count_arithmetic_eq++;
    }
    if(strcmp(command,  "lt")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nD=M-D\n");
        fprintf(this_object->asmfp,"@ARITHMETIC_LT_%d_BEGIN\n",this_object->count_arithmetic_lt);
        fprintf(this_object->asmfp,"D;JLT\n@SP\nA=M-1\nM=0\n");
        fprintf(this_object->asmfp,"@ARITHMETIC_LT_%d_FINISH\n",this_object->count_arithmetic_lt);
        fprintf(this_object->asmfp,"0;JMP\n");
        fprintf(this_object->asmfp,"(ARITHMETIC_LT_%d_BEGIN)\n",this_object->count_arithmetic_lt);
        fprintf(this_object->asmfp,"@SP\nA=M-1\nM=-1\n");
        fprintf(this_object->asmfp,"(ARITHMETIC_LT_%d_FINISH)\n",this_object->count_arithmetic_lt);
        this_object->count_arithmetic_lt++;
    }
    if(strcmp(command,  "gt")==0){
        fprintf(this_object->asmfp,"@SP\nAM=M-1\nD=M\n@SP\nA=M-1\nD=M-D\n");
        fprintf(this_object->asmfp,"@ARITHMETIC_GT_%d_BEGIN\n",this_object->count_arithmetic_gt);
        fprintf(this_object->asmfp,"D;JGT\n@SP\nA=M-1\nM=0\n");
        fprintf(this_object->asmfp,"@ARITHMETIC_GT_%d_FINISH\n",this_object->count_arithmetic_gt);
        fprintf(this_object->asmfp,"0;JMP\n");
        fprintf(this_object->asmfp,"(ARITHMETIC_GT_%d_BEGIN)\n",this_object->count_arithmetic_gt);
        fprintf(this_object->asmfp,"@SP\nA=M-1\nM=-1\n");
        fprintf(this_object->asmfp,"(ARITHMETIC_GT_%d_FINISH)\n",this_object->count_arithmetic_gt);
        this_object->count_arithmetic_gt++;
    }
}