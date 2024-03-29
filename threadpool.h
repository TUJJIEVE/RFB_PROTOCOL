#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <vector>
#include <stdarg.h>

#ifndef THREADPOOL_H
#define THREADPOOL_H

class ThreadPool{
    
    public:
        
        ThreadPool();
        void start(int numt);
        void addJobs(int count,std::function<int(int)> job,...);
        void waitingRoom();
        void stop();
        void destroy();
    
    private:
        
        int numThreads;
        std::queue<std::function<int(int)>> jobQ;
        std::condition_variable cond;
        std::mutex jobMutex;
        std::vector<std::thread> jobThreads;
        bool isOver;
};

#endif