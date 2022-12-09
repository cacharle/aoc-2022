#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <regex>
#include <vector>

auto
main() -> int
{
    auto file = std::ifstream();
    file.open("04/input.txt");
    assert(file.is_open());
    auto assignment_pairs = std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>();
    auto regex = std::regex("^([0-9]+)-([0-9]+),([0-9]+)-([0-9]+)$");
    std::string line;
    while (std::getline(file, line))
    {
        auto matches = std::smatch();
        assert(std::regex_match(line, matches, regex));
        assignment_pairs.push_back({
            {std::stoi(matches[1]), std::stoi(matches[2])},
            {std::stoi(matches[3]), std::stoi(matches[4])},
        });
    }

    auto fully_overlapping_pairs_view =
        assignment_pairs |
        std::views::filter(
            [](auto elf_pair)
            {
                auto first_elf_sections =
                    std::views::iota(elf_pair.first.first, elf_pair.first.second + 1);
                auto second_elf_sections =
                    std::views::iota(elf_pair.second.first, elf_pair.second.second + 1);
                auto [f1, l1] = std::ranges::search(first_elf_sections, second_elf_sections);
                auto [f2, l2] = std::ranges::search(second_elf_sections, first_elf_sections);
                return f1 != l1 || f2 != l2;
            });
    auto fully_overlapping_pairs =
        std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>();
    std::ranges::copy(fully_overlapping_pairs_view, std::back_inserter(fully_overlapping_pairs));
    std::cout << "Part 1: " << fully_overlapping_pairs.size() << std::endl;

    auto overlapping_pairs_view =
        assignment_pairs |
        std::views::filter(
            [](auto elf_pair)
            {
                auto first_elf_sections =
                    std::views::iota(elf_pair.first.first, elf_pair.first.second + 1);
                auto second_elf_sections =
                    std::views::iota(elf_pair.second.first, elf_pair.second.second + 1);
                auto common = std::vector<int>();
                std::ranges::set_intersection(
                    first_elf_sections, second_elf_sections, std::back_inserter(common));
                return common.size() > 0;
            });
    auto overlapping_pairs = std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>();
    std::ranges::copy(overlapping_pairs_view, std::back_inserter(overlapping_pairs));
    std::cout << "Part 2: " << overlapping_pairs.size() << std::endl;

    file.close();
    return 0;
}
