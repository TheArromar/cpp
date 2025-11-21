#ifndef CSV_HPP
#define CSV_HPP

#include <string>
#include <fstream>
#include "models.hpp"

// Tiny CSV reader (2- or 3-column files) with quote + escaped-quote handling.
struct CSVStatus {
    bool ok;
    std::string message;
    int linesRead;
    int rowsLoaded;
    CSVStatus(bool o=false, std::string m="", int lr=0, int rl=0): ok(o), message(m), linesRead(lr), rowsLoaded(rl) {}
};

int splitSkills(const std::string& skillsCSV, char out[][MAX_TOKEN_LEN], int maxOut);
int tokensFromRole(const std::string& role, char out[][MAX_TOKEN_LEN], int maxOut);

// Expect:
//  Resume CSV: "ResumeID,Cleaned_Skills"
//  Job CSV:    "JobID,Role,Cleaned_Skills"  (or "Role_Clean")
CSVStatus loadResumesCSV(const char* path, Resume*& outArr, int& outN);
CSVStatus loadJobsCSV(const char* path, Job*& outArr, int& outN);

#endif
