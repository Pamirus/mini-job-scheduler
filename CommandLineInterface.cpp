#include "CommandLineInterface.h"

#include <iostream>
#include <sstream>

#include "Commands.h"
#include "Scheduler.h"

class CommandLineInterfacePrivate {
public:
    explicit CommandLineInterfacePrivate(Scheduler& scheduler);

    Scheduler& scheduler;
    CommandMap commands;
};

CommandLineInterface::CommandLineInterface(Scheduler& scheduler)
    : d(std::make_unique<CommandLineInterfacePrivate>(scheduler)) {}

CommandLineInterface::~CommandLineInterface() = default;


CommandLineInterfacePrivate::CommandLineInterfacePrivate(Scheduler &scheduler) : scheduler(scheduler)
{
    registerCommands(commands, scheduler);
}

void CommandLineInterface::run() 
{
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
        if (cmd.empty())
            continue;

        auto it = d->commands.find(cmd);
        if (it != d->commands.end())
            it->second->execute(ss);
        else
            std::cout << "Unknown command. Type 'help'.\n";
    }
}
