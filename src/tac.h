#ifndef TAC_H
#define TAC_H
#include "ast.h"

typedef struct { char op[8]; char arg1[32]; char arg2[32]; char res[32]; } Quad;
typedef struct { Quad items[1024]; int count; } TacProgram;

TacProgram generate_tac(const Program *program);
void write_tac(const TacProgram *tac, const char *path);

#endif
