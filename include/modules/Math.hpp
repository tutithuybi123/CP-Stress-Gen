#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstdlib>
#include <vector>

namespace cp_stress_gen {

class Math {
public:
    [[nodiscard]] static long long gcd(long long a, long long b) noexcept {
        if (a < 0) {
            a = -a;
        }
        if (b < 0) {
            b = -b;
        }
        while (b != 0) {
            const long long t = a % b;
            a = b;
            b = t;
        }
        return a;
    }

    [[nodiscard]] static long long lcm(const long long a, const long long b) noexcept {
        if (a == 0 || b == 0) {
            return 0;
        }
        return std::llabs(a / gcd(a, b) * b);
    }

    [[nodiscard]] static std::vector<long long> divisors(const long long value) {
        const long long n = std::llabs(value);
        std::vector<long long> result;
        if (n == 0) {
            return result;
        }

        for (long long d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                result.push_back(d);
                if (d != n / d) {
                    result.push_back(n / d);
                }
            }
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    [[nodiscard]] static std::vector<int> primes_up_to(const int n) {
        if (n < 2) {
            return {};
        }

        std::vector<bool> is_prime(static_cast<std::size_t>(n + 1), true);
        is_prime[0] = false;
        is_prime[1] = false;
        for (int p = 2; p * p <= n; ++p) {
            if (!is_prime[static_cast<std::size_t>(p)]) {
                continue;
            }
            for (int x = p * p; x <= n; x += p) {
                is_prime[static_cast<std::size_t>(x)] = false;
            }
        }

        std::vector<int> result;
        for (int x = 2; x <= n; ++x) {
            if (is_prime[static_cast<std::size_t>(x)]) {
                result.push_back(x);
            }
        }
        return result;
    }

    [[nodiscard]] static int random_prime(const int left, const int right, core::Random& rng) {
        core::require_range(left, right, "Math::random_prime requires left <= right");
        const auto primes = primes_up_to(right);
        std::vector<int> candidates;
        for (const int prime : primes) {
            if (prime >= left) {
                candidates.push_back(prime);
            }
        }
        core::require(!candidates.empty(), "Math::random_prime found no prime in range");
        const std::size_t index = rng.integer<std::size_t>(0, candidates.size() - 1);
        return candidates[index];
    }
};

} // namespace cp_stress_gen

