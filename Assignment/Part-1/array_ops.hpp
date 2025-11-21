#ifndef ARRAY_OPS_HPP
#define ARRAY_OPS_HPP

#include "records.hpp"

// Build arrays from linked lists
Job*    buildJobArray(struct JobNode* head, int n);
Resume* buildResArray(struct ResNode* head, int n);

// Sorting (arrays)
void qsortJob(Job* A,int l,int r);
void qsortRes(Resume* A,int l,int r);
void msortJobArr(Job* A, int l, int r);
void msortResArr(Resume* A, int l, int r);

// Binary search by ID numeric part
int binSearchJobById(Job* A, int n, const char* id);
int binSearchResById(Resume* A, int n, const char* id);

#endif // ARRAY_OPS_HPP
