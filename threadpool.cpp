#include "threadpool.h"



ThreadPool::ThreadPool(){
    isOver = false;

}
void ThreadPool::start(int numt){
    numThreads = numt;
    jobThreads.reserve(numThreads);
    
    for (int i =0;i<numThreads;i++){
        jobThreads.push_back(std::thread(&ThreadPool::waitingRoom,this));
    }

}

void ThreadPool::addJobs(int count, std::function<int(int)> job,...){
    va_list jobs;
    va_start(jobs,job);
    {
        std::unique_lock<std::mutex> l(jobMutex);
        jobQ.push(job);
        cond.notify_all();
    }

    for (int i=1;i<count;i++){
        {
            std::unique_lock<std::mutex> l(jobMutex);
            jobQ.push(va_arg(jobs, std::function <int(int) > ));
            cond.notify_all();
        }
    }
    va_end(jobs);
    return;
}

void ThreadPool::waitingRoom(){
    std::function<int(int)> job;

    while(true){

        {
            std::unique_lock<std::mutex> l(jobMutex);
            while(jobQ.size() == 0 && !isOver){
                cond.wait(l);
            }
            if (isOver) return;
            job = jobQ.front();
            jobQ.pop();
        }

        job(1);
        
    }
}

void ThreadPool::stop(){
    isOver = true;
    cond.notify_all();
    for (int i=0;i<numThreads;i++){
        jobThreads.at(i).join();
    }
    return;
}

