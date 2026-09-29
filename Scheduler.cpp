#include "Scheduler.h"

#include "Job.h"
#include "JobQueue.h"
#include "Worker.h"

#include <atomic>

class SchedulerPrivate {
public:
    SchedulerPrivate(size_t workerCount) : workerCount(workerCount), nextJobId(1) {}

    size_t                                  workerCount;
    std::atomic<uint16_t>                   nextJobId;
    JobQueue                                queue;
    std::vector<std::unique_ptr<Worker>>    workers;
    bool                                    isRunning{false};
};

Scheduler::Scheduler(size_t workerCount)
    : d(std::make_unique<SchedulerPrivate>(workerCount)) {}

Scheduler::~Scheduler() {
    stop();
}

void Scheduler::addJob(const std::string& name, uint16_t duration, JobPriority priority) {
    uint16_t id  = d->nextJobId++;
    auto     job = std::make_shared<Job>(id, name, duration, priority);
    d->queue.push(job);
}

void Scheduler::start() {
    if (d->isRunning) return;

    d->isRunning = true;
    for (size_t i = 0; i < d->workerCount; ++i) {
        auto worker = std::make_unique<Worker>(static_cast<uint8_t>(i + 1), d->queue);
        worker->start();
        d->workers.push_back(std::move(worker));
    }
}

void Scheduler::stop() { //! @todo stop does not run. debug&fix
    if (!d->isRunning) return;

    d->queue.stop();
    for (int i = 0; i < d->workers.size(); ++i) {
        std::unique_ptr<Worker>& worker = d->workers.at(i);
        worker->stop();
    }
    d->workers.clear();
    d->isRunning = false;
}

bool Scheduler::cancelJob(uint16_t id) {
    return d->queue.cancel(id);
}

std::vector<std::shared_ptr<Job>> Scheduler::listJobs() const {
    return d->queue.getAllJobs();
}

std::shared_ptr<Job> Scheduler::getJobStatus(uint16_t id) const { //! @todo add status command to the cli
    return d->queue.findById(id);
}