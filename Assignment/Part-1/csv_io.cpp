#include "csv_io.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

// Tiny open-addressing string set for 16-char IDs (no STL)
struct IdSet {
    static const int SZ = 20011; // prime
    char  keys[SZ][16];
    bool  used[SZ];
    void reset(){ for(int i=0;i<SZ;++i){ used[i]=false; keys[i][0]='\0'; } }
    static unsigned hashId(const char* s){
        unsigned h=1469598101u;
        for(int i=0;s[i] && i<15;++i){ h ^= (unsigned char)s[i]; h *= 16777619u; }
        return h;
    }
    bool insertUnique(const char* s){
        unsigned h = hashId(s)%SZ;
        for(int k=0;k<SZ;++k){
            int i=(h+k)%SZ;
            if(!used[i]){ used[i]=true; ::strncpy(keys[i],s,15); keys[i][15]='\0'; return true; }
            if(::strncmp(keys[i],s,15)==0) return false; // duplicate
        }
        return false; // table full -> treat as duplicate
    }
};

static bool detectJobsHeader(const std::string& header, int& idxJob,int& idxRole,int& idxSkills){
    std::string a,b,c; if(!parse_csv3(header,a,b,c)) return false;
    auto L=[](std::string s){ for(char& c:s) c=(char)tolower((unsigned char)c); return s; };
    std::string la=L(a), lb=L(b), lc=L(c);
    idxJob=idxRole=idxSkills=-1;
    auto set=[&](const std::string& col,int pos){
        if(col.find("jobid")!=std::string::npos) idxJob=pos;
        if(col=="role" || col.find("role_clean")!=std::string::npos) idxRole=pos;
        if(col.find("cleaned_skills")!=std::string::npos) idxSkills=pos;
    };
    set(la,0); set(lb,1); set(lc,2);
    return (idxJob>=0 && idxRole>=0 && idxSkills>=0);
}

bool loadResumesCSV(const char* path, ResNode*& head, int& n){
    head=nullptr; n=0;
    std::ifstream fin(path);
    if(!fin.is_open()){ std::cerr<<"[ERR] Can't open resumes: "<<path<<"\n"; return false; }
    std::string header; if(!std::getline(fin,header)){ std::cerr<<"[ERR] Empty resumes file\n"; return false; }

    IdSet seen; seen.reset();
    int rows=0, malformed=0, dups=0, empties=0;

    std::string line;
    while(std::getline(fin,line)){
        if(line.size()==0){ ++empties; continue; }
        std::string id, skills; if(!parse_csv2(line,id,skills)){ ++malformed; continue; }
        if(id.empty()){ ++malformed; continue; }
        char idBuf[16]; ::strncpy(idBuf,id.c_str(),15); idBuf[15]='\0';
        if(!seen.insertUnique(idBuf)){ ++dups; continue; }

        Resume r; ::strncpy(r.id,idBuf,sizeof(r.id)-1); r.id[sizeof(r.id)-1]='\0';
        r.skillCount = splitSkills(skills,r.skills,MAX_SKILLS);
        pushResFront(head, r); ++rows;
    }
    n=rows;
    std::cout << "[OK] Loaded "<<rows<<" resumes  (empties="<<empties<<", malformed="<<malformed<<", duplicates="<<dups<<")\n";
    return rows>0;
}

bool loadJobsCSV(const char* path, JobNode*& head, int& n){
    head=nullptr; n=0;
    std::ifstream fin(path);
    if(!fin.is_open()){ std::cerr<<"[ERR] Can't open jobs: "<<path<<"\n"; return false; }
    std::string header; if(!std::getline(fin,header)){ std::cerr<<"[ERR] Empty jobs file\n"; return false; }

    int iJob,iRole,iSkills; if(!detectJobsHeader(header,iJob,iRole,iSkills)){
        std::cerr<<"[ERR] Header must contain JobID, Role/Role_Clean, Cleaned_Skills\n"; return false;
    }
    auto getCol=[&](const std::string& line,int idx,std::string& out){ std::string a,b,c; parse_csv3(line,a,b,c); out=(idx==0? a: idx==1? b: c); };

    IdSet seen; seen.reset();
    int rows=0, malformed=0, dups=0, empties=0;

    std::string line;
    while(std::getline(fin,line)){
        if(line.size()==0){ ++empties; continue; }
        std::string jid, role, skills; getCol(line,iJob,jid); getCol(line,iRole,role); getCol(line,iSkills,skills);
        if(jid.empty()){ ++malformed; continue; }
        char idBuf[16]; ::strncpy(idBuf,jid.c_str(),15); idBuf[15]='\0';
        if(!seen.insertUnique(idBuf)){ ++dups; continue; }

        Job j; ::strncpy(j.id,idBuf,sizeof(j.id)-1); j.id[sizeof(j.id)-1]='\0';
        ::strncpy(j.role,role.c_str(),sizeof(j.role)-1); j.role[sizeof(j.role)-1]='\0';
        j.skillCount=splitSkills(skills,j.skills,MAX_SKILLS);
        j.roleTokCount=tokensFromRole(role,j.roleTokens,MAX_ROLE_TOKENS);
        pushJobFront(head, j); ++rows;
    }
    n=rows;
    std::cout << "[OK] Loaded "<<rows<<" jobs     (empties="<<empties<<", malformed="<<malformed<<", duplicates="<<dups<<")\n";
    return rows>0;
}
