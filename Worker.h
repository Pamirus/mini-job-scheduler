#ifndef WORKER_H
#define WORKER_H

#include <experimental/propagate_const>
#include <memory>

class JobQueue;
class WorkerPrivate;

class Worker
{
public:
    Worker(uint8_t id, JobQueue& queue);
    ~Worker();

    void start();
    void stop();

private:
    std::experimental::propagate_const<std::unique_ptr<WorkerPrivate>> d;
};

#endif // WORKER_H
