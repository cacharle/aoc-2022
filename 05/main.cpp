#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

struct Move
{
    size_t quantity;
    size_t from;
    size_t to;
};

auto main() -> int
{
    auto file = std::ifstream();
    file.open("05/input.txt");
    assert(file.is_open());
    auto origin_stacks = std::vector<std::vector<char>>();
    auto moves = std::vector<Move>();
    // read number stacks
    std::string line;
    while (std::getline(file, line))
    {
        if (line.starts_with(" 1"))
        {
            while (line.ends_with(" "))
                line = line.substr(0, line.size() - 1);
            size_t pos = line.rfind(" ");
            assert(pos != std::string::npos);
            origin_stacks.resize(std::stoi(line.substr(pos + 1)));
            break;
        }
    }
    file.seekg(0);
    // read stacks
    while (std::getline(file, line))
    {
        if (line.starts_with(" 1"))
            continue;
        if (line == "")
            break;
        for (size_t i = 1; i < line.size(); i += 4)
        {
            if (std::isspace(line[i]))
                continue;
            assert(std::isupper(line[i]));
            origin_stacks[(i - 1) / 4].push_back(line[i]);
        }
    }
    for (auto &stack : origin_stacks)
        std::ranges::reverse(stack);
    // read moves
    auto move_regex = std::regex("^move ([0-9]+) from ([0-9]+) to ([0-9]+)$");
    while (std::getline(file, line))
    {
        auto matches = std::smatch();
        assert(std::regex_match(line, matches, move_regex));
        moves.push_back({
            std::stoul(matches[1]),
            std::stoul(matches[2]) - 1,
            std::stoul(matches[3]) - 1,
        });
    }

    auto stacks = std::vector(origin_stacks);
    for (auto move : moves)
    {
        for (size_t q = move.quantity; q > 0; q--)
        {
            stacks[move.to].push_back(stacks[move.from].back());
            stacks[move.from].pop_back();
        }
    }
    std::cout << "Part 1: ";
    for (auto stack : stacks)
        std::cout << stack.back();
    std::cout << std::endl;

    stacks = std::vector(origin_stacks);
    for (auto move : moves)
    {
        auto acc = std::vector<char>();
        for (size_t q = move.quantity; q > 0; q--)
        {
            acc.push_back(stacks[move.from].back());
            stacks[move.from].pop_back();
        }
        std::ranges::reverse(acc);
        for (auto x : acc)
            stacks[move.to].push_back(x);
    }
    std::cout << "Part 2: ";
    for (auto stack : stacks)
        std::cout << stack.back();
    std::cout << std::endl;

    file.close();
    return 0;
}
