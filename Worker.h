//! @file
//! @brief The Worker class.

#ifndef WORKER_H
#define WORKER_H

#include <experimental/propagate_const>
#include <memory>

class JobQueue;
class WorkerPrivate;

//! @brief Runs jobs from a JobQueue on its own thread.
//!
//! A worker keeps taking jobs until the queue runs dry after being stopped, or
//! until the worker itself is stopped. It runs a job by sleeping for the job's
//! duration and reports the start and the end on standard output.
class Worker
{
public:
    //! @brief Creates a worker; it does not run until start() is called.
    //! @param id    Number shown in the worker's log messages.
    //! @param queue Queue to take jobs from; must outlive the worker.
    Worker(uint8_t id, JobQueue& queue);

    //! Stops the worker; see stop().
    ~Worker();

    //! @brief Starts the worker thread.
    //! @pre The worker is not running.
    void start();

    //! @brief Stops the worker and waits for its thread to exit.
    //!
    //! A job that has already started is finished first.
    //! @warning Call JobQueue::stop() first. This call does not wake up a worker
    //!          that is waiting in JobQueue::pop(), so it would block forever.
    void stop();

private:
    std::experimental::propagate_const<std::unique_ptr<WorkerPrivate>> d;
};

#endif // WORKER_H
