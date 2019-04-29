#include <thread>
#include <vector>
#include <mutex>
#include <thread>
#include <bits/stdc++.h>

#ifndef CACHE_H
#define CACHE_H

#include "messages.h"



class Cache{
    public:
        Cache(int init){
            rectCache.reserve(init);
            maxSize = init;
        }
        Cache(){
            maxSize = 10;
        }
        void insert(Rectangle);
        Rectangle* lookup(Rectangle &);

    private:
        std::vector<Rectangle> rectCache;
        std::mutex cacheMutex;
        int maxSize;
};


#endif