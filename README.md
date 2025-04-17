# ARGPARSE

Lightweight parser for command-line arguments in modern C++

## Requirements
* C++ 23 (and above)

## Example usage

```
#include "argparse.hpp"
#include <iostream>

using namespace argparse;

int main(int argc, char *argv[])
{
    ArgParser parser;

    parser.add_argument(Argument("print-output").help("Whether to print the output").value(false));
    parser.add_argument(Argument("n-iter-max").help("Max no. iterations for the solver").value(10));
    parser.add_argument(Argument("abs-tol").help("Absolute residual norm tolerance for the solver").value(1e-6));
    parser.add_argument(Argument("log-level").help("Log level").value("info"));

    parser.parse_args(argc, argv);

    std::cout << parser.get_argument("print-output")->str();
    std::cout << parser.get_argument("n-iter-max")->str();
    std::cout << parser.get_argument("abs-tol")->str();
    std::cout << parser.get_argument("log-level")->str();

    int n_iter_max = parser.get_argument("n-iter-max")->value<int>();

    return 0;
}
```