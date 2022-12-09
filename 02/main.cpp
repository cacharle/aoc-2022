#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

enum class Shape
{
    Rock,
    Paper,
    Scissors,
};

auto round_score(std::pair<Shape, Shape> &round) -> int
{
    using enum Shape;
    int base_score;
    switch (round.second)
    {
    case Rock: base_score = 1; break;
    case Paper: base_score = 2; break;
    case Scissors: base_score = 3; break;
    }
    if (round.first == round.second)  // draw
        return base_score + 3;
    // win
    if ((round.second == Rock && round.first == Scissors) ||
        (round.second == Scissors && round.first == Paper) ||
        (round.second == Paper && round.first == Rock))
        return base_score + 6;
    // lose
    return base_score;
}

auto pick_shape(std::pair<Shape, Shape> &round) -> Shape
{
    using enum Shape;
    auto int_shape = static_cast<int>(round.first);
    switch (round.second)
    {
    case Rock: return static_cast<Shape>((((int_shape - 1) % 3) + 3) % 3);
    case Paper: return round.first;
    case Scissors: return static_cast<Shape>((((int_shape + 1) % 3) + 3) % 3);
    default: assert(false);
    }
}

auto main() -> int
{
    auto file = std::ifstream();
    file.open("02/input.txt");
    assert(file.is_open());
    auto rounds = std::vector<std::pair<Shape, Shape>>();
    std::string line;
    while (std::getline(file, line))
    {
        assert(line.size() == 3);
        Shape opponent_shape;
        switch (line[0])
        {
        case 'A': opponent_shape = Shape::Rock; break;
        case 'B': opponent_shape = Shape::Paper; break;
        case 'C': opponent_shape = Shape::Scissors; break;
        default: assert(false); break;
        }
        Shape my_shape;
        switch (line[2])
        {
        case 'X': my_shape = Shape::Rock; break;
        case 'Y': my_shape = Shape::Paper; break;
        case 'Z': my_shape = Shape::Scissors; break;
        default: assert(false); break;
        }
        rounds.push_back(std::pair(opponent_shape, my_shape));
    }

    auto scores = rounds | std::views::transform(round_score);
    auto total_score = std::accumulate(scores.begin(), scores.end(), 0);
    std::cout << "Part 1: " << total_score << std::endl;

    total_score = 0;
    auto picked_shapes = rounds | std::views::transform(pick_shape);
    auto rounds_iter = rounds.begin();
    auto picked_shapes_iter = picked_shapes.begin();
    for (; rounds_iter != rounds.end(); ++rounds_iter, ++picked_shapes_iter)
    {
        auto round = std::pair((*rounds_iter).first, *picked_shapes_iter);
        auto score = round_score(round);
        total_score += score;
    }
    std::cout << "Part 2: " << total_score << std::endl;
    return 0;
}
