#ifndef EXPORTERS_HPP
#define EXPORTERS_HPP
#pragma once

#include "records.hpp"
#include "ll_store.hpp"
#include "records.hpp"

int exportMatchedJobsBySkills(const char* jobCSV,
                            int ds,int sortAlg,bool preSort,
                            char query[][MAX_TOKEN_LEN],int qn,
                            const char* outPath);

int exportMatchedResumesBySkills(const char* resumeCSV,
                                 int ds, int sortAlg, bool preSort,
                                 char query[][MAX_TOKEN_LEN], int qn,
                                 const char* outPath);

int exportAllMatchesTopK(const char* outPath,
                         const ResNode* rHead, int RN,
                         const JobNode* jHead, int JN,
                         int K);

#endif // EXPORTERS_HPP
