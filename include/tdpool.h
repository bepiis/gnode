//
//  tdpool.h
//  Created by Ben Westcott on 9/12/24.
//

#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <stdexcept>

class tdpool
{
    
public:

    tdpool() = delete;
    tdpool(tdpool const& other) = delete;
    tdpool& operator=(tdpool const& other) = delete; 

    explicit inline tdpool(size_t nt)
    : stop(false)
    {
        /*
         * initialize each worker in the pool with
         * a func which takes a task from the task queue
         * and executes it.
         */
        for(size_t i=0; i < nt; ++i)
        {
            workers.emplace_back
            (
             [this]
             {
                 for(;;)
                 {
                     std::function<void()> task;
                     {
                         std::unique_lock<std::mutex> lock(qmutex);
                         
                         // wait until no other workers are modifying the queue
                         cv.wait
                         (
                          // calls lock.lock() when cv.notify_(one/all)() and lock.unlock() is executed
                          lock,
                          // extra condition to break loop if stop condition is called or the task queue is empty
                          [this] { return stop || !tasks.empty(); }
                          );
                         
                         if(stop && tasks.empty())
                         {
                             return;
                         }
                         
                         // now that the queue is locked to this thread, move the task into a variable, and remove it from the queue.
                         task = std::move(tasks.front());
                         tasks.pop();
                     }
                     
                     // execute the task
                     task();
                 }
                 
             }
             );
        }
        }
    
    // ... indicates that F may take any number of arguments
    template<class F, class... Args>
    // && here indicates the function and the args are to be interpreted as r-values
    auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type>
    
    {
        using return_type = typename std::invoke_result<F, Args...>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>
        (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(qmutex);
            
            if(stop)
            {
                throw std::runtime_error("enqueue stopped.");
            }
            
            tasks.emplace
            (
                [task]()
                {
                    (*task)();
                }
            );
        }
        
        cv.notify_one();
        return res;
    }
    
    ~tdpool()
    {
        {
            std::unique_lock<std::mutex> lock(qmutex);
            stop = true;
        }
        
        cv.notify_all();
        for(std::thread& worker : workers)
        {
            worker.join();
        }
    }
        

    
private:
    
    bool stop;
    std::vector<std::thread> workers;
    std::mutex qmutex;
    std::condition_variable cv;
    std::queue<std::function<void()>> tasks;
    
    
};

