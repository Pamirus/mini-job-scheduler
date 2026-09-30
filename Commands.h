#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>
#include <memory>
#include <sstream>
#include <string>

class Scheduler;

//! A CLI command; it parses its own arguments and prints its own output.
class Command
{
public:
    virtual ~Command() = default;

    //! @p args is the rest of the input line, after the command name.
    virtual void        execute(std::stringstream& args) = 0;

    //! Usage line listed by the @c help command.
    virtual std::string usage() const = 0;
};

using CommandMap = std::map<std::string, std::unique_ptr<Command>>;

//! @p scheduler must outlive the commands, and @p commands must not be moved
//! afterwards because the @c help command keeps a reference to it.
void registerCommands(CommandMap& commands, Scheduler& scheduler);

#endif // COMMANDS_H
