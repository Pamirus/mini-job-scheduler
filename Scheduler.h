#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <experimental/propagate_const>
#include <memory>
#include <vector>

class Job;
class SchedulerPrivate;
enum class JobPriority : uint8_t;

//! Owns the job queue and a pool of workers.
class Scheduler {
public:
    explicit Scheduler(size_t workerCount = 2);
    ~Scheduler();
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    //! @p duration is in seconds. Ids are assigned in order, starting from 1.
    void addJob(const std::string& name, uint16_t duration, JobPriority priority);

    //! Does nothing if the workers are already running.
    void start();

    //! Lets running jobs finish; jobs that have not started stay @c PENDING.
    void stop();

    //! Cancels a job that has not started yet; returns @c false otherwise.
    bool cancelJob(uint16_t id);

    std::vector<std::shared_ptr<Job>>   listJobs() const;
    std::shared_ptr<Job>                findJob(uint16_t id) const;

private:
    std::experimental::propagate_const<std::unique_ptr<SchedulerPrivate>> d;
};

#endif // SCHEDULER_H
