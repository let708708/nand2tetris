#include <stdio.h>
#include <string.h>
#include "codewriter.h"


struct codewriter{
    FILE *asmfp;
    char file_name[CODEWRITER_VMFILE_NAME_MAX_SIZE+1];
    int count_arithmetic_eq;
    int count_arithmetic_lt;
    int count_arithmetic_gt;
    int count_call_function;
};

CODEWRITER codewriter_init(FILE* asmfp){
    static struct codewriter this_object_inner;
    this_object_inner.asmfp = asmfp;
    strcpy(this_object_inner.file_name,"");
    this_object_inner.count_arithmetic_eq = 0;
    this_object_inner.count_arithmetic_lt = 0;
    this_object_inner.count_arithmetic_gt = 0;
    this_object_inner.count_call_function = 0;
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

void codewriter_writeLabel(CODEWRITER this_object, char arg1[]){
    fprintf(this_object->asmfp, "(%s$%s)\n", this_object->file_name, arg1);
}

void codewriter_writeGoto(CODEWRITER this_object, char arg1[]){
    fprintf(this_object->asmfp, "@%s$%s\n", this_object->file_name, arg1);
    fprintf(this_object->asmfp, "0;JMP\n");
}

void codewriter_writeIf(CODEWRITER this_object, char arg1[]){
    fprintf(this_object->asmfp, "@SP\nAM=M-1\nD=M\n");
    fprintf(this_object->asmfp, "@%s$%s\n", this_object->file_name, arg1);
    fprintf(this_object->asmfp, "D;JNE\n");
}

void codewriter_writeFunction(CODEWRITER this_object, char arg1[], int arg2){
    fprintf(this_object->asmfp, "(%s)\n", arg1);
    for (int i=0; i<arg2; i++){
        fprintf(this_object->asmfp,"@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    }
}

void codewriter_writeReturn(CODEWRITER this_object){
    fprintf(this_object->asmfp, "@LCL\nD=M\n@R13\nM=D\n");            // FRAME = LCL
    fprintf(this_object->asmfp, "@5\nA=D-A\nD=M\n@R14\nM=D\n");       // RET = *(FRAME - 5)
    fprintf(this_object->asmfp, "@SP\nA=M-1\nD=M\n@ARG\nA=M\nM=D\n"); // *ARG = *(SP-1)
    fprintf(this_object->asmfp, "@ARG\nD=M+1\n@SP\nM=D\n");           // SP = ARG+1
    fprintf(this_object->asmfp, "@R13\nAM=M-1\nD=M\n@THAT\nM=D\n");   // THAT =*(--FRAME)
    fprintf(this_object->asmfp, "@R13\nAM=M-1\nD=M\n@THIS\nM=D\n");   // THIS =*(--FRAME)
    fprintf(this_object->asmfp, "@R13\nAM=M-1\nD=M\n@ARG\nM=D\n");    // ARG =*(--FRAME)
    fprintf(this_object->asmfp, "@R13\nAM=M-1\nD=M\n@LCL\nM=D\n");    // LCL =*(--FRAME)
    fprintf(this_object->asmfp, "@R14\nA=M\n0;JMP\n");                // goto RET
}

void codewriter_writeCall(CODEWRITER this_object, char arg1[], int arg2){
    fprintf(this_object->asmfp, "@CALL_FUNCTION_%d_RETURN_ADDRESS\n", this_object->count_call_function);
    fprintf(this_object->asmfp, "D=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    fprintf(this_object->asmfp, "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    fprintf(this_object->asmfp, "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    fprintf(this_object->asmfp, "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    fprintf(this_object->asmfp, "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    fprintf(this_object->asmfp, "@SP\nD=M\n@%d\nD=D-A\n@ARG\nM=D\n",arg2+5);
    fprintf(this_object->asmfp, "@SP\nD=M\n@LCL\nM=D\n");
    fprintf(this_object->asmfp, "@%s\n0;JMP\n",arg1);
    fprintf(this_object->asmfp, "(CALL_FUNCTION_%d_RETURN_ADDRESS)\n", this_object->count_call_function);
    this_object->count_call_function++;
}

void codewriter_writeInit(CODEWRITER this_object){
    fprintf(this_object->asmfp, "@256\nD=A\n@SP\nM=D\n");
    codewriter_writeCall(this_object,"Sys.init", 0);
}