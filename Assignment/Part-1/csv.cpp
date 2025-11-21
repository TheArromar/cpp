#include "csv.hpp"
#include "utils.hpp"
#include <cstring>

// Robust CSV line splitter: returns up to 3 columns, handles quotes, commas, escaped quotes ("")
static int parse_csv_line3(const std::string& line, std::string cols[3]){
    int col=0; cols[0].clear(); cols[1].clear(); cols[2].clear();
    bool inQ=false;
    for(size_t i=0;i<line.size();++i){
        char ch=line[i];
        if(ch=='"'){
            if(inQ && i+1<line.size() && line[i+1]=='"'){ // escaped quote
                cols[col].push_back('"'); ++i;
            }else{
                inQ=!inQ;
            }
        }else if(ch==',' && !inQ){
            if(col<2) ++col;
        }else{
            cols[col].push_back(ch);
        }
    }
    for(int i=0;i<3;++i){ trim(cols[i]); }
    return col+1; // number of columns present
}

int splitSkills(const std::string& skillsCSV, char out[][MAX_TOKEN_LEN], int maxOut){
    int count=0; std::string cur; bool inQ=false;
    auto flush = [&](){
        std::string tok=cur; trim(tok);
        if(!tok.empty()){
            std::string low=lower_copy(tok);
            bool seen=false; for(int i=0;i<count;++i) if(std::strncmp(out[i],low.c_str(),MAX_TOKEN_LEN)==0){ seen=true; break; }
            if(!seen && count<maxOut){
                std::strncpy(out[count], low.c_str(), MAX_TOKEN_LEN-1);
                out[count][MAX_TOKEN_LEN-1]='\0'; ++count;
            }
        }
        cur.clear();
    };
    for(size_t i=0;i<skillsCSV.size();++i){
        char ch=skillsCSV[i];
        if(ch=='"'){
            if(inQ && i+1<skillsCSV.size() && skillsCSV[i+1]=='"'){ cur.push_back('"'); ++i; }
            else inQ=!inQ;
        }else if(ch==',' && !inQ) flush();
        else cur.push_back(ch);
    }
    flush();
    return count;
}

int tokensFromRole(const std::string& role, char out[][MAX_TOKEN_LEN], int maxOut){
    std::string low=lower_copy(role); std::string cur; int count=0;
    auto flush=[&](){
        std::string tok=cur; trim(tok);
        if(tok.size()>=2){
            bool seen=false; for(int i=0;i<count;++i) if(std::strncmp(out[i],tok.c_str(),MAX_TOKEN_LEN)==0){ seen=true; break; }
            if(!seen && count<maxOut){
                std::strncpy(out[count], tok.c_str(), MAX_TOKEN_LEN-1);
                out[count][MAX_TOKEN_LEN-1]='\0'; ++count;
            }
        }
        cur.clear();
    };
    for(char ch: low){ if(is_alnum(ch)) cur.push_back(ch); else flush(); }
    flush();
    return count;
}

CSVStatus loadResumesCSV(const char* path, Resume*& outArr, int& outN){
    outArr=nullptr; outN=0;
    std::ifstream fin(path);
    if(!fin.is_open()) return CSVStatus(false, "Cannot open resumes file: "+std::string(path));

    std::string header;
    if(!std::getline(fin, header)) return CSVStatus(false, "Empty resumes file");

    // First pass: count valid rows
    std::string line; int count=0, lineNo=1, bad=0;
    while(std::getline(fin,line)){
        ++lineNo; if(line.empty()) continue;
        std::string cols[3]; int n=parse_csv_line3(line, cols);
        if(n<1 || cols[0].empty()){ ++bad; continue; }
        ++count;
    }
    if(count==0) return CSVStatus(false, "No valid resume rows found", lineNo, 0);

    // allocate
    outArr = new Resume[count];
    // rewind
    fin.clear(); fin.seekg(0, std::ios::beg);
    std::getline(fin, header); // skip header

    // second pass: fill
    int idx=0; lineNo=1;
    while(std::getline(fin, line)){
        ++lineNo; if(line.empty()) continue;
        std::string cols[3]; int n=parse_csv_line3(line, cols);
        if(n<1 || cols[0].empty()) continue;
        Resume r{}; std::strncpy(r.id, cols[0].c_str(), sizeof(r.id)-1);
        r.id[sizeof(r.id)-1]='\0';
        r.skillCount = splitSkills((n>=2? cols[1]: std::string()), r.skills, MAX_SKILLS);
        outArr[idx++] = r;
    }
    outN = idx;
    return CSVStatus(true, "Loaded resumes", lineNo, outN);
}

CSVStatus loadJobsCSV(const char* path, Job*& outArr, int& outN){
    outArr=nullptr; outN=0;
    std::ifstream fin(path);
    if(!fin.is_open()) return CSVStatus(false, "Cannot open jobs file: "+std::string(path));

    std::string header;
    if(!std::getline(fin, header)) return CSVStatus(false, "Empty jobs file");

    // First pass: count valid rows
    std::string line; int count=0, lineNo=1, bad=0;
    while(std::getline(fin,line)){
        ++lineNo; if(line.empty()) continue;
        std::string c[3]; int n=parse_csv_line3(line, c);
        if(n<2 || c[0].empty()){ ++bad; continue; }
        ++count;
    }
    if(count==0) return CSVStatus(false, "No valid job rows found", lineNo, 0);

    outArr = new Job[count];

    // rewind
    fin.clear(); fin.seekg(0, std::ios::beg);
    std::getline(fin, header);

    int idx=0; lineNo=1;
    while(std::getline(fin,line)){
        ++lineNo; if(line.empty()) continue;
        std::string c[3]; int n=parse_csv_line3(line,c);
        if(n<2 || c[0].empty()) continue;
        Job j{}; 
        std::strncpy(j.id, c[0].c_str(), sizeof(j.id)-1); j.id[sizeof(j.id)-1]='\0';
        std::strncpy(j.role, (n>=2? c[1].c_str():""), sizeof(j.role)-1); j.role[sizeof(j.role)-1]='\0';
        j.skillCount = splitSkills((n>=3? c[2]: std::string()), j.skills, MAX_SKILLS);
        j.roleTokCount = tokensFromRole(j.role, j.roleTokens, MAX_ROLE_TOKENS);
        outArr[idx++] = j;
    }
    outN = idx;
    return CSVStatus(true, "Loaded jobs", lineNo, outN);
}
