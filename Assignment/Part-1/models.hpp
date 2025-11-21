#ifndef MODELS_HPP
#define MODELS_HPP

#include <cstring>

// Limits
const int MAX_TOKEN_LEN   = 64;
const int MAX_SKILLS      = 256;
const int MAX_ROLE_TOKENS = 128;

// Data records (fixed-size, POD-friendly)
struct Job {
    char id[16];
    char role[256];
    char skills[MAX_SKILLS][MAX_TOKEN_LEN];   int skillCount;
    char roleTokens[MAX_ROLE_TOKENS][MAX_TOKEN_LEN]; int roleTokCount;
};

struct Resume {
    char id[16];
    char skills[MAX_SKILLS][MAX_TOKEN_LEN];   int skillCount;
};

#endif
