#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

struct Monkey
{
    size_t id;
    std::vector<size_t> items;

    struct Operation
    {
        enum class Operator
        {
            Add,
            Mul,
        };
        Operator operator_;
        std::optional<size_t> operand1;
        std::optional<size_t> operand2;

        Operation() : operator_(), operand1(), operand2() {}
        Operation(std::string operator_str, std::string operand1_str, std::string operand2_str)
        {
            operator_ = operator_str == "+" ? Operator::Add : Operator::Mul;
            operand1 = parse_operand(operand1_str);
            operand2 = parse_operand(operand2_str);
        }

        static std::optional<size_t> parse_operand(std::string &operand_str)
        {
            return (operand_str == "old") ? std::nullopt
                                          : std::optional(size_t(std::stoi(operand_str)));
        }
    };
    Operation operation;

    size_t test_div;
    size_t test_true_throw_id;
    size_t test_false_throw_id;
};

auto run(std::vector<Monkey> monkeys, size_t rounds, std::function<size_t(size_t)> result_modifier)
    -> size_t
{
    auto inspect_counts = std::vector<size_t>(monkeys.size(), 0);
    for (size_t round = 0; round < rounds; round++)
    {
        for (auto &monkey : monkeys)
        {
            for (const auto &item : monkey.items)
            {
                inspect_counts[monkey.id]++;
                auto o1 = monkey.operation.operand1.value_or(item);
                auto o2 = monkey.operation.operand2.value_or(item);
                size_t result;
                switch (monkey.operation.operator_)
                {
                case Monkey::Operation::Operator::Add: result = o1 + o2; break;
                case Monkey::Operation::Operator::Mul: result = o1 * o2; break;
                }
                result = result_modifier(result);
                if (result % monkey.test_div == 0)
                    monkeys[monkey.test_true_throw_id].items.push_back(result);
                else
                    monkeys[monkey.test_false_throw_id].items.push_back(result);
            }
            monkey.items.clear();
        }
    }
    std::ranges::sort(inspect_counts, std::ranges::greater());
    return inspect_counts[0] * inspect_counts[1];
}

auto main() -> int
{
    auto file = std::ifstream();
    file.open("11/input.txt");
    assert(file.is_open());

    auto id_regex = std::regex("^Monkey ([0-9]+):$");
    auto starting_items_regex = std::regex("^  Starting items: ((([0-9]+), )*([0-9]+))$");
    auto operation_regex = std::regex("^  Operation: new = (old|[0-9]+) (\\+|\\*) (old|[0-9]+)$");
    auto test_div_regex = std::regex("^  Test: divisible by ([0-9]+)$");
    auto test_true_throw_id_regex = std::regex("^    If true: throw to monkey ([0-9]+)$");
    auto test_false_throw_id_regex = std::regex("^    If false: throw to monkey ([0-9]+)$");

    auto monkeys = std::vector<Monkey>();

    std::string line;
    while (std::getline(file, line))
    {
        if (line == "")
            continue;
        auto monkey = Monkey{};
        auto id_matches = std::smatch();
        assert(std::regex_match(line, id_matches, id_regex));
        monkey.id = std::stoi(id_matches[1]);

        assert(std::getline(file, line));
        auto starting_items_matches = std::smatch();
        assert(std::regex_match(line, starting_items_matches, starting_items_regex));

        auto items_it =
            starting_items_matches[1].str() | std::views::split(',') |
            std::views::transform(
                [](auto string_it)
                { return size_t(std::stoi(std::string(string_it.begin(), string_it.end()))); });
        monkey.items = std::vector(items_it.begin(), items_it.end());

        assert(std::getline(file, line));
        auto operation_matches = std::smatch();
        assert(std::regex_match(line, operation_matches, operation_regex));
        monkey.operation =
            Monkey::Operation(operation_matches[2], operation_matches[1], operation_matches[3]);

        assert(std::getline(file, line));
        auto test_div_matches = std::smatch();
        assert(std::regex_match(line, test_div_matches, test_div_regex));
        monkey.test_div = std::stoi(test_div_matches[1]);

        assert(std::getline(file, line));
        auto test_true_throw_id_matches = std::smatch();
        assert(std::regex_match(line, test_true_throw_id_matches, test_true_throw_id_regex));
        monkey.test_true_throw_id = std::stoi(test_true_throw_id_matches[1]);

        assert(std::getline(file, line));
        auto test_false_throw_id_matches = std::smatch();
        assert(std::regex_match(line, test_false_throw_id_matches, test_false_throw_id_regex));
        monkey.test_false_throw_id = std::stoi(test_false_throw_id_matches[1]);

        monkeys.push_back(monkey);
    }

    auto common_div = std::accumulate(
        monkeys.begin(), monkeys.end(), 1, [](auto acc, auto m) { return acc * m.test_div; });
    std::cout << "Part 1: " << run(monkeys, 20, [](auto result) { return result / 3; })
              << std::endl;
    std::cout << "Part 2: "
              << run(monkeys, 10'000, [common_div](auto result) { return result % common_div; })
              << std::endl;

    return 0;
}
