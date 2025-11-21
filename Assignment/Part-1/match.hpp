#pragma once
#include <string>
#include "records.hpp"

// Perf struct
struct PerfOut {
    long long fetch_us = 0;
    long long match_us = 0;
    long long total_us = 0;
    int        totalMatched = 0;
    std::string matchedLabel;
};

// Scoring APIs (used by exporters)
int scoreResumeJob(const Resume& r, const Job& j);
int scoreSkillsVsJob(const char query[][64], int qn, const Job& j);
int scoreSkillsVsResume(const char query[][64], int qn, const Resume& r);

// Scenario
PerfOut runScenario_printOnly(const char* resumeCSV,
                              const char* jobCSV,
                              int option, int ds, int sortAlg, bool preSort,
                              char query[][64], int qn);
