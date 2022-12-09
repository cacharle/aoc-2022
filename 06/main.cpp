#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

auto main() -> int
{
    auto file = std::ifstream();
    file.open("06/input.txt");
    assert(file.is_open());
    std::string input;
    std::getline(file, input);

    for (size_t i = 0; i + 3 < input.size(); i++)
    {
        auto start_of_packet = input.substr(i, 4);
        std::ranges::sort(start_of_packet);
        auto ret = std::ranges::unique(start_of_packet);
        start_of_packet.erase(ret.begin(), ret.end());
        if (start_of_packet.size() == 4)
        {
            std::cout << "Part 1: " << i + 4 << std::endl;
            break;
        }
    }

    for (size_t i = 0; i + 12 < input.size(); i++)
    {
        auto start_of_packet = input.substr(i, 14);
        std::ranges::sort(start_of_packet);
        auto ret = std::ranges::unique(start_of_packet);
        start_of_packet.erase(ret.begin(), ret.end());
        if (start_of_packet.size() == 14)
        {
            std::cout << "Part 2: " << i + 14 << std::endl;
            break;
        }
    }
    return 0;
}
