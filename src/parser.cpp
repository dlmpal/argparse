#include "parser.hpp"
#include <algorithm>
#include <format>
#include <set>
#include <iostream>

namespace argparse
{
    //=============================================================================
    ArgParser::ArgParser(std::vector<std::string> prefixes, bool store_all)
        : prefixes_(prefixes),
          store_all_(store_all)
    {
        if (prefixes_.empty())
        {
            prefixes_ = {"--", "-"};
        }

        // Sort the prefixes in descending lexicographical order
        std::sort(prefixes_.begin(), prefixes_.end());
        std::reverse(prefixes_.begin(), prefixes_.end());
    }
    //=============================================================================
    const std::vector<Argument> &ArgParser::args() const
    {
        return args_;
    }
    //=============================================================================
    Argument *ArgParser::get_argument(const std::string &name)
    {
        if (auto it = std::find_if(args_.begin(), args_.end(),
                                   [&name](const Argument &arg)
                                   {
                                       if (name == arg.name())
                                       {
                                           return true;
                                       }
                                       else
                                       {
                                           return false;
                                       }
                                   });
            it != args_.end())
        {
            return &(*it);
        }
        else
        {
            return nullptr;
        }
    }
    //=============================================================================
    void ArgParser::add_argument(const Argument &arg)
    {
        if (get_argument(arg.name()) != nullptr)
        {
            std::cerr << std::format("Cannot specify the same argument ({}) twice\n",
                                     arg.name());
            std::exit(EXIT_FAILURE);
        }
        args_.emplace_back(arg);
    }
    //=============================================================================
    void ArgParser::parse_args(int argc, char *argv[])
    {
        // If requested, print the help message and exit
        if (argc > 1 and (std::string(argv[1]) == "-h" or
                          std::string(argv[1]) == "--help"))
        {
            std::string help_msg;
            for (const auto &arg : args_)
            {
                help_msg += arg.str();
            }
            std::cout << help_msg;
            std::exit(EXIT_SUCCESS);
        }

        // Keep track of the specified arguments which have been provided
        std::set<std::string> provided;

        // Loop over the provided arguments, skipping the first
        for (int i = 1; i < argc; i++)
        {
            auto [name, value] = parse_single_arg(argv[i]);

            // Disregard invalid arguments
            if (name.empty())
            {
                continue;
            }

            // If the argument is specified, modify its value
            // Else, if store_all_ is true, store the new unspecified argument
            if (auto arg = get_argument(name); arg != nullptr)
            {
                arg->value(value);
                provided.insert(name);
            }
            else if (store_all_)
            {
                args_.emplace_back(name).value(value);
            }
        }

        // Check if any required arguments were not provided
        for (const auto &arg : args_)
        {
            if (arg.required() and !provided.contains(arg.name()))
            {
                std::cerr << std::format("Required argument {} was not provided\n",
                                         arg.name());
                std::exit(EXIT_FAILURE);
            }
        }
    }
    //=============================================================================
    std::array<std::string, 2>
    ArgParser::parse_single_arg(const std::string &arg) const
    {
        for (const auto prefix : prefixes_)
        {
            // Get the start and end position of the prefix
            std::size_t prefix_start = arg.find(prefix);
            std::size_t prefix_end = prefix_start + prefix.size();

            // Require that all arguments begin with the prefix
            if (prefix_start >= arg.size())
            {
                continue;
            }

            // Get the start and end position of the delimiter
            const char *delim = "=";
            const std::size_t delim_len = 1;
            std::size_t delim_start = std::min(arg.find(delim), arg.size());
            std::size_t delim_end = delim_start + delim_len;

            // Get the name
            auto name = arg.substr(prefix_end, delim_start - prefix_end);

            // Get the value
            std::string value = "";
            if (delim_end < arg.size())
            {
                value = arg.substr(delim_end);
            }

            return {name, value};
        }

        // Warn that the argument is invalid
#ifdef DEBUG
        std::cout << std::format("Invalid command-line argument: {}\n", arg);
#endif
        return {};
    }
}