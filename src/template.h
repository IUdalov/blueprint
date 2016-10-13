#pragma once

#include <string>
#include <vector>
#include <ostream>

class Template;


class Template {
    // (row, column)
    typedef std::pair<size_t, size_t> Point;

    struct Placeholder {
        Placeholder(const Point& b, const Point& e)
                : begin(b), end(e) {
            if (begin.first == end.first) {
                isRaw = true;
                snapshot.reserve(end.second - begin.second);
            } else if (begin.second == end.second) {
                isRaw = false;
                snapshot.reserve(end.first - begin.first);
            } else {
                throw std::runtime_error("Wrong placeholder");
            }

        }

        Point begin;
        Point end;
        bool isRaw;
        std::string snapshot;

    private:
        Placeholder() = delete;
    };

public:
    Template(Template&&) = default;
    virtual ~Template() = default;

    static Template read(const std::string& path);
    operator bool() const noexcept;

    std::string next();
    void place(const std::string&);
    void rollback();

    void log(const std::string& title, std::ostream&) const;

private:
    Template(const Template&) = delete;
    Template() = default;
    void shrink();

private:
    std::vector<Placeholder> empty_;
    std::vector<Placeholder> filled_;
    std::vector<std::string> data_;
};

