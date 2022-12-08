#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

auto
main() -> int
{
    auto file = std::ifstream();
    file.open("input.txt");
    assert(file.is_open());
    auto        rucksacks = std::vector<std::string>();
    std::string line;
    while (std::getline(file, line))
    {
        assert(line.size() % 2 == 0);
        rucksacks.push_back(line);
    }

    auto common_items_priorities =
        rucksacks | std::views::transform(
                        [](auto sacks)
                        {
                            auto mid = sacks.size() / 2;
                            auto part1 = sacks.substr(0, mid);
                            auto part2 = sacks.substr(mid);
                            std::ranges::sort(part1);
                            std::ranges::sort(part2);
                            auto common = std::string();
                            std::ranges::set_intersection(
                                part1, part2, std::back_inserter(common));
                            auto ret = std::ranges::unique(common);
                            common.erase(ret.begin(), ret.end());
                            assert(common.size() == 1);
                            auto common_item = common.front();
                            assert(std::isalpha(common_item));
                            if (std::islower(common_item))
                                return common_item - 'a' + 1;
                            else
                                return common_item - 'A' + 27;
                        });

    auto priorities_sum = std::accumulate(
        common_items_priorities.begin(), common_items_priorities.end(), 0);
    std::cout << "Part 1: " << priorities_sum << std::endl;

    file.close();
    return 0;
}
