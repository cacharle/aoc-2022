#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

struct Position
{
    int y;
    int x;

    bool operator==(const Position &other) const { return x == other.x && y == other.y; }
};

auto rope_step(std::vector<Position> &rope,
               int head_mod_x,
               int head_mod_y,
               std::vector<Position> &visited)
{
    rope.front().x += head_mod_x;
    rope.front().y += head_mod_y;
    for (size_t i = 1; i < rope.size(); i++)
    {
        auto next_knot = rope[i - 1];
        auto mod_x = next_knot.x < rope[i].x ? 1 : -1;
        auto mod_y = next_knot.y < rope[i].y ? 1 : -1;
        auto dist_x = abs(next_knot.x - rope[i].x);
        auto dist_y = abs(next_knot.y - rope[i].y);
        if (dist_x >= 2 && dist_y >= 2)  // 2 cell away in diagonal in both direction
        {
            rope[i].x = next_knot.x + mod_x;
            rope[i].y = next_knot.y + mod_y;
        }
        else if (dist_x >= 2)  // 2 cell away in x (maybe 1 cell diagonal)
        {
            rope[i].x = next_knot.x + mod_x;
            rope[i].y = next_knot.y;
        }
        else if (dist_y >= 2)  // 2 cell away in y (maybe 1 cell diagonal)
        {
            rope[i].y = next_knot.y + mod_y;
            rope[i].x = next_knot.x;
        }
    }
    if (std::ranges::find(visited, rope.back()) == visited.end())
        visited.push_back(rope.back());
}

auto main() -> int
{
    auto file = std::ifstream();
    file.open("09/input.txt");
    assert(file.is_open());

    auto visited1 = std::vector<Position>();
    auto visited2 = std::vector<Position>();
    auto rope1 = std::vector<Position>(2, Position{0, 0});
    auto rope2 = std::vector<Position>(10, Position{0, 0});

    std::string line;
    while (std::getline(file, line))
    {
        auto mod_x = 0;
        auto mod_y = 0;
        switch (line[0])
        {
        case 'U': mod_y = 1; break;
        case 'D': mod_y = -1; break;
        case 'R': mod_x = 1; break;
        case 'L': mod_x = -1; break;
        default: assert(false); break;
        }
        auto count = std::stoi(line.substr(2));
        for (int i = 0; i < count; i++)
        {
            rope_step(rope1, mod_x, mod_y, visited1);
            rope_step(rope2, mod_x, mod_y, visited2);
        }
    }
    std::cout << "Part 1: " << visited1.size() << std::endl;
    std::cout << "Part 2: " << visited2.size() << std::endl;

    return 0;
}
