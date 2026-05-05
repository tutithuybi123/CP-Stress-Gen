#include "cp_stress_gen.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

long long brute_count_inversions(const std::vector<long long>& values) {
    long long answer = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        for (std::size_t j = i + 1; j < values.size(); ++j) {
            if (values[i] > values[j]) {
                ++answer;
            }
        }
    }
    return answer;
}

long long optimized_count_inversions(std::vector<long long> values) {
    long long answer = 0;
    std::vector<long long> buffer(values.size());

    const auto solve = [&](const auto& self, const std::size_t left, const std::size_t right) -> void {
        if (right - left <= 1) {
            return;
        }
        const std::size_t mid = left + (right - left) / 2;
        self(self, left, mid);
        self(self, mid, right);

        std::size_t i = left;
        std::size_t j = mid;
        std::size_t k = left;
        while (i < mid || j < right) {
            if (j == right || (i < mid && values[i] <= values[j])) {
                buffer[k++] = values[i++];
            } else {
                answer += static_cast<long long>(mid - i);
                buffer[k++] = values[j++];
            }
        }
        std::copy(buffer.begin() + static_cast<std::ptrdiff_t>(left), buffer.begin() + static_cast<std::ptrdiff_t>(right), values.begin() + static_cast<std::ptrdiff_t>(left));
    };

    solve(solve, 0, values.size());
    return answer;
}

int main() {
    cp_stress_gen::core::Random rng(301);
    cp_stress_gen::core::Printer out(std::cout);

    for (int tc = 1; tc <= 200; ++tc) {
        const auto values = cp_stress_gen::Array(30).many_equal(7).shuffle().build(rng);
        const long long brute = brute_count_inversions(values);
        const long long optimized = optimized_count_inversions(values);
        if (brute != optimized) {
            out.line("mismatch", tc);
            out.line(values.size());
            out.vector(values);
            out.line("brute", brute);
            out.line("optimized", optimized);
            return 1;
        }
    }

    out.line("all tests matched");
    return 0;
}
