//! @file
//! @brief The Scheduler class.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <experimental/propagate_const>
#include <memory>
#include <vector>

class Job;
class SchedulerPrivate;
enum class JobPriority : uint8_t;

//! @brief Owns the job queue and a pool of workers.
//!
//! This is the interface the CLI works with: jobs are added, cancelled and
//! listed here, and the workers are started and stopped here.
class Scheduler {
public:
    //! Creates a scheduler that runs @p workerCount workers once started.
    explicit Scheduler(size_t workerCount = 2);

    //! Stops the scheduler; see stop().
    ~Scheduler();

    //! @brief Queues a job that runs for @p duration seconds.
    //!
    //! Ids are assigned in order, starting from 1. Jobs can be added whether or
    //! not the scheduler is running.
    void addJob(const std::string& name, uint16_t duration, JobPriority priority);

    //! Starts the workers. Does nothing if they are already running.
    void start();

    //! Stops the queue and waits for all workers to exit. Does nothing if not running.
    void stop();

    //! @brief Cancels a job that has not started yet.
    //! @return @c true if a @c PENDING job with this @p id was found.
    bool cancelJob(uint16_t id);

    //! Returns every job added so far, in the order they were added.
    std::vector<std::shared_ptr<Job>>   listJobs() const;

    //! Returns the job with the given @p id, or @c nullptr if there is none.
    std::shared_ptr<Job>                findJob(uint16_t id) const;

private:
    std::experimental::propagate_const<std::unique_ptr<SchedulerPrivate>> d;
};

#endif // SCHEDULER_H
