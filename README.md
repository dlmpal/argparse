# ArgParse

Lightweight parser for command-line arguments in modern C++

## Requirements
* C++ 23 (and above)
* CMake (3.16 and above)

## Installation
Begin by cloning the repo:
```
git clone https://github.com/dlmpal/argparse.git
```
Once inside the argparse directory, run
```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$ARGPARSE_INSTALL_DIR
```
where **$ARGPARSE_INSTALL_DIR** is the directory in which argparse will be installed. Finally, run:
```
(cd build; make install)
```

## Example usage
An example of using argparse can be found in the **examples/** directory. Typical usage is as follows:
```
#include "argparse.hpp"
#include <iostream>

using namespace argparse;

int main(int argc, char *argv[])
{
    // Create a parser
    ArgParser parser({}, false);

    // Add arguments
    parser.add_argument(Argument("print-output").help("Whether to print the output"));
    parser.add_argument(Argument("n-iter-max", true).help("Max no. iterations for the solver").value(10)); ///< Required
    parser.add_argument(Argument("abs-tol").help("Absolute residual norm tolerance for the solver").value(1e-6));
    parser.add_argument(Argument("log-level").help("Log level").value("info"));

    // Parse the arguments provided via the command-line
    parser.parse_args(argc, argv);

    // Print all stored arguments
    for (const auto &arg : parser.args())
    {
        std::cout << arg.str();
    }

    // Get the value of a specified integer argument
    int n_iter_max = parser.get_argument("n-iter-max")->value<int>();
    std::cout << n_iter_max << "\n";

    // Get the value of an unspecified string argument
    // The argument is stored only if the parser was created with store_all=true
    if (auto arg = parser.get_argument("solver-type"); arg != nullptr)
    {
        std::cout << parser.get_argument("solver-type")->value<std::string>() << "\n";
    }

    return 0;
}
```

In order to link to argparse, CMake can be used:
```
cmake_minimum_required(VERSION 3.16)
project(example)
find_package(argparse REQUIRED)
add_executable(example main.cpp)
target_link_libraries(example argparse::argparse)
```