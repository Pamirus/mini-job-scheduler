#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <experimental/propagate_const>
#include <memory>
#include <vector>

class Job;
class SchedulerPrivate;
enum class JobPriority : uint8_t;

class Scheduler {
public:
    explicit Scheduler(size_t workerCount = 2);
    ~Scheduler();

    void addJob(const std::string& name, uint16_t duration, JobPriority priority);
    void start();
    void stop();
    bool cancelJob(uint16_t id);

    std::vector<std::shared_ptr<Job>>   listJobs() const;
    std::shared_ptr<Job>                findJob(uint16_t id) const;

private:
    std::experimental::propagate_const<std::unique_ptr<SchedulerPrivate>> d;
};

#endif // SCHEDULER_H
