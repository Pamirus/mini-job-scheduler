#ifndef JOB_H
#define JOB_H

#include <experimental/propagate_const>
#include <memory>
#include <ctime>


enum class JobPriority : uint8_t {
    LOW = 0,
    NORMAL,
    HIGH
};

enum class JobStatus : uint8_t {
    PENDING = 0,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
};

class JobPrivate;

class Job
{
public:
    Job(uint16_t id, std::string name, uint16_t durationSec, JobPriority priority);
    ~Job();

    uint16_t       getId() const;
    std::string    getName() const;
    uint16_t       getDuration() const;
    JobPriority    getPriority() const;
    JobStatus      getStatus() const;
    std::time_t    getCreationTime() const;

    void setStatus(JobStatus status);

    std::string priorityToString() const;
    std::string statusToString() const;

    Job(Job&&) noexcept;
    Job& operator=(Job&&) noexcept;
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;

private:
    std::experimental::propagate_const<std::unique_ptr<JobPrivate>> d;
};

#endif // JOB_H
