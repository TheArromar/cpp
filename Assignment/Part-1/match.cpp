#include "match.hpp"
#include "csv_io.hpp"
#include "ll_store.hpp"
#include "array_ops.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

struct TopItemJob{ const Job* job; int score; int jobNum; };
struct TopItemRes{ const Resume* res; int score; int resNum; };

static void updateTop(TopItemJob* top,int K,const Job* j,int sc){
    int jn=idNumericPart(j->id);
    int pos=0; for(int i=1;i<K;++i) if(top[i].score<top[pos].score) pos=i;
    if(sc>top[pos].score || (sc==top[pos].score && jn<top[pos].jobNum)){ top[pos].job=j; top[pos].score=sc; top[pos].jobNum=jn; }
}
static void sortTop(TopItemJob* top,int K){
    for(int a=0;a<K-1;++a) for(int b=0;b<K-1-a;++b)
        if(top[b].score<top[b+1].score || (top[b].score==top[b+1].score && top[b].jobNum>top[b+1].jobNum)){
            TopItemJob t=top[b]; top[b]=top[b+1]; top[b+1]=t;
        }
}
static void updateTop(TopItemRes* top,int K,const Resume* r,int sc){
    int rn=idNumericPart(r->id);
    int pos=0; for(int i=1;i<K;++i) if(top[i].score<top[pos].score) pos=i;
    if(sc>top[pos].score || (sc==top[pos].score && rn<top[pos].resNum)){ top[pos].res=r; top[pos].score=sc; top[pos].resNum=rn; }
}
static void sortTop(TopItemRes* top,int K){
    for(int a=0;a<K-1;++a) for(int b=0;b<K-1-a;++b)
        if(top[b].score<top[b+1].score || (top[b].score==top[b+1].score && top[b].resNum>top[b+1].resNum)){
            TopItemRes t=top[b]; top[b]=top[b+1]; top[b+1]=t;
        }
}

int scoreResumeJob(const Resume& r, const Job& j){
    int sc=0; for(int i=0;i<r.skillCount;++i) if(tokenExists((char(*)[MAX_TOKEN_LEN])j.skills,j.skillCount,r.skills[i])) ++sc;
    for(int i=0;i<r.skillCount;++i) if(tokenExists((char(*)[MAX_TOKEN_LEN])j.roleTokens,j.roleTokCount,r.skills[i])) sc+=2;
    return sc;
}
int scoreSkillsVsJob(const char query[][MAX_TOKEN_LEN], int qn, const Job& j){
    int sc=0; for(int i=0;i<qn;++i) if(tokenExists((char(*)[MAX_TOKEN_LEN])j.skills,j.skillCount,query[i])) ++sc;
    for(int i=0;i<qn;++i) if(tokenExists((char(*)[MAX_TOKEN_LEN])j.roleTokens,j.roleTokCount,query[i])) sc+=2;
    return sc;
}
int scoreSkillsVsResume(const char query[][MAX_TOKEN_LEN], int qn, const Resume& r){
    int sc=0; for(int i=0;i<qn;++i) if(tokenExists((char(*)[MAX_TOKEN_LEN])r.skills,r.skillCount,query[i])) ++sc;
    return sc;
}

PerfOut runScenario_printOnly(const char* resumeCSV, const char* jobCSV,
                            int option,int ds,int sortAlg,bool preSort,
                            char query[][MAX_TOKEN_LEN], int qn)
{
    PerfOut out;
    auto T0=Clock::now();

    // Fetch
    auto F1=Clock::now();
    JobNode* jHead=nullptr; ResNode* rHead=nullptr; int JN=0,RN=0;
    if(!loadJobsCSV(jobCSV,jHead,JN)) return out;
    if(!loadResumesCSV(resumeCSV,rHead,RN)) { freeList(jHead); return out; }
    auto F2=Clock::now(); out.fetch_us=std::chrono::duration_cast<std::chrono::microseconds>(F2-F1).count();

    Job* J=nullptr; Resume* R=nullptr;
    if(ds==1){
        if(JN>0){ J=buildJobArray(jHead,JN); }
        if(RN>0){ R=buildResArray(rHead,RN); }
        if(preSort){
            if(sortAlg==1){ if(JN>0) msortJobArr(J,0,JN-1); if(RN>0) msortResArr(R,0,RN-1); }
            else           { if(JN>0) qsortJob(J,0,JN-1);   if(RN>0) qsortRes(R,0,RN-1);   }
        }
    }else{
        if(preSort){ if(jHead) mergeSortJobsLL(&jHead); if(rHead) mergeSortResLL(&rHead); }
    }

    // Match/Print
    std::string qstr; buildQueryString(query, qn, qstr);

    auto M1=Clock::now();
    if(option==1){
        TopItemJob top[TOPK_DEFAULT]; for(int i=0;i<TOPK_DEFAULT;++i) top[i]={nullptr,-1,0};
        int withScore=0;
        if(ds==1){
            for(int j=0;j<JN;++j){ int sc=scoreSkillsVsJob(query,qn,J[j]); if(sc>0) ++withScore; updateTop(top,TOPK_DEFAULT,&J[j],sc); }
        }else{
            for(JobNode* p=jHead;p;p=p->next){ int sc=scoreSkillsVsJob(query,qn,p->data); if(sc>0) ++withScore; updateTop(top,TOPK_DEFAULT,&(p->data),sc); }
        }
        sortTop(top,TOPK_DEFAULT); out.totalMatched=withScore; out.matchedLabel="jobs";
        std::cout << "\nTop " << TOPK_DEFAULT << " matching jobs:\n";
        std::cout << "Query: " << qstr << "\n";
        int shown=0; for(int i=0;i<TOPK_DEFAULT;++i) if(top[i].job && top[i].score>0){
            std::cout<<"  #"<<(i+1)<<"  "<<top[i].job->id
                <<"  | Score "<<top[i].score
                <<"  | Role: "<<top[i].job->role
                <<"  | Query: " << qstr << "\n"; ++shown; }
        if(!shown) std::cout<<"  (No strong matches)\n";
    }else{ // option==2
        TopItemRes top[TOPK_DEFAULT]; for(int i=0;i<TOPK_DEFAULT;++i) top[i]={nullptr,-1,0};
        int withScore=0;
        if(ds==1){
            for(int r=0;r<RN;++r){ int sc=scoreSkillsVsResume(query,qn,R[r]); if(sc>0) ++withScore; updateTop(top,TOPK_DEFAULT,&R[r],sc); }
        }else{
            for(ResNode* p=rHead;p;p=p->next){ int sc=scoreSkillsVsResume(query,qn,p->data); if(sc>0) ++withScore; updateTop(top,TOPK_DEFAULT,&(p->data),sc); }
        }
        sortTop(top,TOPK_DEFAULT); out.totalMatched=withScore; out.matchedLabel="resumes";
        std::cout << "\nTop " << TOPK_DEFAULT << " matching resumes:\n";
        std::cout << "Query: " << qstr << "\n";
        int shown=0; for(int i=0;i<TOPK_DEFAULT;++i) if(top[i].res && top[i].score>0){
            std::cout<<"  #"<<(i+1)<<"  "<<top[i].res->id
                <<"  | Score "<<top[i].score
                <<"  | Query: " << qstr << "\n"; ++shown; }
        if(!shown) std::cout<<"  (No strong matches)\n";
    }
    auto M2=Clock::now(); out.match_us=std::chrono::duration_cast<std::chrono::microseconds>(M2-M1).count();
    auto T1=Clock::now(); out.total_us=std::chrono::duration_cast<std::chrono::microseconds>(T1-T0).count();

    if(J) delete[] J; if(R) delete[] R; freeList(jHead); freeList(rHead);
    return out;
}
