


#include "cache.h"


void Cache::insert(Rectangle rect){
   {
        std::unique_lock<std::mutex> l(cacheMutex);
        if (rectCache.size() == maxSize){
            rectCache.erase(rectCache.end());
            rectCache.insert(rectCache.begin(),rect);        
        }
        else{
            rectCache.insert(rectCache.begin(),rect);
        }

   }
}

Rectangle * Cache::lookup(Rectangle &rect){
    {
        std::unique_lock<std::mutex> l(cacheMutex);
        std::vector<Rectangle>::iterator it = std::find(rectCache.begin(),rectCache.end(),rect);
        if(it == rectCache.end()){
            return nullptr; 
        }
        auto temp = *it;
        rectCache.erase(it);
        rectCache.insert(rectCache.begin(),std::move(temp));

        return &*(rectCache.begin());
    
    }
}

