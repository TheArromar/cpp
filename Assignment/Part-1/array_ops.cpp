#include "array_ops.hpp"
#include "ll_store.hpp"
#include "utils.hpp"
#include <cstring>

static int cmpJob(const Job& A, const Job& B){ int a=idNumericPart(A.id), b=idNumericPart(B.id); return (a<b?-1:(a>b?1:0)); }
static int cmpRes(const Resume& A, const Resume& B){ int a=idNumericPart(A.id), b=idNumericPart(B.id); return (a<b?-1:(a>b?1:0)); }

Job* buildJobArray(JobNode* head, int n){ if(n<=0) return nullptr; Job* A=new Job[n]; int i=0; for(JobNode* p=head;p;p=p->next) A[i++]=p->data; return A; }
Resume* buildResArray(ResNode* head, int n){ if(n<=0) return nullptr; Resume* A=new Resume[n]; int i=0; for(ResNode* p=head;p;p=p->next) A[i++]=p->data; return A; }

static void swapJob(Job& a, Job& b){ Job t=a; a=b; b=t; }
static void swapRes(Resume& a, Resume& b){ Resume t=a; a=b; b=t; }
static int partJob(Job* A,int l,int r){ int p=idNumericPart(A[r].id); int i=l; for(int j=l;j<r;++j){ if(idNumericPart(A[j].id)<=p){ swapJob(A[i],A[j]); ++i; } } swapJob(A[i],A[r]); return i; }
void qsortJob(Job* A,int l,int r){ if(l<r){ int pi=partJob(A,l,r); if(pi>l) qsortJob(A,l,pi-1); if(pi<r) qsortJob(A,pi+1,r); } }
static int partRes(Resume* A,int l,int r){ int p=idNumericPart(A[r].id); int i=l; for(int j=l;j<r;++j){ if(idNumericPart(A[j].id)<=p){ swapRes(A[i],A[j]); ++i; } } swapRes(A[i],A[r]); return i; }
void qsortRes(Resume* A,int l,int r){ if(l<r){ int pi=partRes(A,l,r); if(pi>l) qsortRes(A,l,pi-1); if(pi<r) qsortRes(A,pi+1,r); } }

static void mergeJobArr(Job* A, int l, int m, int r){
    int n1=m-l+1, n2=r-m; Job* L=new Job[n1]; Job* R=new Job[n2];
    for(int i=0;i<n1;++i) L[i]=A[l+i]; for(int j=0;j<n2;++j) R[j]=A[m+1+j];
    int i=0,j=0,k=l; while(i<n1 && j<n2){ if(cmpJob(L[i],R[j])<=0) A[k++]=L[i++]; else A[k++]=R[j++]; }
    while(i<n1) A[k++]=L[i++]; while(j<n2) A[k++]=R[j++]; delete[] L; delete[] R;
}
void msortJobArr(Job* A, int l, int r){ if(l>=r) return; int m=l+(r-l)/2; msortJobArr(A,l,m); msortJobArr(A,m+1,r); mergeJobArr(A,l,m,r); }

static void mergeResArr(Resume* A, int l, int m, int r){
    int n1=m-l+1, n2=r-m; Resume* L=new Resume[n1]; Resume* R=new Resume[n2];
    for(int i=0;i<n1;++i) L[i]=A[l+i]; for(int j=0;j<n2;++j) R[j]=A[m+1+j];
    int i=0,j=0,k=l; while(i<n1 && j<n2){ if(cmpRes(L[i],R[j])<=0) A[k++]=L[i++]; else A[k++]=R[j++]; }
    while(i<n1) A[k++]=L[i++]; while(j<n2) A[k++]=R[j++]; delete[] L; delete[] R;
}
void msortResArr(Resume* A, int l, int r){ if(l>=r) return; int m=l+(r-l)/2; msortResArr(A,l,m); msortResArr(A,m+1,r); mergeResArr(A,l,m,r); }

int binSearchJobById(Job* A, int n, const char* id){
    int key = idNumericPart(id);
    int lo=0, hi=n-1;
    while(lo<=hi){
        int mid = lo + (hi-lo)/2;
        int v = idNumericPart(A[mid].id);
        if(v==key) return mid;
        if(v<key) lo=mid+1; else hi=mid-1;
    }
    return -1;
}
int binSearchResById(Resume* A, int n, const char* id){
    int key = idNumericPart(id);
    int lo=0, hi=n-1;
    while(lo<=hi){
        int mid = lo + (hi-lo)/2;
        int v = idNumericPart(A[mid].id);
        if(v==key) return mid;
        if(v<key) lo=mid+1; else hi=mid-1;
    }
    return -1;
}
