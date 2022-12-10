#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <variant>
#include <vector>

struct Tree
{
    typedef std::map<std::string, Tree> Directory;
    std::variant<size_t, Directory> actual;
    Tree *parent;

    Tree() : actual(Directory()), parent(nullptr) {}
    Tree(Tree *parent) : actual(Directory()), parent(parent) {}
    Tree(Tree *parent, size_t file_size) : actual(file_size), parent(parent) {}

    ~Tree() {}

    size_t disk_size() const
    {
        if (std::holds_alternative<size_t>(actual))
            return std::get<size_t>(actual);
        auto dir = std::get<Directory>(actual);
        auto total = 0;
        for (const auto &[_, sub_tree] : dir)
            total += sub_tree.disk_size();
        return total;
    }

    void directory_apply(std::function<void(const Tree *)> func) const
    {
        if (std::holds_alternative<size_t>(actual))
            return;
        func(this);
        auto dir = std::get<Directory>(actual);
        for (const auto &[_, sub_tree] : dir)
            sub_tree.directory_apply(func);
    }
};

auto line_to_words(std::string &line) -> std::vector<std::string>
{
    auto words_view = std::string_view(line.c_str()) | std::ranges::views::split(' ');
    auto words = std::vector<std::string>();
    for (const auto &word : words_view)
        words.push_back(std::string(word.begin(), word.end()));
    return words;
}

auto main() -> int
{
    auto file = std::ifstream();
    file.open("07/input.txt");
    assert(file.is_open());

    auto root = Tree();
    auto *tree = &root;

    std::string line;
    while (std::getline(file, line))
    {
        auto words = line_to_words(line);
        assert(words[0] == "$");
        if (words[1] == "cd")
        {
            if (words[2] == "/")
                tree = &root;
            else if (words[2] == "..")
                tree = tree->parent;
            else
                tree = &std::get<Tree::Directory>(tree->actual)[words[2]];
            continue;
        }
        else if (words[1] == "ls")
        {
            for (auto oldpos = file.tellg(); std::getline(file, line); oldpos = file.tellg())
            {
                auto words = line_to_words(line);
                if (words[0] == "$")
                {
                    file.seekg(oldpos);
                    break;
                }
                if (words[0] == "dir")
                {
                    auto dir = Tree(tree);
                    std::get<Tree::Directory>(tree->actual)[words[1]] = dir;
                }
                else
                {
                    auto file = Tree(tree, std::stoul(words[0]));
                    std::get<Tree::Directory>(tree->actual)[words[1]] = file;
                }
            }
        }
    }

    auto total = 0;
    root.directory_apply(
        [&total](const auto tree)
        {
            if (auto size = tree->disk_size(); size < 100'000)
                total += size;
        });
    std::cout << "Part 1: " << total << std::endl;

    auto root_disk_size = root.disk_size();
    auto fs_size = 70'000'000;
    auto update_size = 30'000'000;
    auto unused_space = fs_size - root_disk_size;
    auto required_free_space = update_size - unused_space;

    auto valid_sizes = std::vector<size_t>();
    root.directory_apply(
        [&valid_sizes, required_free_space](const auto tree)
        {
            if (auto size = tree->disk_size(); size >= required_free_space)
                valid_sizes.push_back(size);
        });

    auto deleted_dir_size = std::ranges::min(valid_sizes);
    std::cout << "Part 2: " << deleted_dir_size << std::endl;

    return 0;
}
