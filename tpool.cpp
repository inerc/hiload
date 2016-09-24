#include "tpool.h"

TPool::TPool(size_t threads) {
    for (size_t i = 0; i < threads; i++) {

        WorkerPtr workerPtr = std::make_unique<Worker>([this] {
            forever {
                taskFunction task = nullptr;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    queue_condition.wait(lock, [this] { return !tasks.empty(); });
                    task = tasks.front();
                    tasks.pop();
                }
                if (task) task();
            }
        });
        workerPtr->start();
        pool.push_back(std::move(workerPtr));
    }
}

void TPool::addTask(const taskFunction& func) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(func);
    }
    queue_condition.notify_one();
}
