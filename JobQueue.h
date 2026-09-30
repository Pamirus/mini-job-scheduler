//! @file
//! @brief The JobQueue class.

#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <experimental/propagate_const>
#include <memory>
#include <vector>

class Job;
class JobQueuePrivate;

//! @brief Thread-safe priority queue that hands jobs to the workers.
//!
//! Besides the jobs waiting to run, the queue keeps every job ever pushed, so
//! finished and cancelled jobs can still be listed and looked up.
class JobQueue
{
public:
    JobQueue();
    ~JobQueue();

    //! Adds @p job to the queue and wakes up one waiting worker.
    void push(std::shared_ptr<Job> job);

    //! @brief Blocks until a job is available or the queue is stopped.
    //!
    //! Jobs come out highest priority first; jobs with the same priority do not
    //! necessarily come out in the order they were pushed. Cancelled jobs are
    //! returned too, so the caller has to skip them.
    //! @return The next job, or @c nullptr once the queue is stopped and empty.
    std::shared_ptr<Job> pop();

    //! @brief Cancels a job that has not started yet.
    //!
    //! The job stays in the queue with status @c CANCELLED; see pop().
    //! @return @c true if a @c PENDING job with this @p id was found.
    bool cancel(uint16_t id);

    //! Returns the job with the given @p id, or @c nullptr if there is none.
    std::shared_ptr<Job> findById(uint16_t id) const;

    //! Returns every job ever pushed, in the order they were pushed.
    std::vector<std::shared_ptr<Job>> getAllJobs() const;

    //! @brief Stops the queue and wakes up all waiting workers.
    //!
    //! pop() keeps returning the jobs still in the queue until it is empty.
    //! A stopped queue cannot be restarted.
    void stop();

    //! Returns @c true if no job is waiting to be popped, cancelled ones included.
    bool isEmpty() const;

private:
    std::experimental::propagate_const<std::unique_ptr<JobQueuePrivate>> d;
};

#endif // JOBQUEUE_H
