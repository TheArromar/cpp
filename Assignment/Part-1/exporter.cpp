#include "exporters.hpp"
#include "csv_io.hpp"
#include "array_ops.hpp"
#include "utils.hpp"
#include <fstream>
#include "match.hpp"

int exportMatchedJobsBySkills(const char* jobCSV,
                            int ds,int sortAlg,bool preSort,
                            char query[][MAX_TOKEN_LEN],int qn,
                            const char* outPath)
{
    JobNode* jHead=nullptr; int JN=0;
    if(!loadJobsCSV(jobCSV,jHead,JN)) return -1;

    Job* J=nullptr;
    if(ds==1){
        J=buildJobArray(jHead,JN);
        if(preSort){
            if(sortAlg==1){ if(JN>0) msortJobArr(J,0,JN-1); }
            else          { if(JN>0) qsortJob(J,0,JN-1);   }
        }
    }else{
        if(preSort) mergeSortJobsLL(&jHead);
    }

    std::string qstr; buildQueryString(query, qn, qstr);

    std::ofstream f(outPath);
    if(!f.is_open()){ if(J) delete[] J; freeList(jHead); return -1; }
    f << "Rank,JobID,Score,Role,Query\n";

    int matched=0;
    if(ds==1){
        for(int j=0;j<JN;++j){
            int sc = scoreSkillsVsJob(query, qn, J[j]);
            if(sc>0){
                ++matched;
                f << matched << "," << J[j].id << "," << sc
                  << ",\"" << J[j].role << "\"," << "\"" << qstr << "\"\n";
            }
        }
    }else{
        for(JobNode* p=jHead;p;p=p->next){
            int sc=scoreSkillsVsJob(query, qn, p->data);
            if(sc>0){
                ++matched;
                f << matched << "," << p->data.id << "," << sc
                  << ",\"" << p->data.role << "\"," << "\"" << qstr << "\"\n";
            }
        }
    }

    f.close();
    if(J) delete[] J; freeList(jHead);
    return matched;
}

int exportMatchedResumesBySkills(const char* resumeCSV,
                                 int ds, int sortAlg, bool preSort,
                                 char query[][MAX_TOKEN_LEN], int qn,
                                 const char* outPath)
{
    ResNode* rHead = nullptr; int RN = 0;
    if(!loadResumesCSV(resumeCSV, rHead, RN)) return -1;

    Resume* R = nullptr;
    if(ds==1){
        R = buildResArray(rHead, RN);
        if(preSort){
            if(sortAlg==1){ if(RN>0) msortResArr(R, 0, RN-1); }
            else          { if(RN>0) qsortRes(R, 0, RN-1);   }
        }
    }else{
        if(preSort) mergeSortResLL(&rHead);
    }

    std::string qstr; buildQueryString(query, qn, qstr);

    std::ofstream f(outPath);
    if(!f.is_open()){ if(R) delete[] R; freeList(rHead); return -1; }
    f << "Rank,ResumeID,Score,Query\n";

    int matched = 0;
    if(ds==1){
        for(int i=0; i<RN; ++i){
            int sc = scoreSkillsVsResume(query, qn, R[i]);
            if(sc > 0){
                ++matched;
                f << matched << "," << R[i].id << "," << sc
                  << ",\"" << qstr << "\"\n";
            }
        }
    }else{
        for(ResNode* p=rHead; p; p=p->next){
            int sc = scoreSkillsVsResume(query, qn, p->data);
            if(sc > 0){
                ++matched;
                f << matched << "," << p->data.id << "," << sc
                  << ",\"" << qstr << "\"\n";
            }
        }
    }

    f.close();
    if(R) delete[] R;
    freeList(rHead);
    return matched;
}

int exportAllMatchesTopK(const char* outPath,
                         const ResNode* rHead, int RN,
                         const JobNode* jHead, int JN,
                         int K)
{
    Job* J=nullptr; Resume* R=nullptr;
    if(JN>0){ J=new Job[JN]; int i=0; for(const JobNode* p=jHead;p;p=p->next) J[i++]=p->data; qsortJob(J,0,JN-1); }
    if(RN>0){ R=new Resume[RN]; int i=0; for(const ResNode* p=rHead;p;p=p->next) R[i++]=p->data; qsortRes(R,0,RN-1); }

    std::ofstream f(outPath);
    if(!f.is_open()){ if(J) delete[] J; if(R) delete[] R; return -1; }
    f << "ResumeID,Rank,JobID,Score,Role\n";
    int written=0;

    for(int r=0;r<RN;++r){
        struct Top { const Job* job; int score; int jobNum; } top[10];
        for(int i=0;i<K;++i) top[i] = {nullptr,-1,0};
        for(int j=0;j<JN;++j){
            int sc = scoreResumeJob(R[r], J[j]);
            int jn = idNumericPart(J[j].id);
            int pos=0; for(int i=1;i<K;++i) if(top[i].score<top[pos].score) pos=i;
            if(sc>top[pos].score || (sc==top[pos].score && jn<top[pos].jobNum)){
                top[pos] = { &J[j], sc, jn };
            }
        }
        for(int a=0;a<K-1;++a) for(int b=0;b<K-1-a;++b)
            if( top[b].score < top[b+1].score ||
                (top[b].score==top[b+1].score && top[b].jobNum>top[b+1].jobNum) ){
                auto t=top[b]; top[b]=top[b+1]; top[b+1]=t;
            }
        for(int i=0;i<K;++i){
            if(top[i].job && top[i].score>0){
                f << R[r].id << "," << (i+1) << ","
                  << top[i].job->id << "," << top[i].score << ","
                  << '"' << top[i].job->role << '"' << "\n";
                ++written;
            }
        }
    }

    f.close();
    if(J) delete[] J; if(R) delete[] R;
    return written;
}
