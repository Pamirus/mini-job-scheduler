#include "JobQueue.h"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <algorithm>

#include "Job.h"

struct JobComparator {
    bool operator()(const std::shared_ptr<Job>& firstJob, const std::shared_ptr<Job>& secondJob) const {
        return static_cast<uint8_t>(firstJob->getPriority()) < static_cast<uint8_t>(secondJob->getPriority());
    }
};

class JobQueuePrivate {
    using JobPtr        = std::shared_ptr<Job>;
    using PriorityQueue = std::priority_queue<JobPtr, std::vector<JobPtr>, JobComparator>;

public:
    mutable std::mutex      mutex;
    std::condition_variable cv;
    PriorityQueue           priorityQueue;
    std::vector<JobPtr>     allJobsHistory;
    bool                    isStopped{false};
};

JobQueue::JobQueue() : d(std::make_unique<JobQueuePrivate>()) {}
JobQueue::~JobQueue() = default;

void JobQueue::push(std::shared_ptr<Job> job)
{
    {
        std::lock_guard<std::mutex> lock(d->mutex);
        d->priorityQueue.push(job);
        d->allJobsHistory.push_back(job);
    }
    d->cv.notify_one();
}

std::shared_ptr<Job> JobQueue::pop()
{
    std::unique_lock<std::mutex> lock(d->mutex);
    d->cv.wait(lock, [this]() {
        return !d->priorityQueue.empty() || d->isStopped;
    });

    if (d->isStopped || d->priorityQueue.empty()) {
        return nullptr;
    }

    auto job = d->priorityQueue.top();
    d->priorityQueue.pop();
    return job;
}

bool JobQueue::cancel(uint16_t id)
{
    std::lock_guard<std::mutex> lock(d->mutex);
    auto it = std::find_if(d->allJobsHistory.begin(), d->allJobsHistory.end(),
                           [id](const std::shared_ptr<Job>& job) { return job->getId() == id; });
    if (it != d->allJobsHistory.end() && (*it)->getStatus() == JobStatus::PENDING) {
        (*it)->setStatus(JobStatus::CANCELLED);
        return true;
    }
    return false;
}

std::shared_ptr<Job> JobQueue::findById(uint16_t id) const
{
    std::lock_guard<std::mutex> lock(d->mutex);
    auto it = std::find_if(d->allJobsHistory.begin(), d->allJobsHistory.end(),
                           [id](const std::shared_ptr<Job>& job) { return job->getId() == id; });
    return (it != d->allJobsHistory.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<Job>> JobQueue::getAllJobs() const
{
    std::lock_guard<std::mutex> lock(d->mutex);
    return d->allJobsHistory;
}

void JobQueue::start()
{
    std::lock_guard<std::mutex> lock(d->mutex);
    d->isStopped = false;
}

void JobQueue::stop()
{
    {
        std::lock_guard<std::mutex> lock(d->mutex);
        d->isStopped = true;
    }
    d->cv.notify_all();
}

bool JobQueue::isEmpty() const
{
    std::lock_guard<std::mutex> lock(d->mutex);
    return d->priorityQueue.empty();
}