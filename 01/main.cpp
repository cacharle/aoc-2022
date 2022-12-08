#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

auto
main() -> int
{
    auto file = std::ifstream();
    file.open("input.txt");
    assert(file.is_open());
    auto calories = std::vector<std::vector<int>>();
    calories.push_back(std::vector<int>());
    std::string line;
    while (std::getline(file, line))
    {
        if (line == "")
        {
            calories.push_back(std::vector<int>());
            continue;
        }
        calories.back().push_back(std::stoi(line));
    }

    auto sums = calories | std::views::transform(
                               [](auto elf_calories) {
                                   return std::accumulate(
                                       elf_calories.begin(), elf_calories.end(), 0);
                               });
    auto sums_vec = std::vector<int>(sums.begin(), sums.end());
    std::ranges::sort(sums_vec, std::ranges::greater());
    std::cout << "Part 1: " << sums_vec.front() << std::endl;
    std::cout << "Part 2: " << sums_vec[0] + sums_vec[1] + sums_vec[2] << std::endl;

    file.close();
    return 0;
}
