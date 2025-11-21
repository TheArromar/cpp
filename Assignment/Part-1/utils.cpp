#include "utils.hpp"
#include <cctype>
#include <cstring>

using namespace std;

void ltrim(string &s){ size_t i=0; while(i<s.size()&&isspace((unsigned char)s[i])) ++i; if(i>0) s.erase(0,i); }
void rtrim(string &s){ if(s.empty()) return; size_t i=s.size(); while(i>0&&isspace((unsigned char)s[i-1])) --i; if(i<s.size()) s.erase(i); }
void trim(string &s){ ltrim(s); rtrim(s); }
string lower_copy(const string& s){ string t=s; for(char &c:t) c=(char)tolower((unsigned char)c); return t; }
bool is_alnum(char c){ return isalpha((unsigned char)c)||isdigit((unsigned char)c); }
int idNumericPart(const char* id){ int n=0; for(int i=0; id[i]; ++i) if(isdigit((unsigned char)id[i])) n = n*10 + (id[i]-'0'); return n; }

bool parse_csv2(const string& line, string& a, string& b){
    a.clear(); b.clear(); string cur; bool inQ=false; int col=0;
    for(char ch: line){
        if(ch=='"') inQ=!inQ;
        else if(ch==',' && !inQ){ if(col==0){ a=cur; cur.clear(); col=1; } else cur.push_back(ch); }
        else cur.push_back(ch);
    }
    if(col==0){ a=cur; b=""; } else b=cur;
    trim(a); trim(b);
    if(a.size()>=2 && a.front()=='"'&&a.back()=='"') a=a.substr(1,a.size()-2);
    if(b.size()>=2 && b.front()=='"'&&b.back()=='"') b=b.substr(1,b.size()-2);
    return !a.empty();
}
bool parse_csv3(const string& line, string& a, string& b, string& c){
    a.clear(); b.clear(); c.clear(); string cur; bool inQ=false; int col=0;
    for(char ch: line){
        if(ch=='"') inQ=!inQ;
        else if(ch==',' && !inQ){
            if(col==0){ a=cur; cur.clear(); col=1; }
            else if(col==1){ b=cur; cur.clear(); col=2; }
            else cur.push_back(ch);
        }else cur.push_back(ch);
    }
    if(col==0){ a=cur; } else if(col==1){ b=cur; } else c=cur;
    trim(a); trim(b); trim(c);
    if(a.size()>=2 && a.front()=='"'&&a.back()=='"') a=a.substr(1,a.size()-2);
    if(b.size()>=2 && b.front()=='"'&&b.back()=='"') b=b.substr(1,b.size()-2);
    if(c.size()>=2 && c.front()=='"'&&c.back()=='"') c=c.substr(1,c.size()-2);
    return !a.empty();
}

int splitSkills(const string& skillsCSV, char out[][MAX_TOKEN_LEN], int maxOut){
    int count=0; string cur; bool inQ=false;
    auto flush = [&](){
        string tok=cur; trim(tok);
        if(!tok.empty()){
            string low=lower_copy(tok);
            bool seen=false; for(int i=0;i<count;++i) if(::strncmp(out[i],low.c_str(),MAX_TOKEN_LEN)==0){ seen=true; break; }
            if(!seen && count<maxOut){ ::strncpy(out[count], low.c_str(), MAX_TOKEN_LEN-1); out[count][MAX_TOKEN_LEN-1]='\0'; ++count; }
        }
        cur.clear();
    };
    for(char ch: skillsCSV){
        if(ch=='"') inQ=!inQ;
        else if(ch==',' && !inQ) flush();
        else cur.push_back(ch);
    }
    flush();
    return count;
}
int tokensFromRole(const string& role, char out[][MAX_TOKEN_LEN], int maxOut){
    string low=lower_copy(role); string cur; int count=0;
    auto flush=[&](){
        string tok=cur; trim(tok);
        if(tok.size()>=2){
            bool seen=false; for(int i=0;i<count;++i) if(::strncmp(out[i],tok.c_str(),MAX_TOKEN_LEN)==0){ seen=true; break; }
            if(!seen && count<maxOut){ ::strncpy(out[count], tok.c_str(), MAX_TOKEN_LEN-1); out[count][MAX_TOKEN_LEN-1]='\0'; ++count; }
        }
        cur.clear();
    };
    for(char ch: low){ if(is_alnum(ch)) cur.push_back(ch); else flush(); } flush(); return count;
}
bool tokenExists(char arr[][MAX_TOKEN_LEN], int n, const char* t){
    for(int i=0;i<n;++i) if(::strncmp(arr[i],t,MAX_TOKEN_LEN)==0) return true;
    return false;
}
void buildQueryString(char query[][MAX_TOKEN_LEN], int qn, std::string& out){
    out.clear(); for(int i=0;i<qn;++i){ if(i) out.append(", "); out.append(query[i]); }
}
