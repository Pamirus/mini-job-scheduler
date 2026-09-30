#ifndef JOB_H
#define JOB_H

#include <experimental/propagate_const>
#include <memory>
#include <ctime>


//! Higher priorities run first.
enum class JobPriority : uint8_t {
    LOW = 0,
    NORMAL,
    HIGH
};

enum class JobStatus : uint8_t {
    PENDING = 0,
    RUNNING,
    COMPLETED,
    FAILED,         //!< Not set by the current implementation.
    CANCELLED
};

class JobPrivate;

//! A job's work is simulated: a worker sleeps for its duration. Only the status
//! changes after creation, and it is safe to read and change from any thread.
class Job
{
public:
    //! Starts as @c PENDING, stamped with the current time.
    explicit Job(uint16_t id, std::string name, uint16_t durationSec, JobPriority priority);
    ~Job();
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;
    Job(Job&&) = delete;
    Job& operator=(Job&&) = delete;

    uint16_t       getId() const;
    std::string    getName() const;
    uint16_t       getDuration() const;      //!< In seconds.
    JobPriority    getPriority() const;
    JobStatus      getStatus() const;
    std::time_t    getCreationTime() const;

    //! Atomically moves the job from @p from to @p to; returns @c false if it was
    //! no longer in @p from.
    bool changeStatus(JobStatus from, JobStatus to);

    std::string priorityToString() const;
    std::string statusToString() const;

private:
    std::experimental::propagate_const<std::unique_ptr<JobPrivate>> d;
};

#endif // JOB_H
