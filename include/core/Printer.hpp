#pragma once

#include <ostream>

namespace cp_stress_gen::core {

class Printer {
public:
    explicit Printer(std::ostream& output) noexcept : output_(output) {}

    template <typename... Args>
    Printer& line(const Args&... args) {
        print_args(args...);
        output_ << '\n';
        return *this;
    }

    template <typename... Args>
    Printer& space(const Args&... args) {
        print_args(args...);
        return *this;
    }

    template <typename Container>
    Printer& vector(const Container& values) {
        bool first = true;
        for (const auto& value : values) {
            if (!first) {
                output_ << ' ';
            }
            output_ << value;
            first = false;
        }
        output_ << '\n';
        return *this;
    }

    template <typename Container>
    Printer& edges(const Container& edges) {
        for (const auto& edge : edges) {
            output_ << edge.u << ' ' << edge.v;
            if (edge.weighted) {
                output_ << ' ' << edge.w;
            }
            output_ << '\n';
        }
        return *this;
    }

private:
    std::ostream& output_;

    void print_args() {}

    template <typename First, typename... Rest>
    void print_args(const First& first, const Rest&... rest) {
        output_ << first;
        print_rest(rest...);
    }

    void print_rest() {}

    template <typename First, typename... Rest>
    void print_rest(const First& first, const Rest&... rest) {
        output_ << ' ' << first;
        print_rest(rest...);
    }
};

} // namespace cp_stress_gen::core

