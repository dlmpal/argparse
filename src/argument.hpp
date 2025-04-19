#pragma once

#include <string>

namespace argparse
{
    /// @brief An argument is defined by its name, its value (as a character string),
    /// and an optional help string. By default, the arguments have an empty value and
    /// help string
    class Argument
    {
    public:
        /// @brief Create an argument with a specified name
        Argument(const std::string &name, bool required = false);

        /// @brief Get the argument's name
        std::string name() const;

        /// @brief Get the argument's value in the specified return type.
        /// @note If the value string is empty and the return type is not
        /// string or bool, raises an exception. For empty value string and bool
        /// return type, returns true
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
                return value_;
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

        /// @brief Returns whether the argument is required
        bool required() const;

        /// @brief Get the string representation of the argument
        std::string str() const;

    private:
        /// @brief Name
        std::string name_;

        /// @brief Value (as string)
        std::string value_;

        /// @brief Help string
        std::string help_;

        /// @brief Whether the variable's value must be specified
        bool required_;
    };
}