#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>
#include <memory>
#include <sstream>
#include <string>

class Scheduler;
class Command
{
public:
    virtual ~Command() = default;

    virtual void        execute(std::stringstream& args) = 0;
    virtual std::string usage() const = 0;
};

using CommandMap = std::map<std::string, std::unique_ptr<Command>>;

void registerCommands(CommandMap& commands, Scheduler& scheduler);

#endif // COMMANDS_H
