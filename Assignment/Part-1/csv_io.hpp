#ifndef CSV_IO_HPP
#define CSV_IO_HPP

#include "ll_store.hpp"

// Loaders with validation & duplicate detection; print stats; return false on failure
bool loadResumesCSV(const char* path, ResNode*& head, int& n);
bool loadJobsCSV   (const char* path, JobNode*& head, int& n);

#endif // CSV_IO_HPP
