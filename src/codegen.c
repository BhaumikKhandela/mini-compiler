#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int is_num(const char*s){ if(!*s) return 0; int i=0; if(s[0]=='-') i=1; for(;s[i];i++) if(!isdigit((unsigned char)s[i])) return 0; return 1; }

void generate_assembly(const TacProgram *tac, const char *path){
    FILE*f=fopen(path,"w"); if(!f) return;
    fprintf(f,"; MIPS-like pseudo assembly\n");
    for(int i=0;i<tac->count;i++){
        Quad q=tac->items[i];
        if(strcmp(q.op,"LABEL")==0) fprintf(f,"%s:\n", q.res);
        else if(strcmp(q.op,"GOTO")==0) fprintf(f,"J %s\n", q.res);
        else if(strcmp(q.op,"IFZ")==0){
            if(is_num(q.arg1)) fprintf(f,"LI R1, %s\n", q.arg1); else fprintf(f,"LOAD R1, %s\n", q.arg1);
            fprintf(f,"BEQ R1, R0, %s\n", q.res);
        } else if(strcmp(q.op,"=")==0){
            if(is_num(q.arg1)) fprintf(f,"LI R1, %s\n", q.arg1); else fprintf(f,"LOAD R1, %s\n", q.arg1);
            fprintf(f,"STORE R1, %s\n", q.res);
        } else {
            if(is_num(q.arg1)) fprintf(f,"LI R1, %s\n", q.arg1); else fprintf(f,"LOAD R1, %s\n", q.arg1);
            if(is_num(q.arg2)) fprintf(f,"LI R2, %s\n", q.arg2); else fprintf(f,"LOAD R2, %s\n", q.arg2);
            if(strcmp(q.op,"+")==0) fprintf(f,"ADD R3, R1, R2\n");
            else if(strcmp(q.op,"-")==0) fprintf(f,"SUB R3, R1, R2\n");
            else if(strcmp(q.op,"*")==0) fprintf(f,"MUL R3, R1, R2\n");
            else if(strcmp(q.op,"/")==0) fprintf(f,"DIV R3, R1, R2\n");
            else if(strcmp(q.op,"<")==0) fprintf(f,"SLT R3, R1, R2\n");
            else if(strcmp(q.op,">")==0) fprintf(f,"SLT R3, R2, R1\n");
            else if(strcmp(q.op,"==")==0) fprintf(f,"SEQ R3, R1, R2\n");
            else if(strcmp(q.op,"!=")==0) fprintf(f,"SNE R3, R1, R2\n");
            fprintf(f,"STORE R3, %s\n", q.res);
        }
    }
    fclose(f);
}
