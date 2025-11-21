#include <iostream>
#include <chrono>
#include "config.hpp"
#include "utils.hpp"
#include "ll_store.hpp"
#include "array_ops.hpp"
#include "csv_io.hpp"
#include "match.hpp"
#include "exporters.hpp"
#include "benchmarks.hpp"

using namespace std;

// CLI helpers
static int askDS(){ cout << "\nSelect a data structure:\n1. Array\n2. Linked List\n> "; int x; cin>>x; cin.ignore(10000,'\n'); return x; }
static int askSortAlgo(){ cout << "\nSelect a sorting algorithm:\n1. Merge Sort\n2. Quick Sort\n> "; int x; cin>>x; cin.ignore(10000,'\n'); return x; }
static bool askSortBefore(){ cout << "\nDo you want to sort before searching?\n1. Yes\n2. No\n> "; int x; cin>>x; cin.ignore(10000,'\n'); return x==1; }

static int readQuerySkills(char out[][MAX_TOKEN_LEN], int maxOut){
    cout << "\nEnter skills (type 'done' to finish):\n";
    string line; int n=0;
    while(true){
        if(!getline(cin,line)) break;
        trim(line);
        if(line.empty()) continue;
        if(lower_copy(line)=="done") break;
        string low=lower_copy(line);
        bool seen=false; for(int i=0;i<n;++i) if(::strncmp(out[i],low.c_str(),MAX_TOKEN_LEN)==0){ seen=true; break; }
        if(!seen && n<maxOut){ ::strncpy(out[n],low.c_str(),MAX_TOKEN_LEN-1); out[n][MAX_TOKEN_LEN-1]='\0'; ++n; }
    }
    return n;
}

static void printMenu(){
    cout << "\n=== Job Matcher ===\n";
    cout << "1) Find matching jobs by skill (also exports matched_jobs.csv)\n";
    cout << "2) Find matching resumes by skill (also exports matched_resumes.csv)\n";
    cout << "3) Show first 5 jobs\n";
    cout << "4) Show first 5 resumes\n";
    cout << "5) Export best three jobs for each Resume (matches_top3.csv)\n";
    cout << "6) Benchmarks (arrays vs linked lists)\n";
    cout << "7) Exit\n";
    cout << "> ";
}

int main(int argc, char** argv){
    const char* resumeCSV = (argc>=2? argv[1] : "cleaned_resume.csv");
    const char* jobCSV    = (argc>=3? argv[2] : "cleaned_job_description.csv");

    bool running=true;
    while(running){
        printMenu();
        int opt; if(!(cin>>opt)){ cin.clear(); cin.ignore(10000,'\n'); continue; }
        cin.ignore(10000,'\n');

        if(opt==7){ cout<<"Exiting program.\n"; break; }

        if(opt==3 || opt==4 || opt==5 || opt==6){
            if(opt==3 || opt==4){
                JobNode* jH=nullptr; ResNode* rH=nullptr; int JN=0,RN=0;
                if(!loadJobsCSV(jobCSV,jH,JN)){ cout<<"[ERR] jobs CSV\n"; continue; }
                if(!loadResumesCSV(resumeCSV,rH,RN)){ freeList(jH); cout<<"[ERR] resume CSV\n"; continue; }
                if(opt==3) showFirstJobs(jH,5); else showFirstResumes(rH,5);
                freeList(jH); freeList(rH);
                continue;
            }
            if(opt==5){
                JobNode* jH=nullptr; ResNode* rH=nullptr; int JN=0,RN=0;
                if(!loadJobsCSV(jobCSV,jH,JN)){ cout<<"[ERR] jobs CSV\n"; continue; }
                if(!loadResumesCSV(resumeCSV,rH,RN)){ freeList(jH); cout<<"[ERR] resume CSV\n"; continue; }
                auto t1=chrono::high_resolution_clock::now();
                int rows = exportAllMatchesTopK("matches_top3.csv", rH, RN, jH, JN, 3);
                auto t2=chrono::high_resolution_clock::now();
                if(rows>=0){
                    cout << "[OK] wrote matches_top3.csv with " << rows << " data rows (+1 header)\n";
                    cout << "Total matched jobs have been exported.\n";
                }else{
                    cout << "[ERR] failed to write matches_top3.csv\n";
                }
                cout << "Export Time: " << chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << " ms\n";
                freeList(jH); freeList(rH);
                continue;
            }
            if(opt==6){
                runBenchmarks(resumeCSV, jobCSV);
                continue;
            }
        }

        if(opt==1 || opt==2){
            int ds      = askDS();        // 1 Array, 2 Linked-List
            int sortAlg = askSortAlgo();  // 1 Merge, 2 Quick
            bool preSort= askSortBefore();

            cout << "\n[CONFIG] DS=" << (ds==1? "Array":"Linked List")
                << " | Sort=" << (sortAlg==1? "Merge":"Quick")
                << " | PreSort=" << (preSort? "Yes":"No") << "\n";

            char query[64][MAX_TOKEN_LEN];
            int qn = readQuerySkills(query, 64);
            if(qn==0){ cout << "No skills entered.\n"; continue; }

            auto perf = runScenario_printOnly(resumeCSV, jobCSV, opt, ds, sortAlg, preSort, query, qn);

            cout << "\nTotal matched " << perf.matchedLabel << ": " << perf.totalMatched << "\n";
            cout << "Fetch Time: " << (perf.fetch_us/1000) << " ms\n";
            cout << "Match Time: " << (perf.match_us/1000) << " ms\n";
            cout << "Total Program Execution Time: " << (perf.total_us/1000) << " ms\n";

            if(opt==1){
                int exported = exportMatchedJobsBySkills(jobCSV, ds, sortAlg, preSort, query, qn, "matched_jobs.csv");
                if(exported>=0){ cout << "Total matched jobs have been exported.\n"; }
                else{ cout << "[ERR] Could not export matched jobs.\n"; }
            }
            if(opt==2){
                int exportedR = exportMatchedResumesBySkills(resumeCSV, ds, sortAlg, preSort, query, qn, "matched_resumes.csv");
                if(exportedR >= 0){ cout << "Total matched resumes have been exported.\n"; }
                else{ cout << "[ERR] Could not export matched resumes.\n"; }
            }
            continue;
        }

        cout << "[ERR] Invalid option.\n";
    }
    return 0;
}
