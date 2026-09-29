#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <experimental/propagate_const>
#include <memory>
#include <vector>

class Job;
class JobQueuePrivate;

class JobQueue
{
public:
    JobQueue();
    ~JobQueue();

    void push(std::shared_ptr<Job> job);
    std::shared_ptr<Job> pop();
    bool cancel(uint16_t id);
    std::shared_ptr<Job> findById(uint16_t id) const;
    std::vector<std::shared_ptr<Job>> getAllJobs() const;

    void stop();
    bool isEmpty() const;

private:
    std::experimental::propagate_const<std::unique_ptr<JobQueuePrivate>> d;
};

#endif // JOBQUEUE_H
