//! @file
//! @brief The Command interface and the set of CLI commands.

#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>
#include <memory>
#include <sstream>
#include <string>

class Scheduler;

//! @brief A command the CLI can run, such as @c add or @c cancel.
//!
//! Each command parses its own arguments and prints its own output.
class Command
{
public:
    virtual ~Command() = default;

    //! @brief Runs the command.
    //! @param args The rest of the input line, after the command name.
    virtual void        execute(std::stringstream& args) = 0;

    //! Usage line listed by the @c help command.
    virtual std::string usage() const = 0;
};

//! Commands by the name the user types.
using CommandMap = std::map<std::string, std::unique_ptr<Command>>;

//! @brief Adds every CLI command to @p commands.
//!
//! @p scheduler must outlive the commands. The @c help command keeps a
//! reference to @p commands, so the map must not be moved afterwards.
void registerCommands(CommandMap& commands, Scheduler& scheduler);

#endif // COMMANDS_H
