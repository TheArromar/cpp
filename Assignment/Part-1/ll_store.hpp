#ifndef LL_STORE_HPP
#define LL_STORE_HPP

#include "records.hpp"

// Public nodes to avoid access issues
struct JobNode{ Job data; JobNode* next; JobNode(); };
struct ResNode{ Resume data; ResNode* next; ResNode(); };

// Basic ops
JobNode* pushJobFront(JobNode*& head, const Job& j);
ResNode* pushResFront(ResNode*& head, const Resume& r);
void freeList(JobNode*& h);
void freeList(ResNode*& h);

// Merge sort (by numeric ID)
void mergeSortJobsLL(JobNode** head);
void mergeSortResLL(ResNode** head);

// Cloning (used by benchmarks)
JobNode* cloneJobList(JobNode* h);
ResNode* cloneResList(ResNode* h);

// Display helpers
void showFirstJobs(JobNode* head, int n);
void showFirstResumes(ResNode* head, int n);

#endif // LL_STORE_HPP
