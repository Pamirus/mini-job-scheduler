#include "Commands.h"

#include <ctime>
#include <iomanip>
#include <limits>

#include "Console.h"
#include "Job.h"
#include "Scheduler.h"

namespace {

class SchedulerCommand : public Command
{
public:
    explicit SchedulerCommand(Scheduler& scheduler) : scheduler(scheduler) {}

protected:
    Scheduler& scheduler;
};

class AddCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream& args) override
    {
        std::string name, prioStr;
        int duration;
        if (!(args >> name >> duration >> prioStr)) {
            print("Usage: " + usage() + "\n");
            return;
        }
        if (duration < 0 || duration > maxDuration) {
            print("Duration must be between 0 and " + std::to_string(maxDuration) + " seconds.\n");
            return;
        }
        JobPriority priority{};
        if (!parsePriority(prioStr, priority)) {
            print("Unknown priority '" + prioStr + "'. Use low, normal or high.\n");
            return;
        }
        scheduler.addJob(name, static_cast<uint16_t>(duration), priority);
        print("Job " + name + " added.\n");
    }

    std::string usage() const override
    {
        return "add <name> <duration_sec> <priority: low|normal|high>";
    }

private:
    static constexpr int maxDuration = std::numeric_limits<uint16_t>::max();
    static bool parsePriority(const std::string& str, JobPriority& priority)
    {
        if (str == "low"    || str == "LOW"   ) { priority = JobPriority::LOW;    return true; }
        if (str == "normal" || str == "NORMAL") { priority = JobPriority::NORMAL; return true; }
        if (str == "high"   || str == "HIGH"  ) { priority = JobPriority::HIGH;   return true; }
        return false;
    }
};

class ListCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream&) override
    {
        auto jobs = scheduler.listJobs();
        std::ostringstream out;
        out << "\n" << std::left << std::setw(6) << "ID"
            << std::setw(15) << "NAME"
            << std::setw(10) << "PRIORITY"
            << std::setw(12) << "STATUS" << "\n";
        out << std::string(43, '-') << "\n";
        for (const auto& job : jobs) {
            out << std::left << std::setw(6) << job->getId()
            << std::setw(15) << job->getName()
            << std::setw(10) << job->priorityToString()
            << std::setw(12) << job->statusToString() << "\n";
        }
        out << "\n";
        print(out.str());
    }

    std::string usage() const override
    {
        return "list";
    }
};

class StartCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream&) override
    {
        scheduler.start();
        print("Scheduler started with workers.\n");
    }

    std::string usage() const override
    {
        return "start";
    }
};

class StopCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream&) override
    {
        scheduler.stop();
        print("Scheduler stopped.\n");
    }

    std::string usage() const override
    {
        return "stop";
    }
};

class CancelCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream& args) override
    {
        uint16_t id;
        if (!(args >> id)) {
            print("Usage: " + usage() + "\n");
            return;
        }
        if (scheduler.cancelJob(id))
            print("Job " + std::to_string(id) + " cancelled.\n");
        else
            print("Cannot cancel job (Not found or already executing/completed).\n");
    }

    std::string usage() const override
    {
        return "cancel <id>";
    }
};

class StatusCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream& args) override
    {
        uint16_t id;
        if (!(args >> id)) {
            print("Usage: " + usage() + "\n");
            return;
        }
        auto job = scheduler.findJob(id);
        if (!job) {
            print("Job " + std::to_string(id) + " not found.\n");
            return;
        }
        std::time_t created = job->getCreationTime();
        std::ostringstream out;
        out << "Name:     " << job->getName() << "\n"
            << "Priority: " << job->priorityToString() << "\n"
            << "Status:   " << job->statusToString() << "\n"
            << "Duration: " << job->getDuration() << " sec\n"
            << "Created:  " << std::put_time(std::localtime(&created), "%H:%M:%S") << "\n";
        print(out.str());
    }

    std::string usage() const override
    {
        return "status <id>";
    }
};

class HelpCommand : public Command
{
public:
    explicit HelpCommand(const CommandMap& commands) : commands(commands) {}

    void execute(std::stringstream&) override
    {
        std::string text = "Commands:\n";
        for (const auto& entry : commands)
            text += "  " + entry.second->usage() + "\n";
        print(text + "  quit\n");
    }

    std::string usage() const override
    {
        return "help";
    }

private:
    const CommandMap& commands;
};

} // namespace

void registerCommands(CommandMap& commands, Scheduler& scheduler)
{
    commands["add"]    = std::make_unique<AddCommand>(scheduler);
    commands["list"]   = std::make_unique<ListCommand>(scheduler);
    commands["start"]  = std::make_unique<StartCommand>(scheduler);
    commands["stop"]   = std::make_unique<StopCommand>(scheduler);
    commands["cancel"] = std::make_unique<CancelCommand>(scheduler);
    commands["status"] = std::make_unique<StatusCommand>(scheduler);
    commands["help"]   = std::make_unique<HelpCommand>(commands);
}
