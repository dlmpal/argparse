#pragma once

#include <array>
#include <vector>
#include <string>

using real_t = double;

namespace argparse
{
    /// @brief An argument is defined by its name, its value (as a character string),
    /// and an optional help string. By default, the arguments have an empty value and
    /// help string
    class Argument
    {
    public:
        /// @brief Create an argument with a specified name
        Argument(const std::string &name);

        /// @brief Get the argument's name
        std::string name() const;

        /// @brief Get the argument's value in the specified return type
        template <typename T>
        auto value() const
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                if (value_.empty() or value_ == "true")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                return std::stoi(value_);
            }
            else if constexpr (std::is_same_v<T, real_t>)
            {
                if constexpr (std::is_same_v<real_t, float>)
                {
                    return std::stof(value_);
                }
                else
                {
                    return std::stod(value_);
                }
            }
            else
            {
                return value;
            }
        }

        /// @brief Set the argument's value
        /// @note Returns the argument
        template <typename T>
        Argument &value(const T &value)
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                value_ = value ? "true" : "false";
            }
            else if constexpr (not std::is_same_v<T, std::string> and
                               not std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, char> and
                               not std::is_same_v<std::remove_const_t<std::remove_extent_t<T>>, char>)
            {
                value_ = std::to_string(value);
            }
            else
            {
                value_ = value;
            }

            return *this;
        }

        /// @brief Get the argument's help string
        std::string help() const;

        /// @brief Set the argument's help string
        /// @note Returns the argument
        Argument &help(const std::string &help);

        /// @brief Get the string representation of the argument
        std::string str() const;

    private:
        /// @brief Name
        std::string name_;

        /// @brief Value (as string)
        std::string value_;

        /// @brief Help string
        std::string help_;
    };

    /// @brief Command-line argument parser
    class ArgParser
    {
    public:
        /// @brief Create a parser with a specified list of argument valid argument prefixes
        /// @note The default prefixes are "--" and "-"
        /// @note Spaces are not valid prefixes
        ArgParser(std::vector<std::string> prefixes = {});

        /// @brief Get the list of specified arguments
        const std::vector<Argument> &args() const;

        /// @brief Add an argument to the parser
        void add_argument(const Argument &arg);

        /// @brief Get an argument by its name, if it exists.
        /// If not, returns nullptr
        Argument *get_argument(const std::string &name);

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
    };
}