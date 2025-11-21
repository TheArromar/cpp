#include "ll_store.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstring>

JobNode::JobNode():next(nullptr){ std::memset(&data,0,sizeof(Job)); }
ResNode::ResNode():next(nullptr){ std::memset(&data,0,sizeof(Resume)); }

JobNode* pushJobFront(JobNode*& head, const Job& j){ JobNode* n=new JobNode(); n->data=j; n->next=head; head=n; return n; }
ResNode* pushResFront(ResNode*& head, const Resume& r){ ResNode* n=new ResNode(); n->data=r; n->next=head; head=n; return n; }

void freeList(JobNode*& h){ JobNode* p=h; while(p){ JobNode* q=p->next; delete p; p=q; } h=nullptr; }
void freeList(ResNode*& h){ ResNode* p=h; while(p){ ResNode* q=p->next; delete p; p=q; } h=nullptr; }

// Merge sort helpers
static JobNode* mergeJob(JobNode* a, JobNode* b){
    if(!a) return b; if(!b) return a;
    JobNode dummy; JobNode* t=&dummy; dummy.next=nullptr;
    while(a&&b){
        if(idNumericPart(a->data.id) <= idNumericPart(b->data.id)){ t->next=a; a=a->next; }
        else{ t->next=b; b=b->next; }
        t=t->next;
    }
    t->next=(a?a:b); return dummy.next;
}
static void splitJob(JobNode* h, JobNode** A, JobNode** B){
    if(!h||!h->next){ *A=h; *B=nullptr; return; }
    JobNode* slow=h; JobNode* fast=h->next;
    while(fast){ fast=fast->next; if(fast){ slow=slow->next; fast=fast->next; } }
    *A=h; *B=slow->next; slow->next=nullptr;
}
void mergeSortJobsLL(JobNode** head){
    JobNode* h=*head; if(!h||!h->next) return; JobNode *A,*B; splitJob(h,&A,&B); mergeSortJobsLL(&A); mergeSortJobsLL(&B); *head=mergeJob(A,B);
}

static ResNode* mergeRes(ResNode* a, ResNode* b){
    if(!a) return b; if(!b) return a;
    ResNode dummy; ResNode* t=&dummy; dummy.next=nullptr;
    while(a&&b){
        if(idNumericPart(a->data.id) <= idNumericPart(b->data.id)){ t->next=a; a=a->next; }
        else{ t->next=b; b=b->next; }
        t=t->next;
    }
    t->next=(a?a:b); return dummy.next;
}
static void splitRes(ResNode* h, ResNode** A, ResNode** B){
    if(!h||!h->next){ *A=h; *B=nullptr; return; }
    ResNode* slow=h; ResNode* fast=h->next;
    while(fast){ fast=fast->next; if(fast){ slow=slow->next; fast=fast->next; } }
    *A=h; *B=slow->next; slow->next=nullptr;
}
void mergeSortResLL(ResNode** head){
    ResNode* h=*head; if(!h||!h->next) return; ResNode *A,*B; splitRes(h,&A,&B); mergeSortResLL(&A); mergeSortResLL(&B); *head=mergeRes(A,B);
}

// Clone (for benchmarks)
JobNode* cloneJobList(JobNode* h){
    JobNode* nh=nullptr;
    JobNode* tail=nullptr;
    for(JobNode* p=h;p;p=p->next){
        JobNode* n=new JobNode();
        n->data = p->data; n->next=nullptr;
        if(!nh){ nh=tail=n; } else { tail->next=n; tail=n; }
    }
    return nh;
}
ResNode* cloneResList(ResNode* h){
    ResNode* nh=nullptr;
    ResNode* tail=nullptr;
    for(ResNode* p=h;p;p=p->next){
        ResNode* n=new ResNode();
        n->data = p->data; n->next=nullptr;
        if(!nh){ nh=tail=n; } else { tail->next=n; tail=n; }
    }
    return nh;
}

// Display
void showFirstJobs(JobNode* head, int n){
    std::cout << "\n--- First " << n << " Jobs ---\n";
    int i=0; for(JobNode* p=head; p && i<n; p=p->next,++i){
        std::cout << p->data.id << " | " << p->data.role << "\n";
    }
}
void showFirstResumes(ResNode* head, int n){
    std::cout << "\n--- First " << n << " Resumes ---\n";
    int i=0; for(ResNode* p=head; p && i<n; p=p->next,++i){
        std::cout << p->data.id << "\n";
    }
}
