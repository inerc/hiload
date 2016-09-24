#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <functional>
#include <memory>

class Worker : public QThread
{
    Q_OBJECT

    typedef std::function<void()> runFunction;
    runFunction runMe;

public:
    explicit Worker(const runFunction &run, QObject *parent = 0);

protected:
    void run();
};

namespace std {

    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args ) {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }

}

typedef std::unique_ptr<Worker> WorkerPtr;

#endif // WORKER_H
