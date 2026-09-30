#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <experimental/propagate_const>
#include <memory>
#include <vector>

class Job;
class JobQueuePrivate;

//! Thread-safe priority queue shared by the scheduler and its workers. It also
//! keeps every job ever pushed, so finished jobs can still be listed and found.
class JobQueue
{
public:
    JobQueue();
    ~JobQueue();
    JobQueue(const JobQueue&) = delete;
    JobQueue& operator=(const JobQueue&) = delete;
    JobQueue(JobQueue&&) = delete;
    JobQueue& operator=(JobQueue&&) = delete;

    void push(std::shared_ptr<Job> job);

    //! Blocks until a job is available and returns the highest-priority one, or
    //! @c nullptr once the queue is stopped. Cancelled jobs are returned too.
    std::shared_ptr<Job> pop();

    //! Cancels a job that is still @c PENDING; it stays in the queue until popped.
    bool cancel(uint16_t id);

    std::shared_ptr<Job> findById(uint16_t id) const;
    std::vector<std::shared_ptr<Job>> getAllJobs() const;

    //! Reopens the queue after stop().
    void start();

    //! Wakes up waiting workers and makes pop() return @c nullptr; jobs still in
    //! the queue wait for start().
    void stop();

    bool isEmpty() const;

private:
    std::experimental::propagate_const<std::unique_ptr<JobQueuePrivate>> d;
};

#endif // JOBQUEUE_H
