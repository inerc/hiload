#include "worker.h"

Worker::Worker(const runFunction &run, QObject *parent) :
   QThread(parent), runMe(run) {}

void Worker::run() {
    runMe();
}
