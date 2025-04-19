#include "argument.hpp"
#include <format>

namespace argparse
{
    //=============================================================================
    Argument::Argument(const std::string &name, bool required)
        : name_(name),
          help_(),
          value_(),
          required_(required)
    {
    }
    //=============================================================================
    std::string Argument::name() const
    {
        return name_;
    }
    //=============================================================================
    /// @brief Get the argument's help string
    std::string Argument::help() const
    {
        return help_;
    }
    //=============================================================================
    Argument &Argument::help(const std::string &help)
    {
        help_ = help;
        return *this;
    }
    //=============================================================================
    bool Argument::required() const
    {
        return required_;
    }
    //=============================================================================
    std::string Argument::str() const
    {
        return std::format("--{}\t{} (Required {}, Value: {})\n",
                           name_, help_, required_, value_);
    }
}