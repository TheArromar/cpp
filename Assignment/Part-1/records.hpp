#ifndef RECORDS_HPP
#define RECORDS_HPP

#include "config.hpp"
#include <cstring>

struct Job{
    char id[16];
    char role[256];
    char skills[MAX_SKILLS][MAX_TOKEN_LEN]; int skillCount;
    char roleTokens[MAX_ROLE_TOKENS][MAX_TOKEN_LEN]; int roleTokCount;
    Job(){ std::memset(this,0,sizeof(Job)); }
};

struct Resume{
    char id[16];
    char skills[MAX_SKILLS][MAX_TOKEN_LEN]; int skillCount;
    Resume(){ std::memset(this,0,sizeof(Resume)); }
};

#endif // RECORDS_HPP
