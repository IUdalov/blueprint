#include "dictionary.h"

#include <iostream>
#include <fstream>

struct Node {
    char sym_;

    Node(char sym)
        : sym_(sym) {
    }

    bool operator<(const Node& other) {
        return  sym_ < other.sym_;
    }
};

void Dictionary::add(const std::string& word) {
    data_[word.size()].insert(word);
}

bool Dictionary::get(const std::string& pattern, Cursor& c) const {
    size_t len = pattern.length();
    auto chunk = data_.find(len);
    if (chunk == data_.end()) {
        return false;
    }
    if (!c) {
        c = chunk->second.begin();
    } else {
        c.p_++;
    }

    for(; c.p_ != chunk->second.end(); c.p_++) {
        if (match(pattern, *c)) {
            return true;
        }
    }
    return false;
}

size_t Dictionary::size() const{
    size_t res = 0;
    for(const auto& it : data_) {
        res += it.second.size();
    }
    return res;
}

void Dictionary::log(std::ostream& out) const {
    out << size() << " words are present." << std::endl;
    for(const auto& it : data_) {
        out << it.second.size() << " words with length " << it.first << std::endl;
    }
}

void Dictionary::shrink() {
    // TODO: change set to sorted vector
}

bool Dictionary::match(const std::string& pattern, const std::string& word) const {
    for(size_t i = 0; i < word.size(); i++) {
        if (pattern[i] != '?' && pattern[i] != word[i]) {
            return false;
        }
    }
    return true;
}

Dictionary Dictionary::read(const std::string& path) {
    std::ifstream in;
    in.open(path);
    if (!in.good()) {
        throw std::runtime_error("Unable to read file " + path);
    }

    Dictionary d;
    std::string word;
    while(std::getline(in, word))
        if (!word.empty())
            d.add(word);

    d.shrink();

    return d;
}
