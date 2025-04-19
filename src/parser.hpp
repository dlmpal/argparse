#pragma once

#include "argument.hpp"
#include <array>
#include <vector>

namespace argparse
{
    /// @brief Command-line argument parser
    class ArgParser
    {
    public:
        /// @brief Create a parser
        /// @param prefixes Valid argument prefixes
        /// @param store_all Whether to also store unspecified arguments, i.e.
        /// arguments that have not been added at compile-time
        /// @note The default prefixes are "--" and "-".
        /// Spaces are not valid prefixes
        ArgParser(std::vector<std::string> prefixes = {}, bool store_all = false);

        /// @brief Get the stored arguments
        const std::vector<Argument> &args() const;

        /// @brief Get an argument by its name, if it exists.
        /// If not, returns nullptr
        Argument *get_argument(const std::string &name);

        /// @brief Add an argument to the parser
        /// @note If the argument has already been added, raises an error
        void add_argument(const Argument &arg);

        /// @brief Parse the command-line arguments
        void parse_args(int argc, char *argv[]);

    private:
        /// @brief Parse a single argument, separating its name and value
        std::array<std::string, 2> parse_single_arg(const std::string &arg) const;

    private:
        /// @brief Specified arguments
        std::vector<Argument> args_;

        /// @brief Valid argument prefixes
        std::vector<std::string> prefixes_;

        /// @brief Whether to store unspecified arguments
        bool store_all_;
    };
}