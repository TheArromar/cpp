#include "benchmarks.hpp"
#include "csv_io.hpp"
#include "ll_store.hpp"
#include "array_ops.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

static void memReport(int JN, int RN){
    long long jobArr = (long long)JN * (long long)sizeof(Job);
    long long resArr = (long long)RN * (long long)sizeof(Resume);
    long long jobLL  = (long long)JN * (long long)sizeof(JobNode);
    long long resLL  = (long long)RN * (long long)sizeof(ResNode);

    auto KB = [](long long b){ return (double)b/1024.0; };

    std::cout << "\n=== Memory Estimate ===\n";
    std::cout << "Arrays:\n";
    std::cout << "  Jobs:    " << jobArr << " bytes (" << KB(jobArr) << " KB)\n";
    std::cout << "  Resumes: " << resArr << " bytes (" << KB(resArr) << " KB)\n";
    std::cout << "Linked Lists (node contains record + pointer):\n";
    std::cout << "  Jobs:    " << jobLL  << " bytes (" << KB(jobLL)  << " KB)\n";
    std::cout << "  Resumes: " << resLL  << " bytes (" << KB(resLL)  << " KB)\n";
    std::cout << "Note: allocator overhead and heap fragmentation are not included.\n";
}

void runBenchmarks(const char* resumeCSV, const char* jobCSV){
    JobNode* jHead=nullptr; ResNode* rHead=nullptr; int JN=0,RN=0;
    if(!loadJobsCSV(jobCSV,jHead,JN)) { std::cout<<"[ERR] jobs CSV\n"; return; }
    if(!loadResumesCSV(resumeCSV,rHead,RN)){ freeList(jHead); std::cout<<"[ERR] resume CSV\n"; return; }

    Job* J = buildJobArray(jHead, JN);
    const int TRIALS = 5;
    long long arrSortJobs_us=0, llSortJobs_us=0;
    long long arrSearchJobs_us=0, llSearchJobs_us=0;

    // SORT
    for(int t=0;t<TRIALS;++t){
        Job* tmp = nullptr;
        if(JN>0){ tmp = new Job[JN]; for(int i=0;i<JN;++i) tmp[i]=J[i]; }
        auto s1=Clock::now(); if(JN>0) qsortJob(tmp,0,JN-1); auto s2=Clock::now();
        arrSortJobs_us += std::chrono::duration_cast<std::chrono::microseconds>(s2-s1).count();
        if(tmp) delete[] tmp;

        JobNode* jC = cloneJobList(jHead);
        auto l1=Clock::now(); if(jC) mergeSortJobsLL(&jC); auto l2=Clock::now();
        llSortJobs_us += std::chrono::duration_cast<std::chrono::microseconds>(l2-l1).count();
        freeList(jC);
    }

    Job* Jsorted = nullptr; if(JN>0){ Jsorted=new Job[JN]; for(int i=0;i<JN;++i) Jsorted[i]=J[i]; qsortJob(Jsorted,0,JN-1); }
    JobNode* jSorted = cloneJobList(jHead); if(jSorted) mergeSortJobsLL(&jSorted);

    // SEARCH
    int lookups = (JN<1000? JN : 1000);
    for(int t=0;t<TRIALS;++t){
        int step = (JN>0 ? (JN / (lookups==0?1:lookups)) : 1); if(step<1) step=1;

        auto a1=Clock::now();
        for(int i=0, taken=0; i<JN && taken<lookups; i+=step,++taken){
            binSearchJobById(Jsorted, JN, Jsorted[i].id);
        }
        auto a2=Clock::now();
        arrSearchJobs_us += std::chrono::duration_cast<std::chrono::microseconds>(a2-a1).count();

        auto linFind = [](JobNode* h, const char* id)->JobNode*{
            for(JobNode* p=h;p;p=p->next) if(::strncmp(p->data.id,id,15)==0) return p;
            return nullptr;
        };
        auto l1=Clock::now();
        for(int i=0, taken=0; i<JN && taken<lookups; i+=step,++taken){
            linFind(jSorted, Jsorted[i].id);
        }
        auto l2=Clock::now();
        llSearchJobs_us += std::chrono::duration_cast<std::chrono::microseconds>(l2-l1).count();
    }

    std::cout << "\n=== Benchmarks (Averages over " << TRIALS << " trials) ===\n";
    std::cout << "[Sort on Jobs]   Array QuickSort: " << (arrSortJobs_us/TRIALS) << " us"
              << "   |   LinkedList MergeSort: " << (llSortJobs_us/TRIALS) << " us\n";
    std::cout << "[Search on Jobs] Array Binary:    " << (arrSearchJobs_us/TRIALS) << " us"
              << "   |   LinkedList Linear: " << (llSearchJobs_us/TRIALS) << " us\n";

    memReport(JN, RN);

    if(J) delete[] J;
    if(Jsorted) delete[] Jsorted;
    freeList(jSorted);
    freeList(jHead); freeList(rHead);
}
