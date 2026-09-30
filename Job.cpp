#include "Job.h"

#include <atomic>
#include <string>

class JobPrivate {
public:
    explicit JobPrivate(uint16_t id, std::string name, uint16_t duration, JobPriority priority)
        : id(id)
        , name(std::move(name))
        , duration(duration)
        , priority(priority)
        , status(JobStatus::PENDING)
        , creationTime(std::time(nullptr)) {}

    uint16_t               id;
    std::string            name;
    uint16_t               duration;
    JobPriority            priority;
    std::atomic<JobStatus> status;
    std::time_t            creationTime;
};

Job::Job(uint16_t id, std::string name, uint16_t durationSec, JobPriority priority)
    : d(std::make_unique<JobPrivate>(id, std::move(name), durationSec, priority)) {}

Job::~Job() = default;

uint16_t    Job::getId() const {            return d->id;           }
std::string Job::getName() const {          return d->name;         }
uint16_t    Job::getDuration() const {      return d->duration;     }
JobPriority Job::getPriority() const {      return d->priority;     }
JobStatus   Job::getStatus() const {        return d->status;       }
std::time_t Job::getCreationTime() const {  return d->creationTime; }

bool Job::changeStatus(JobStatus from, JobStatus to)
{
    return d->status.compare_exchange_strong(from, to);
}

std::string Job::priorityToString() const
{
    switch (d->priority) {
    case JobPriority::LOW:      return "LOW";
    case JobPriority::NORMAL:   return "NORMAL";
    case JobPriority::HIGH:     return "HIGH";
    }
    return "UNKNOWN";
}

std::string Job::statusToString() const
{
    switch (d->status) {
    case JobStatus::PENDING:      return "PENDING";
    case JobStatus::RUNNING:      return "RUNNING";
    case JobStatus::COMPLETED:    return "COMPLETED";
    case JobStatus::FAILED:       return "FAILED";
    case JobStatus::CANCELLED:    return "CANCELLED";
    }
    return "UNKNOWN";
}