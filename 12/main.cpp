#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

struct Position
{
    int y;
    int x;

    bool operator==(const Position &other) const { return x == other.x && y == other.y; }

    Position(const Position& other) : y(other.y), x(other.x) {}
    Position& operator=(const Position& other) { y = other.y; x = other.x; return *this; }
    Position(int y_, int x_) : y(y_), x(x_) {}
    ~Position() {}
};

// from https://stackoverflow.com/questions/17016175
template <> struct std::hash<Position>
{
    std::size_t operator()(const Position &p) const
    {
        std::size_t hx = std::hash<int>()(p.x);
        std::size_t hy = std::hash<int>()(p.y);
        return hx ^ (hy << 1);
    }
};

auto explore(const std::vector<std::vector<char>> &height_map,
             const std::unordered_set<Position> &visited_parent,
             const Position current,
             std::vector<Position> &output) -> void
{
    // static int c = 0;
    // c++;

    // if (c > 2)
    //     return;
    if (height_map[current.y][current.x] == 'E')
    {
        // std::cout << "WINNNNNNNNNNNN" << std::endl;
        output.push_back(current);
        return;
    }
    auto visited = visited_parent;
    visited.insert(current);
    auto neighbours = std::vector<Position>();
    if (current.y > 0)
        neighbours.push_back(Position{current.y - 1, current.x});
    if (current.x > 0)
        neighbours.push_back(Position{current.y, current.x - 1});
    if (static_cast<size_t>(current.y) < height_map.size() - 1)
        neighbours.push_back(Position{current.y + 1, current.x});
    if (static_cast<size_t>(current.x) < height_map[0].size() - 1)
        neighbours.push_back(Position{current.y, current.x + 1});
    std::erase_if(neighbours,
                  [&](auto p)
                  {
                      auto current_height = height_map[current.y][current.x];
                      auto height = height_map[p.y][p.x];
                      // std::cout << p.y << "," << p.x << ": " << height << std::endl;
                      if (visited.find(p) != visited.end())
                          return true;
                      if (current_height == 'S')
                          return false;
                      if (height == 'E')
                        height = 'z';
                      if (height == 'S')
                          return true;
                      return height > current_height + 1;
                  });
    if (neighbours.empty())
        return;
    auto neighbours_path = std::vector<std::vector<Position>>();
    for (const auto& n : neighbours)
    {
        auto path = std::vector<Position>();
        explore(height_map, visited, n, path);
        // std::vector<Position> p(path);
        // p.push_back(current);
        // if (path.size() == 1)
        //     std::cout << height_map[path.back().y][path.back().x] << std::endl;
        neighbours_path.push_back(std::vector(path));
        neighbours_path.back().push_back(current);
    }
    std::erase_if(neighbours_path,
                  [&height_map](auto& path)
                  { return path.empty() || height_map[path.front().y][path.front().x] != 'E'; });
    if (neighbours_path.empty())
        return;
    // std::cout << neighbours_path.size() << std::endl;
    auto result = std::ranges::min_element(
        neighbours_path, [](const auto& path1, const auto& path2) {
            return path1.size() < path2.size();
        });
    std::ranges::copy(*result, std::back_inserter(output));
}

auto main() -> int
{
    auto file = std::ifstream();
    file.open("12/input.txt");
    assert(file.is_open());

    auto height_map = std::vector<std::vector<char>>();
    auto starting_pos = Position(0, 0);
    auto ending_pos = Position(0, 0);

    std::string line;
    while (std::getline(file, line))
    {
        height_map.push_back(std::vector<char>());
        for (const auto c : line)
        {
            height_map.back().push_back(c);
            switch (c)
            {
            case 'S':
                starting_pos = Position{static_cast<int>(height_map.size()) - 1,
                                        static_cast<int>(height_map.back().size()) - 1};
                break;
            case 'E':
                ending_pos = Position{static_cast<int>(height_map.size()) - 1,
                                      static_cast<int>(height_map.back().size()) - 1};
                break;
            }
        }
    }

    auto visited = std::unordered_set<Position>();
    auto path = std::vector<Position>();
    explore(height_map, visited, starting_pos, path);
    for (const auto& pos : path)
        std::cout << pos.y << "," << pos.x << std::endl;
    std::cout << path.size() - 1<< std::endl;

    return 0;
}
