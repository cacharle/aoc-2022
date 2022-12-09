#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

auto item_score(char c)
{
    assert(std::isalpha(c));
    if (std::islower(c))
        return c - 'a' + 1;
    else
        return c - 'A' + 27;
}

auto main() -> int
{
    auto file = std::ifstream();
    file.open("03/input.txt");
    assert(file.is_open());
    auto rucksacks = std::vector<std::string>();
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
                            std::ranges::set_intersection(part1, part2, std::back_inserter(common));
                            auto ret = std::ranges::unique(common);
                            common.erase(ret.begin(), ret.end());
                            assert(common.size() == 1);
                            return item_score(common.front());
                        });
    auto priorities_sum =
        std::accumulate(common_items_priorities.begin(), common_items_priorities.end(), 0);
    std::cout << "Part 1: " << priorities_sum << std::endl;

    // NOTE: chunks not supported yet (same with zip in previous problem, common c++,
    // u gotta do btr m8) auto chunks = rucksacks | std::views::chunk(3);
    // std::ranges::for_each(chunks, [](auto c) { std::cout << c << '\n'; });

    size_t total = 0;
    for (size_t i = 0; i + 2 < rucksacks.size(); i += 3)
    {
        std::ranges::sort(rucksacks[i]);
        std::ranges::sort(rucksacks[i + 1]);
        std::ranges::sort(rucksacks[i + 2]);
        auto common = std::string();
        std::ranges::set_intersection(rucksacks[i], rucksacks[i + 1], std::back_inserter(common));
        auto common2 = std::string();
        std::ranges::set_intersection(common, rucksacks[i + 2], std::back_inserter(common2));
        auto ret = std::ranges::unique(common2);
        common2.erase(ret.begin(), ret.end());
        assert(common2.size() == 1);
        total += item_score(common2.front());
    }
    std::cout << "Part 2: " << total << std::endl;

    file.close();
    return 0;
}
