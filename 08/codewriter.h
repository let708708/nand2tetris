#ifndef __CODEWRITER_H_INCLUDE__
#define __CODEWRITER_H_INCLUDE__

#define CODEWRITER_VMFILE_NAME_MAX_SIZE 255
#define CODEWRITER_TEMP_FIRST_NUMBER 5
#define CODEWRITER_POINTER_FIRST_NUMBER 3

typedef struct codewriter * CODEWRITER;

CODEWRITER codewriter_init(FILE* asmfp);
void codewriter_setFileName(CODEWRITER this_object, char VMfile_name[]);
void codewriter_writePush(CODEWRITER this_object, char arg1[], int arg2);
void codewriter_writePop(CODEWRITER this_object, char arg1[], int arg2);
void codewriter_writeArithmetic(CODEWRITER this_object, char command[]);
void codewriter_writeLabel(CODEWRITER this_object, char arg1[]);
void codewriter_writeGoto(CODEWRITER this_object, char arg1[]);
void codewriter_writeIf(CODEWRITER this_object, char arg1[]);
void codewriter_writeFunction(CODEWRITER this_object, char arg1[], int arg2);
void codewriter_writeReturn(CODEWRITER this_object);
void codewriter_writeInit(CODEWRITER this_object);
void codewriter_writeCall(CODEWRITER this_object, char arg1[], int arg2);

#endif