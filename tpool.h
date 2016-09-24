#ifndef TPOOL_H
#define TPOOL_H

#include <vector>
#include "worker.h"
#include <queue>
#include <QMutex>
#include <QWaitCondition>
#include <mutex>
#include <condition_variable>

class TPool
{
    typedef std::function<void ()> taskFunction;
    
    std::vector<WorkerPtr> pool;
    std::queue<taskFunction> tasks;
    
    std::mutex queue_mutex;
    std::condition_variable queue_condition;

public:
    void addTask(const taskFunction& func);
    TPool(size_t threads);
};

#endif // TPOOL_H
