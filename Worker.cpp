#include "Worker.h"

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

#include "Console.h"
#include "Job.h"
#include "JobQueue.h"

class WorkerPrivate {
public:
    explicit WorkerPrivate(uint8_t id, JobQueue& queue) : id(id), queue(queue), running(false) {}

    void run();

    uint8_t             id;
    JobQueue&           queue;
    std::atomic<bool>   running;
    std::thread         thread;
};

void WorkerPrivate::run()
{
    while (running) {
        std::shared_ptr<Job> job = queue.pop();
        if (!job)
            break;
        if (!job->changeStatus(JobStatus::PENDING, JobStatus::RUNNING)) {
            continue;
        }

        print("\n[Worker " + std::to_string(id) + "] Started Job "
              + std::to_string(job->getId()) + " (" + job->getName() + ")\n");

        std::this_thread::sleep_for(std::chrono::seconds(job->getDuration()));

        if (job->changeStatus(JobStatus::RUNNING, JobStatus::COMPLETED)) {
            print("\n[Worker " + std::to_string(id) + "] Finished Job "
                  + std::to_string(job->getId()) + "\n");
        }
    }
}

Worker::Worker(uint8_t id, JobQueue& queue)
    : d(std::make_unique<WorkerPrivate>(id, queue)) {}

Worker::~Worker()
{
    stop();
}

void Worker::start()
{
    d->running = true;
    d->thread = std::thread(&WorkerPrivate::run, d.get());
}

void Worker::stop()
{
    if (d->running) {
        d->running = false;
        if (d->thread.joinable()) {
            d->thread.join();
        }
    }
}