#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace cp_stress_gen::core {

class Printer {
public:
    explicit Printer(std::ostream& output) : output_(&output) {}

    [[nodiscard]] Printer indexed(const int shift) const {
        Printer copy(*this);
        copy.index_shift_ += shift;
        return copy;
    }

    Printer& separator(const char value) {
        separator_ = std::string(1, value);
        return *this;
    }

    Printer& separator(std::string value) {
        separator_ = std::move(value);
        return *this;
    }

    template <typename... Args>
    Printer& line(const Args&... args) {
        print_args(args...);
        *output_ << '\n';
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
                *output_ << separator_;
            }
            print_indexed_value(value);
            first = false;
        }
        *output_ << '\n';
        return *this;
    }

    template <typename Matrix>
    Printer& matrix(const Matrix& rows) {
        for (const auto& row : rows) {
            vector(row);
        }
        return *this;
    }

    template <typename Grid>
    Printer& grid(const Grid& rows) {
        for (const auto& row : rows) {
            print_grid_row(row);
        }
        return *this;
    }

    template <typename Container>
    Printer& edges(const Container& edges) {
        for (const auto& edge : edges) {
            print_edge_node(edge.u);
            *output_ << separator_;
            print_edge_node(edge.v);
            if (edge.weighted) {
                *output_ << separator_ << edge.w;
            }
            *output_ << '\n';
        }
        return *this;
    }

    template <typename Container>
    Printer& with_n(const Container& values) {
        line(container_size(values));
        return vector(values);
    }

    template <typename Container>
    Printer& edges(const std::size_t n, const Container& values) {
        return edges(n, container_size(values), values);
    }

    template <typename Container>
    Printer& edges(const std::size_t n, const std::size_t m, const Container& values) {
        line(n, m);
        return edges(values);
    }

    template <typename Container>
    Printer& with_n_m(const std::size_t n, const std::size_t m, const Container& values) {
        return edges(n, m, values);
    }

    template <typename Callable>
    Printer& testcases(const std::size_t count, Callable callable) {
        line(count);
        for (std::size_t case_index = 1; case_index <= count; ++case_index) {
            callable(case_index, *this);
        }
        return *this;
    }

private:
    std::ostream* output_;
    std::string separator_{" "};
    int index_shift_{0};

    template <typename T>
    struct is_shiftable_integral {
        using value_type = typename std::decay<T>::type;
        static constexpr bool value =
            std::is_integral<value_type>::value &&
            !std::is_same<value_type, bool>::value &&
            !std::is_same<value_type, char>::value &&
            !std::is_same<value_type, signed char>::value &&
            !std::is_same<value_type, unsigned char>::value;
    };

    template <typename T>
    struct is_character {
        using value_type = typename std::decay<T>::type;
        static constexpr bool value =
            std::is_same<value_type, char>::value ||
            std::is_same<value_type, signed char>::value ||
            std::is_same<value_type, unsigned char>::value;
    };

    template <typename Container>
    static std::size_t container_size(const Container& values) {
        return static_cast<std::size_t>(values.size());
    }

    void print_args() {}

    template <typename First, typename... Rest>
    void print_args(const First& first, const Rest&... rest) {
        *output_ << first;
        print_rest(rest...);
    }

    void print_rest() {}

    template <typename First, typename... Rest>
    void print_rest(const First& first, const Rest&... rest) {
        *output_ << separator_ << first;
        print_rest(rest...);
    }

    template <typename T>
    typename std::enable_if<is_shiftable_integral<T>::value, void>::type print_indexed_value(const T& value) {
        *output_ << static_cast<long long>(value) + static_cast<long long>(index_shift_);
    }

    template <typename T>
    typename std::enable_if<!is_shiftable_integral<T>::value, void>::type print_indexed_value(const T& value) {
        *output_ << value;
    }

    template <typename T>
    void print_edge_node(const T& value) {
        print_indexed_value(value);
    }

    template <typename Row>
    void print_grid_row(const Row& row) {
        bool first = true;
        for (const auto& value : row) {
            print_grid_separator(value, first);
            print_indexed_value(value);
            first = false;
        }
        *output_ << '\n';
    }

    template <typename T>
    typename std::enable_if<is_character<T>::value, void>::type print_grid_separator(const T&, const bool) {}

    template <typename T>
    typename std::enable_if<!is_character<T>::value, void>::type print_grid_separator(const T&, const bool first) {
        if (!first) {
            *output_ << separator_;
        }
    }
};

} // namespace cp_stress_gen::core
