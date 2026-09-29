#include "Worker.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "Job.h"
#include "JobQueue.h"

class WorkerPrivate {
public:
    WorkerPrivate(uint8_t id, JobQueue& queue) : id(id), queue(queue), running(false) {}

    void run();

    uint8_t             id;
    JobQueue&           queue;
    std::atomic<bool>   running;
    std::thread         thread;
};

void WorkerPrivate::run() {
    while (running) {
        std::shared_ptr<Job> job = queue.pop();
        if (!job)
            break;

        if (job->getStatus() == JobStatus::CANCELLED) {
            continue;
        }

        job->setStatus(JobStatus::RUNNING);
        std::cout << "\n[Worker " << static_cast<int>(id) << "] Started Job "
                  << job->getId() << " (" << job->getName() << ")" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(job->getDuration()));

        if (job->getStatus() == JobStatus::RUNNING) {
            job->setStatus(JobStatus::COMPLETED);
            std::cout << "\n[Worker " << static_cast<int>(id) << "] Finished Job "
                      << job->getId() << std::endl;
        }
    }
}

Worker::Worker(uint8_t id, JobQueue& queue)
    : d(std::make_unique<WorkerPrivate>(id, queue)) {}

Worker::~Worker() {
    stop();
}

void Worker::start() {
    d->running = true;
    d->thread = std::thread(&WorkerPrivate::run, d.get());
}

void Worker::stop() {
    if (d->running) {
        d->running = false;
        if (d->thread.joinable()) {
            d->thread.join();
        }
    }
}