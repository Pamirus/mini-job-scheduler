#ifndef WORKER_H
#define WORKER_H

#include <experimental/propagate_const>
#include <memory>

class JobQueue;
class WorkerPrivate;

//! Runs jobs from a JobQueue on its own thread; a job is simulated by sleeping
//! for its duration.
class Worker
{
public:
    //! @p queue must outlive the worker; @p id only appears in log messages.
    Worker(uint8_t id, JobQueue& queue);
    ~Worker();
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    Worker(Worker&&) = delete;
    Worker& operator=(Worker&&) = delete;

    //! Must not be called while the worker is running.
    void start();

    //! Stops the worker and waits for its thread to exit.
    void stop();

private:
    std::experimental::propagate_const<std::unique_ptr<WorkerPrivate>> d;
};

#endif // WORKER_H
