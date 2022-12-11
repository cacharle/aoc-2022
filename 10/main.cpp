#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

auto main() -> int
{
    auto file = std::ifstream();
    file.open("10/input.txt");
    assert(file.is_open());

    long x = 1;
    auto cycles = std::vector<int>();

    std::string line;
    while (std::getline(file, line))
    {
        if (line == "noop")
        {
            cycles.push_back(x);
            continue;
        }
        assert(line.substr(0, 5) == "addx ");
        auto add = std::stoi(line.substr(5));
        cycles.push_back(x);
        cycles.push_back(x);
        x += add;
    }
    // when c++23 is here, use std::view::stride instead of the caveman method
    auto total = 0;
    for (size_t i = 19; i < cycles.size(); i += 40)
        total += (i + 1) * cycles[i];
    std::cout << "Part 1: " << total << std::endl;

    auto crt = std::string(40 * 6, '.');
    for (int i = 0; i < static_cast<int>(cycles.size()); i++)
    {
        if (i % 40 <= cycles[i] + 1 && i % 40 >= cycles[i] - 1)
            crt[i] = '#';
    }
    std::cout << "Part 2" << std::endl;
    for (size_t i = 0; i < crt.size(); i += 40)
        std::cout << crt.substr(i, 40) << std::endl;

    return 0;
}
