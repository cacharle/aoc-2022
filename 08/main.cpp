#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

auto main() -> int
{
    auto file = std::ifstream();
    file.open("08/input.txt");
    assert(file.is_open());

    auto trees = std::vector<std::vector<int>>();

    std::string line;
    while (std::getline(file, line))
    {
        trees.push_back(std::vector<int>());
        for (auto c : line)
            trees.back().push_back(c - '0');
    }
    // yes, yes, everything bellow is very bad code, repeat everywhere, don't judge yourself too
    // hard, it was annoying to generalize and it's late

    auto visited = std::vector<std::pair<int, int>>();
    int visible_count = 0;
    for (size_t i = 0; i < trees.size(); i++)
    {
        int max_height = -1;
        for (size_t j = 0; j < trees[i].size(); j++)
        {
            auto height = trees[i][j];
            if (height <= max_height)
                continue;
            max_height = height;
            auto pos = std::pair<int, int>{i, j};
            if (std::ranges::find(visited, pos) != visited.end())
                continue;
            visited.push_back(pos);
            visible_count++;
        }
        max_height = -1;
        for (int j = trees[i].size() - 1; j >= 0; j--)
        {
            auto height = trees[i][j];
            if (height <= max_height)
                continue;
            max_height = height;
            auto pos = std::pair<int, int>{i, j};
            if (std::ranges::find(visited, pos) != visited.end())
                continue;
            visited.push_back(pos);
            visible_count++;
        }
    }
    for (size_t j = 0; j < trees[0].size(); j++)
    {
        int max_height = -1;
        for (size_t i = 0; i < trees.size(); i++)
        {
            auto height = trees[i][j];
            if (height <= max_height)
                continue;
            max_height = height;
            auto pos = std::pair<int, int>{i, j};
            if (std::ranges::find(visited, pos) != visited.end())
                continue;
            visited.push_back(pos);
            visible_count++;
        }
        max_height = -1;
        for (int i = trees.size() - 1; i >= 0; i--)
        {
            auto height = trees[i][j];
            if (height <= max_height)
                continue;
            max_height = height;
            auto pos = std::pair<int, int>{i, j};
            if (std::ranges::find(visited, pos) != visited.end())
                continue;
            visited.push_back(pos);
            visible_count++;
        }
    }
    std::cout << "Part 1: " << visible_count << std::endl;

    auto scenic_scores = std::vector<int>();
    for (size_t i = 0; i < trees.size(); i++)
    {
        for (size_t j = 0; j < trees[i].size(); j++)
        {
            int viewing_distances[] = {0, 0, 0, 0};
            for (size_t k = i + 1; k < trees.size(); k++)
            {
                viewing_distances[0]++;
                if (trees[k][j] >= trees[i][j])
                    break;
            }
            for (size_t k = j + 1; k < trees[i].size(); k++)
            {
                viewing_distances[1]++;
                if (trees[i][k] >= trees[i][j])
                    break;
            }
            for (int k = i - 1; k >= 0; k--)
            {
                viewing_distances[2]++;
                if (trees[k][j] >= trees[i][j])
                    break;
            }
            for (int k = j - 1; k >= 0; k--)
            {
                viewing_distances[3]++;
                if (trees[i][k] >= trees[i][j])
                    break;
            }
            scenic_scores.push_back(std::accumulate(
                viewing_distances, viewing_distances + 4, 1, std::multiplies<int>()));
        }
    }
    auto max_score = std::ranges::max_element(scenic_scores);
    std::cout << "Part 2: " << *max_score << std::endl;

    return 0;
}
