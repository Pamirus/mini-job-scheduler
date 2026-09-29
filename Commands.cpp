#include "Commands.h"

#include <ctime>
#include <iomanip>
#include <iostream>

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
        uint16_t duration;
        if (!(args >> name >> duration >> prioStr)) {
            std::cout << "Usage: " << usage() << "\n";
            return;
        }
        scheduler.addJob(name, duration, parsePriority(prioStr));
        std::cout << "Job " << name << " added.\n";
    }

    std::string usage() const override
    {
        return "add <name> <duration_sec> <priority: low|normal|high>";
    }

private:
    static JobPriority parsePriority(const std::string& str)
    {
        if (str == "high"   || str == "HIGH"  ) return JobPriority::HIGH;
        if (str == "normal" || str == "NORMAL") return JobPriority::NORMAL;
        return JobPriority::LOW;
    }
};

class ListCommand : public SchedulerCommand
{
public:
    using SchedulerCommand::SchedulerCommand;

    void execute(std::stringstream&) override
    {
        auto jobs = scheduler.listJobs();
        std::cout << "\n" << std::left << std::setw(6) << "ID"
                  << std::setw(15) << "NAME"
                  << std::setw(10) << "PRIORITY"
                  << std::setw(12) << "STATUS" << "\n";
        std::cout << std::string(43, '-') << "\n";
        for (const auto& job : jobs) {
            std::cout << std::left << std::setw(6) << job->getId()
            << std::setw(15) << job->getName()
            << std::setw(10) << job->priorityToString()
            << std::setw(12) << job->statusToString() << "\n";
        }
        std::cout << std::endl;
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
        std::cout << "Scheduler started with workers.\n";
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
        std::cout << "Scheduler stopped.\n";
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
            std::cout << "Usage: " << usage() << "\n";
            return;
        }
        if (scheduler.cancelJob(id))
            std::cout << "Job " << id << " cancelled.\n";
        else
            std::cout << "Cannot cancel job (Not found or already executing/completed).\n";
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
            std::cout << "Usage: " << usage() << "\n";
            return;
        }
        auto job = scheduler.findJob(id);
        if (!job) {
            std::cout << "Job " << id << " not found.\n";
            return;
        }
        std::time_t created = job->getCreationTime();
        std::cout << "Name:     " << job->getName() << "\n"
                  << "Priority: " << job->priorityToString() << "\n"
                  << "Status:   " << job->statusToString() << "\n"
                  << "Duration: " << job->getDuration() << " sec\n"
                  << "Created:  " << std::put_time(std::localtime(&created), "%H:%M:%S") << "\n";
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
        std::cout << "Commands:\n";
        for (const auto& entry : commands)
            std::cout << "  " << entry.second->usage() << "\n";
        std::cout << "  quit\n";
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
