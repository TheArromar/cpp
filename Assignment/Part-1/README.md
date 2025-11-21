
# Academic Note #

This project was done by:
- Arromar A/L Irishigesan (Leader) - TP070288
- MOHAMMED ABDULBAQI MOHAMMED AHMED AQLAN - TP079915
- ABDULLAH MANAR MAATOQ ABDO - TP073641
- Dhiren Reef Sebastian - TP079271


This project is intended for the university module Data Structure (DSTR) only.

___

# Project Overview #

This program loads jobs and resumes from CSV, then allows you to:

1. Search and filter using skills typed by the user.
2. Sort/search using either Arrays or Linked Lists (your own implementations).
3. Additional benchmarks are shown when result are shown when finding matching jobs or resume by skill.
4. Match resumes ↔ jobs with a weighted rule-based score.
5. Export results to CSV (top-k recommendations, all matched jobs/resumes).
6. Run benchmarks to compare array vs linked list time and memory.

No STL containers are used for core data (no std::vector/std::list).
We use raw arrays and hand-rolled singly linked lists with manual memory management.
Comments are used throughout the project in each file to explain in-depth of the purpose of the code.

__________

# DSTR-Assignment-code #

driver.cpp          # CLI driver (end-to-end program)
utils.hpp / .cpp    # trimming, tokenizing, lowercasing, small helpers
records.hpp         # Job/Resume structs and constants
csv_io.hpp / .cpp   # robust CSV loader (2/3-column, quotes, errors, stats)
array_ops.hpp / .cpp# sorting (quick/merge) and array helpers
ll_store.hpp / .cpp # singly linked-list + merge sort + safe memory mgmt
match.hpp / .cpp    # scoring & top-K selection; scenario runner
exporter.hpp / .cpp # CSV exporters (matched_jobs/resumes, top-3 per resume)
benchmarks.hpp/.cpp # time & memory benchmarks (array vs list)
sample_jobs.csv     # tiny demo dataset
sample_resumes.csv  # tiny demo dataset
job_description.csv # (provided dataset - place here)
resume.csv          # (provided dataset - place here)
README.md           # this document

__________

# Build Instructions #


First, run both the python files for data cleaning.
```bash
python .\resume_cleaner.py
python .\jobDesc_cleaner.py
```

From project folder, in terminal, run:
```bash
g++ -std=gnu++17 -O2 driver.cpp utils.cpp ll_store.cpp array_ops.cpp csv_io.cpp match.cpp exporter.cpp benchmarks.cpp -o jobmatch
```

Then run using:
```bash
./jobmatch
```
__________

# Features #

## Foundations ##

1. C++ only; no STL containers for core data.
2. End-to-end driver with menu.
3. CSV parser for provided datasets.

##  Required ##

1. Array:
- Sorting: quick sort / merge sort on Job[], Resume[].
- Searching/Filtering: skill-based filtering (score > 0).

2. Linked List:
- Sorting: linked-list merge sort.
- Searching: linear traversal matching.
- Matching: rule-based weighted scoring (see below).

3. Error handling: malformed/missing/duplicate rows tracked and reported.

## Performance ##
1. Benchmarks comparing array vs linked list.
2. Memory estimation & node overhead explained.
3. Algorithms chosen to suit structure characteristics.


_______

# Data Structures & Algorithms #

## Arrays ##
- Storage: Job[], Resume[] (raw arrays).
- Sorting:
    - Quick sort (partition by numeric part of ID)
    - Merge sort (stable)
- Searching: linear scans across arrays (optional binary search on IDs if presorted, not required for skill match).
- **Why arrays?** Great cache locality and constant-time indexing. Quick/merge are optimal choices for contiguous memory.

## Linked Lists ##
- Storage: singly linked list (Node { T data; Node* next; }).
- Sorting: merge sort for linked lists (O(n log n) without random access).
- Searching: linear traversal.
- **Why lists?** Merge sort is natural for lists; insertions/deletions can be O(1) if needed. Trade-off: worse cache locality, extra pointer overhead per node.

________

# Error Handling & Validation #

Implemented in csv_io.* and used throughout the program:
- Missing files or bad headers have clear error messages.
- Empty files are reported or ignored.
- Malformed rows are skipped.
- Duplicate IDs are tracked and skipped.
- Quoted fields and commas are handled with a singler parser and stray quotes are rejected as malformed

______