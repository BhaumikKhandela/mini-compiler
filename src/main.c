#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"
#include "semantic.h"
#include "tac.h"
#include "optimizer.h"
#include "codegen.h"

extern int yyparse(void);
extern FILE *yyin;
extern Program parsed_program;

static void dump_tokens(const char *src, const char *out_path){
    FILE*in=fopen(src,"r"); FILE*out=fopen(out_path,"w"); if(!in||!out){ if(in)fclose(in); if(out)fclose(out); return; }
    int c;
    while((c=fgetc(in))!=EOF){
        if(isspace(c)) continue;
        if(isalpha(c)||c=='_'){
            char buf[64]; int i=0; buf[i++]=(char)c;
            while((c=fgetc(in))!=EOF && (isalnum(c)||c=='_')) if(i<63) buf[i++]=(char)c;
            buf[i]='\0'; if(c!=EOF) ungetc(c,in);
            if(strcmp(buf,"int")==0||strcmp(buf,"if")==0||strcmp(buf,"else")==0||strcmp(buf,"while")==0) fprintf(out,"KEYWORD(%s)\n",buf);
            else fprintf(out,"IDENT(%s)\n",buf);
        } else if(isdigit(c)){
            char buf[64]; int i=0; buf[i++]=(char)c;
            while((c=fgetc(in))!=EOF && isdigit(c)) if(i<63) buf[i++]=(char)c;
            buf[i]='\0'; if(c!=EOF) ungetc(c,in); fprintf(out,"NUMBER(%s)\n",buf);
        } else {
            int n=fgetc(in);
            if((c=='='&&n=='=')||(c=='!'&&n=='=')) fprintf(out,"OP(%c%c)\n",c,n);
            else { if(n!=EOF) ungetc(n,in); fprintf(out,"SYM(%c)\n",c); }
        }
    }
    fclose(in); fclose(out);
}

int main(int argc, char **argv){
    if(argc < 3){ fprintf(stderr,"Usage: %s <input.toy> <output_dir>\n",argv[0]); return 1; }
    const char *input=argv[1], *od=argv[2];
    char p_tokens[256], p_ast[256], p_sem[256], p_tac[256], p_opt[256], p_asm[256];
    snprintf(p_tokens,sizeof(p_tokens),"%s/tokens.txt",od);
    snprintf(p_ast,sizeof(p_ast),"%s/ast.txt",od);
    snprintf(p_sem,sizeof(p_sem),"%s/semantic.txt",od);
    snprintf(p_tac,sizeof(p_tac),"%s/tac.txt",od);
    snprintf(p_opt,sizeof(p_opt),"%s/optimized_tac.txt",od);
    snprintf(p_asm,sizeof(p_asm),"%s/assembly.s",od);

    dump_tokens(input,p_tokens);
    yyin=fopen(input,"r"); if(!yyin){ perror("open input"); return 1; }
    if(yyparse()!=0){ fclose(yyin); fprintf(stderr,"Parsing failed\n"); return 1; }
    fclose(yyin);

    FILE*astf=fopen(p_ast,"w"); if(astf){ FILE*tmp=stdout; stdout=astf; print_ast(&parsed_program); stdout=tmp; fclose(astf);} 
    int sem_ok=semantic_check(&parsed_program,p_sem);
    TacProgram tac=generate_tac(&parsed_program);
    write_tac(&tac,p_tac);
    TacProgram opt=optimize_tac(&tac);
    write_tac(&opt,p_opt);
    generate_assembly(&opt,p_asm);

    free_program(&parsed_program);
    if(!sem_ok) return 2;
    return 0;
}
