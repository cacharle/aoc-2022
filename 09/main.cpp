#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <variant>
#include <vector>

struct Position
{
    int x;
    int y;

    bool operator==(const Position &other) const { return x == other.x && y == other.y; }
};

auto main() -> int
{
    auto file = std::ifstream();
    file.open("09/input.txt");
    assert(file.is_open());

    auto visited = std::vector<Position>();
    auto head = Position{0, 0};
    auto tail = Position{0, 0};

    std::string line;
    while (std::getline(file, line))
    {
        // assert(line.size() == 3);
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
            head.x += mod_x;
            head.y += mod_y;
            if (abs(head.x - tail.x) >= 2)
            {
                tail.x = head.x - mod_x;
                tail.y = head.y;
            }
            else if (abs(head.y - tail.y) >= 2)
            {
                tail.y = head.y - mod_y;
                tail.x = head.x;
            }
            if (std::ranges::find(visited, tail) == visited.end())
                visited.push_back(tail);
            // for (int i = 0; i < 5; i++)
            // {
            //     for (int j = 0; j < 6; j++)
            //     {
            //         auto p = Position{j, i};
            //         if (p == tail)
            //             std::cout << "T";
            //         else if (p == head)
            //             std::cout << "H";
            //         else if (std::ranges::find(visited, p) == visited.end())
            //             std::cout << ".";
            //         else
            //             std::cout << "#";
            //     }
            //     std::cout << std::endl;
            // }
            //     std::cout << std::endl;
        }
    }
    std::cout << "Part 1: " << visited.size() << std::endl;

    return 0;
}
