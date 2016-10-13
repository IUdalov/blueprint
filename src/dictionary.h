#pragma once

#include <string>
#include <map>
#include <set>
#include <ostream>

class Dictionary {
public:
    struct Cursor {
        Cursor() : valid_(false) {
        }

        operator bool() const noexcept {
            return valid_;
        }

        const std::string& operator*() {
            return *p_;
        }

        void operator=(const std::set<std::string>::iterator& p) {
            valid_ = true;
            p_ = p;
        }

        std::set<std::string>::iterator p_;
        bool valid_;
    };

public:
    Dictionary() = default;
    Dictionary(Dictionary&&) = default;
    virtual ~Dictionary() = default;

    static Dictionary read(const std::string& path);
    void add(const std::string& word);

    bool get(const std::string& pattern, Cursor& c) const;
    size_t size() const;
    void log(std::ostream&) const;

private:
    Dictionary(const Dictionary&) = delete;
    std::map<size_t, std::set<std::string>> data_;

private:
    bool match(const std::string& pattern, const std::string& word) const;
    void shrink();
};

//Dictionary readDictionary(const std::string& path);