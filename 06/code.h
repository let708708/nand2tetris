#ifndef __CODE_H_INCLUDE__
#define __CODE_H_INCLUDE__

#define CODE_DEST_BIT_SIZE (3)
#define CODE_COMP_BIT_SIZE (7)
#define CODE_JUMP_BIT_SIZE (3) 

void code_dest(char mnemonic[], char dest[]);
void code_comp(char mnemonic[], char comp[]);
void code_jump(char mnemonic[], char comp[]);

#endif