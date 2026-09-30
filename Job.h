//! @file
//! @brief The Job class and its priority and status enums.

#ifndef JOB_H
#define JOB_H

#include <experimental/propagate_const>
#include <memory>
#include <ctime>


//! Scheduling priority of a job; higher priorities run first.
enum class JobPriority : uint8_t {
    LOW = 0,
    NORMAL,
    HIGH
};

//! Lifecycle state of a job.
enum class JobStatus : uint8_t {
    PENDING = 0,    //!< Waiting in the queue.
    RUNNING,        //!< Being run by a worker.
    COMPLETED,      //!< Finished.
    FAILED,         //!< Reserved; not set by the current implementation.
    CANCELLED       //!< Cancelled before a worker started it.
};

class JobPrivate;

//! @brief A unit of work handled by the scheduler.
//!
//! A job carries no work of its own: a worker simulates it by sleeping for the
//! job's duration. The queue and the workers update its status as it moves
//! through its lifecycle.
class Job
{
public:
    //! Creates a @c PENDING job, stamped with the current time.
    Job(uint16_t id, std::string name, uint16_t durationSec, JobPriority priority);
    ~Job();

    uint16_t       getId() const;            //!< Id assigned by the scheduler.
    std::string    getName() const;          //!< Name given when the job was added.
    uint16_t       getDuration() const;      //!< Run time in seconds.
    JobPriority    getPriority() const;      //!< Scheduling priority.
    JobStatus      getStatus() const;        //!< Current lifecycle state.
    std::time_t    getCreationTime() const;  //!< When the job was created.

    //! Moves the job to a new lifecycle state.
    void setStatus(JobStatus status);

    //! Priority as upper-case text, e.g. @c "HIGH".
    std::string priorityToString() const;
    //! Status as upper-case text, e.g. @c "PENDING".
    std::string statusToString() const;

    Job(Job&&) noexcept;
    Job& operator=(Job&&) noexcept;
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;

private:
    std::experimental::propagate_const<std::unique_ptr<JobPrivate>> d;
};

#endif // JOB_H
