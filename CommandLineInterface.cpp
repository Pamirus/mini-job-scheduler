#include "CommandLineInterface.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Job.h"
#include "Scheduler.h"

class CommandLineInterfacePrivate {
public:
    explicit CommandLineInterfacePrivate(Scheduler& scheduler) : scheduler(scheduler) {}

    Scheduler& scheduler;

    JobPriority parsePriority(const std::string& str);

    void handleAdd(std::stringstream& ss);

    void handleList();
};

CommandLineInterface::CommandLineInterface(Scheduler& scheduler)
    : d(std::make_unique<CommandLineInterfacePrivate>(scheduler)) {}

CommandLineInterface::~CommandLineInterface() = default;

JobPriority CommandLineInterfacePrivate::parsePriority(const std::string &str)
{
    if (str == "high"   || str == "HIGH"  ) return JobPriority::HIGH;
    if (str == "normal" || str == "NORMAL") return JobPriority::NORMAL;
    return JobPriority::LOW;
}

void CommandLineInterfacePrivate::handleAdd(std::stringstream &ss)
{
    std::string name, prioStr;
    uint16_t duration;
    if (ss >> name >> duration >> prioStr) {
        scheduler.addJob(name, duration, parsePriority(prioStr));
        std::cout << "Job added successfully.\n";
    }
    else {
        std::cout << "Usage: add <name> <duration_sec> <priority: low|normal|high>\n";
    }
}

void CommandLineInterfacePrivate::handleList()
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

void CommandLineInterface::run() {
    std::cout << "=== Mini Job Scheduler CLI ===\nType 'help' for commands.\n\n";
    std::string line;

    while (true) {
        std::cout << "> " << std::flush;
        if (!std::getline(std::cin, line) || line == "quit") {
            d->scheduler.stop();
            break;
        }

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        //! @todo apply polymorphism here
        if (cmd == "add") {
            d->handleAdd(ss);
        }
        else if (cmd == "list") {
            d->handleList();
        }
        else if (cmd == "start") {
            d->scheduler.start();
            std::cout << "Scheduler started with workers.\n";
        }
        else if (cmd == "stop") {
            d->scheduler.stop();
            std::cout << "Scheduler stopped.\n";
        }
        else if (cmd == "cancel") {
            uint16_t id;
            if (ss >> id) {
                if (d->scheduler.cancelJob(id))
                    std::cout << "Job " << id << " cancelled.\n";
                else
                    std::cout << "Cannot cancel job (Not found or already executing/completed).\n";
            }
        }
        else if (cmd == "help") {
            std::cout << "Commands:\n"
                      << "  add <name> <duration> <priority>\n"
                      << "  list\n"
                      << "  start\n"
                      << "  stop\n"
                      << "  cancel <id>\n"
                      << "  quit\n";
        }
        else if (!cmd.empty()) {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }
}
