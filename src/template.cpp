#include "template.h"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace {

void sanitize(std::string& line) {
    for(auto& it : line) {
        if (it != ' ') {
            it = '?';
        }
    }
}

} // namespace

Template Template::read(const std::string& path) {
    std::ifstream in;
    in.open(path);
    if (in.bad()) {
        throw std::runtime_error("Unable to read " + path);
    }

    size_t maxLen = 0;
    std::string line;
    Template t;
    while(std::getline(in, line)) {
        sanitize(line);
        t.data_.push_back(line);
        if (line.length() > maxLen) {
            maxLen = line.length();
        }
    }

    if (0 == maxLen) {
        throw std::runtime_error("Template file is empty!");
    }

    for(auto& it : t.data_) {
        it.resize(maxLen, ' ');
    }

    // add lines;
    for(size_t i = 0; i < t.data_.size(); i++) {
        size_t beg = 0;
        bool onWord = false;
        for(size_t j = 0; j < t.data_[i].length(); j++) {
            char sym = t.data_[i][j];
            switch(sym) {
                case ' ':
                    if (onWord) {
                        if (beg != j - 1) {
                            Placeholder p(Point(i, beg), Point(i, j - 1));
                            t.empty_.push_back(p);
                        }
                        onWord = false;

                    }
                    break;
                case '?':
                    if (!onWord) {
                        beg = j;
                        onWord = true;
                    }
                    break;
                default:
                    throw std::runtime_error(std::string("Invalid symbol") + sym);
            }
        }
        if (onWord && beg != t.data_[i].length() - 1) {
            Placeholder p(Point(i, beg), Point(i, t.data_[i].length() - 1));
            t.empty_.push_back(p);
        }
    }

    // add columns
    for(size_t j = 0; j < t.data_[0].length(); j++) {
        bool onWord = false;
        size_t beg = 0;
        for(size_t i = 0; i < t.data_.size(); i++) {
            char sym = t.data_[i][j];
            switch(sym) {
                case ' ':
                    if (onWord) {
                        if (beg != i - 1) {
                            Placeholder p(Point(beg, j), Point(i - 1, j));
                            t.empty_.push_back(p);
                        }
                        onWord = false;

                    }
                    break;
                case '?':
                    if (!onWord) {
                        beg = i;
                        onWord = true;
                    }
                    break;
            }
        }

        if (onWord && beg != t.data_.size() - 1) {
            Placeholder p(Point(beg, j), Point(t.data_.size() - 1, j));
            t.empty_.push_back(p);
        }
    }

    std::sort(t.empty_.begin(), t.empty_.end(), [](const Placeholder& f, const Placeholder& s) {
        return (f.begin.first + f.begin.second) < (s.begin.first + s.begin.second);
    });

    std::reverse(t.empty_.begin(), t.empty_.end());

    t.shrink();

    return t;
}

Template::operator bool() const noexcept {
    return empty_.empty();
}

std::string Template::next() {
    size_t s = empty_.size();
    Placeholder p = empty_[s - 1];
    std::string res;

    if (p.isRaw) {
        size_t firstCol = p.begin.second;
        size_t lastCol = p.end.second;
        size_t raw = p.begin.first;
        for(size_t j = firstCol; j <= lastCol; j++) {
            res.push_back(data_[raw][j]);
        }
    } else {
        size_t firstRow = p.begin.first;
        size_t lastRow = p.end.first;
        size_t col = p.begin.second;
        for(size_t i = firstRow; i <= lastRow; i++) {
            res.push_back(data_[i][col]);
        }
    }
    return res;
}

void Template::place(const std::string& word) {
    size_t s = empty_.size();
    Placeholder p = empty_[s - 1];
    p.snapshot.clear();
    if (p.isRaw) {
        size_t firstCol = p.begin.second;
        size_t lastCol = p.end.second;
        size_t raw = p.begin.first;
        for(size_t j = firstCol; j <= lastCol; j++) {
            p.snapshot.push_back(data_[raw][j]);
            data_[raw][j] = word[j - firstCol];
        }
    } else {
        size_t firstRow = p.begin.first;
        size_t lastRow = p.end.first;
        size_t col = p.begin.second;
        for(size_t i = firstRow; i <= lastRow; i++) {
            p.snapshot.push_back(data_[i][col]);
            data_[i][col] =  word[i - firstRow];
        }
    }

    filled_.push_back(p);
    empty_.pop_back();
}

void Template::rollback() {
    size_t s = filled_.size();
    Placeholder p = filled_[s - 1];

    if (p.isRaw) {
        size_t firstCol = p.begin.second;
        size_t lastCol = p.end.second;
        size_t raw = p.begin.first;
        for(size_t j = firstCol; j <= lastCol; j++) {
            data_[raw][j] = p.snapshot[j - firstCol];
        }
    } else {
        size_t firstRow = p.begin.first;
        size_t lastRow = p.end.first;
        size_t col = p.begin.second;
        for(size_t i = firstRow; i <= lastRow; i++) {
            data_[i][col] =  p.snapshot[i - firstRow];
        }
    }

    p.snapshot.clear();
    filled_.pop_back();
    empty_.push_back(p);
}

void Template::log(const std::string& title, std::ostream& out) const {
    out << "***" << title << "***" << std::endl;
    out << "Words: " << empty_.size() << std::endl;
    for(const auto& it : data_) {
        out << it << std::endl;
    }
}

void Template::shrink() {
    data_.shrink_to_fit();
    for(auto& it : data_) {
        it.shrink_to_fit();
    }

    filled_.reserve(empty_.size());
}
