#include "../include/bst_tree.h"
#include <iostream>
#include <vector>

struct Song {
    std::string title;
    std::string artist;
    std::string album;
    unsigned int year;

    bool operator<(const Song& other) const {
        return title < other.title; // сортировка по названию
    }

    friend std::ostream& operator<<(std::ostream& os, const Song& s) {
        os << s.title << " - " << s.artist << " (" << s.year << ")";
        return os;
    }
};

int main() {
    using Tree = BinarySearchTree<Song, std::less<Song>, std::allocator<Song>, InOrderTag>;
    Tree tree;

    // "Заглушка" вместо чтения mp3
    std::vector<Song> songs = {
        {"Moonlight Sonata", "Beethoven", "Classics", 1801},
        {"Bohemian Rhapsody", "Queen", "A Night at the Opera", 1975},
        {"Imagine", "John Lennon", "Imagine", 1971},
        {"Billie Jean", "Michael Jackson", "Thriller", 1982},
        {"Clocks", "Coldplay", "A Rush of Blood to the Head", 2002}
    };

    for (const auto& s : songs)
        tree.insert(s);

    std::cout << "Songs sorted by title:\n";
    for (const auto& s : tree)
        std::cout << s << '\n';

    return 0;
}
