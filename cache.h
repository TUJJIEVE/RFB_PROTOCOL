#include <thread>
#include "messages.h"
#include <vector>
#include <mutex>
#include <thread>
#include <bits/stdc++.h>
class Cache{
    public:
        Cache(int init){
            rectCache.reserve(init);
            maxSize = init;
        }
        
        void insert(Rectangle);
        Rectangle* lookup(Rectangle &);

    private:
        std::vector<Rectangle> rectCache;
        std::mutex cacheMutex;
        int maxSize;
};