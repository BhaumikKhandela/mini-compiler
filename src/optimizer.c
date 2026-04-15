#include "optimizer.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static int is_num(const char*s){ if(!*s) return 0; int i=0; if(s[0]=='-') i=1; for(;s[i];i++) if(!isdigit((unsigned char)s[i])) return 0; return 1; }

TacProgram optimize_tac(const TacProgram *in){
    TacProgram out={0};
    char knownVar[128][32], knownVal[128][32]; int kv=0;
    char exprL[128][32], exprOp[128][8], exprR[128][32], exprRes[128][32]; int ec=0;
    for(int i=0;i<in->count;i++){
        Quad q=in->items[i];
        for(int k=0;k<kv;k++){ if(strcmp(q.arg1,knownVar[k])==0) snprintf(q.arg1,32,"%s",knownVal[k]); if(strcmp(q.arg2,knownVar[k])==0) snprintf(q.arg2,32,"%s",knownVal[k]); }
        if(strcmp(q.op,"=")==0 && is_num(q.arg1)){
            int found=0; for(int k=0;k<kv;k++) if(strcmp(knownVar[k],q.res)==0){ snprintf(knownVal[k],32,"%s",q.arg1); found=1; }
            if(!found){ snprintf(knownVar[kv],32,"%s",q.res); snprintf(knownVal[kv],32,"%s",q.arg1); kv++; }
        }
        if(strcmp(q.op,"+")==0||strcmp(q.op,"-")==0||strcmp(q.op,"*")==0||strcmp(q.op,"/")==0){
            int reused=0;
            for(int e=0;e<ec;e++) if(strcmp(exprL[e],q.arg1)==0&&strcmp(exprOp[e],q.op)==0&&strcmp(exprR[e],q.arg2)==0){
                Quad assign={"=","","", ""}; snprintf(assign.arg1,32,"%s",exprRes[e]); snprintf(assign.res,32,"%s",q.res); out.items[out.count++]=assign; reused=1; break;
            }
            if(!reused){ snprintf(exprL[ec],32,"%s",q.arg1); snprintf(exprOp[ec],8,"%s",q.op); snprintf(exprR[ec],32,"%s",q.arg2); snprintf(exprRes[ec],32,"%s",q.res); ec++; out.items[out.count++]=q; }
        } else out.items[out.count++]=q;
    }
    return out;
}
