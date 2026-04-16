#include "tac.h"
#include <stdio.h>
#include <string.h>

static TacProgram prog;
static int tmp_id,label_id;
static char bufs[4096][32];
static int bidx;

static char* save(const char*s){ snprintf(bufs[bidx],32,"%s",s); return bufs[bidx++]; }
static char* new_tmp(void){ char t[32]; snprintf(t,32,"t%d",++tmp_id); return save(t); }
static char* new_label(void){ char t[32]; snprintf(t,32,"L%d",++label_id); return save(t); }
static void emit(const char*op,const char*a1,const char*a2,const char*r){ Quad*q=&prog.items[prog.count++]; snprintf(q->op,8,"%s",op); snprintf(q->arg1,32,"%s",a1?a1:""); snprintf(q->arg2,32,"%s",a2?a2:""); snprintf(q->res,32,"%s",r?r:""); }

static char* gen_expr(const Expr*e){
    if(e->type==EXPR_INT){ char t[32]; snprintf(t,32,"%d",e->data.int_val); return save(t);} 
    if(e->type==EXPR_VAR) return save(e->data.var_name);
    char*l=gen_expr(e->data.bin.left), *r=gen_expr(e->data.bin.right), *t=new_tmp();
    const char*op="+";
    switch(e->data.bin.op){case OP_ADD:op="+";break;case OP_SUB:op="-";break;case OP_MUL:op="*";break;case OP_DIV:op="/";break;case OP_LT:op="<";break;case OP_GT:op=">";break;case OP_EQ:op="==";break;case OP_NEQ:op="!=";break;}
    emit(op,l,r,t); return t;
}

static void gen_stmt(const Stmt*s){
    for(;s;s=s->next){
        if(s->type==STMT_ASSIGN){ char*rhs=gen_expr(s->data.assign.expr); emit("=",rhs,NULL,s->data.assign.var); }
        else if(s->type==STMT_IF){
            char*c=gen_expr(s->data.if_stmt.cond); char*l_else=new_label(); char*l_end=new_label();
            emit("IFZ",c,NULL,l_else); gen_stmt(s->data.if_stmt.then_branch); emit("GOTO",NULL,NULL,l_end);
            emit("LABEL",NULL,NULL,l_else); gen_stmt(s->data.if_stmt.else_branch); emit("LABEL",NULL,NULL,l_end);
        } else if(s->type==STMT_WHILE){
            char*l_start=new_label(); char*l_end=new_label(); emit("LABEL",NULL,NULL,l_start);
            char*c=gen_expr(s->data.while_stmt.cond); emit("IFZ",c,NULL,l_end); gen_stmt(s->data.while_stmt.body);
            emit("GOTO",NULL,NULL,l_start); emit("LABEL",NULL,NULL,l_end);
        } else if(s->type==STMT_BLOCK){
            gen_stmt(s->data.block.stmts);
        } else if(s->type==STMT_DECL){
            /* Declarations do not directly emit TAC in this compiler. */
        }
    }
}

TacProgram generate_tac(const Program *p){ memset(&prog,0,sizeof(prog)); tmp_id=0; label_id=0; bidx=0; gen_stmt(p->stmts); return prog; }
void write_tac(const TacProgram *t, const char *path){ FILE*f=fopen(path,"w"); for(int i=0;i<t->count;i++){ const Quad*q=&t->items[i];
    if(strcmp(q->op,"=")==0) fprintf(f,"%s = %s\n",q->res,q->arg1);
    else if(strcmp(q->op,"IFZ")==0) fprintf(f,"IFZ %s GOTO %s\n",q->arg1,q->res);
    else if(strcmp(q->op,"GOTO")==0) fprintf(f,"GOTO %s\n",q->res);
    else if(strcmp(q->op,"LABEL")==0) fprintf(f,"%s:\n",q->res);
    else fprintf(f,"%s = %s %s %s\n",q->res,q->arg1,q->op,q->arg2);
} fclose(f);} 
