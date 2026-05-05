#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Math {
public:
    struct ExtendedGcdResult {
        long long gcd;
        long long x;
        long long y;
    };

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

    [[nodiscard]] static bool is_prime(const long long n) noexcept {
        if (n < 2) {
            return false;
        }
        if (n % 2 == 0) {
            return n == 2;
        }
        for (long long d = 3; d * d <= n; d += 2) {
            if (n % d == 0) {
                return false;
            }
        }
        return true;
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

    [[nodiscard]] static long long random_composite(const long long left, const long long right, core::Random& rng) {
        core::require_range(left, right, "Math::random_composite requires left <= right");
        std::vector<long long> candidates;
        for (long long value = left; value <= right; ++value) {
            if (value > 1 && !is_prime(value)) {
                candidates.push_back(value);
            }
        }
        core::require(!candidates.empty(), "Math::random_composite found no composite in range");
        const std::size_t index = rng.integer<std::size_t>(0, candidates.size() - 1);
        return candidates[index];
    }

    [[nodiscard]] static std::pair<long long, long long> coprime_pair(
        const long long left,
        const long long right,
        core::Random& rng
    ) {
        core::require_range(left, right, "Math::coprime_pair requires left <= right");
        for (int attempt = 0; attempt < 10000; ++attempt) {
            const long long a = rng.integer<long long>(left, right);
            const long long b = rng.integer<long long>(left, right);
            if (gcd(a, b) == 1) {
                return std::make_pair(a, b);
            }
        }
        for (long long a = left; a <= right; ++a) {
            for (long long b = left; b <= right; ++b) {
                if (gcd(a, b) == 1) {
                    return std::make_pair(a, b);
                }
            }
        }
        throw std::invalid_argument("Math::coprime_pair found no coprime pair in range");
    }

    [[nodiscard]] static std::pair<long long, long long> with_gcd(
        const long long g,
        const long long multiplier_left,
        const long long multiplier_right,
        core::Random& rng
    ) {
        core::require(g > 0, "Math::with_gcd requires g > 0");
        core::require(multiplier_left > 0, "Math::with_gcd requires positive multipliers");
        const auto multipliers = coprime_pair(multiplier_left, multiplier_right, rng);
        return std::make_pair(g * multipliers.first, g * multipliers.second);
    }

    [[nodiscard]] static long long mod_pow(long long base, long long exponent, const long long mod) {
        core::require(mod > 0, "Math::mod_pow requires mod > 0");
        core::require(exponent >= 0, "Math::mod_pow requires exponent >= 0");
        base = normalize_mod(base, mod);

        long long result = 1 % mod;
        while (exponent > 0) {
            if ((exponent & 1LL) != 0) {
                result = mul_mod(result, base, mod);
            }
            base = mul_mod(base, base, mod);
            exponent >>= 1;
        }
        return result;
    }

    [[nodiscard]] static ExtendedGcdResult extended_gcd(long long a, long long b) noexcept {
        const long long sign_a = a < 0 ? -1 : 1;
        const long long sign_b = b < 0 ? -1 : 1;
        a = std::llabs(a);
        b = std::llabs(b);

        long long old_r = a;
        long long r = b;
        long long old_s = 1;
        long long s = 0;
        long long old_t = 0;
        long long t = 1;

        while (r != 0) {
            const long long q = old_r / r;

            const long long next_r = old_r - q * r;
            old_r = r;
            r = next_r;

            const long long next_s = old_s - q * s;
            old_s = s;
            s = next_s;

            const long long next_t = old_t - q * t;
            old_t = t;
            t = next_t;
        }

        return ExtendedGcdResult{old_r, old_s * sign_a, old_t * sign_b};
    }

    [[nodiscard]] static long long mod_inverse(const long long value, const long long mod) {
        core::require(mod > 0, "Math::mod_inverse requires mod > 0");
        const ExtendedGcdResult result = extended_gcd(value, mod);
        core::require(result.gcd == 1, "Math::mod_inverse requires coprime value and mod");
        return normalize_mod(result.x, mod);
    }

    [[nodiscard]] static std::vector<std::pair<long long, int>> factorize_trial(long long value) {
        value = std::llabs(value);
        std::vector<std::pair<long long, int>> result;
        if (value < 2) {
            return result;
        }

        for (long long p = 2; p * p <= value; p += (p == 2 ? 1 : 2)) {
            if (value % p != 0) {
                continue;
            }
            int count = 0;
            while (value % p == 0) {
                value /= p;
                ++count;
            }
            result.push_back(std::make_pair(p, count));
        }
        if (value > 1) {
            result.push_back(std::make_pair(value, 1));
        }
        return result;
    }

private:
    [[nodiscard]] static long long normalize_mod(long long value, const long long mod) noexcept {
        value %= mod;
        if (value < 0) {
            value += mod;
        }
        return value;
    }

    [[nodiscard]] static long long add_mod(const long long a, const long long b, const long long mod) noexcept {
        if (a >= mod - b) {
            return a - (mod - b);
        }
        return a + b;
    }

    [[nodiscard]] static long long mul_mod(long long a, long long b, const long long mod) noexcept {
        long long result = 0;
        while (b > 0) {
            if ((b & 1LL) != 0) {
                result = add_mod(result, a, mod);
            }
            b >>= 1;
            if (b > 0) {
                a = add_mod(a, a, mod);
            }
        }
        return result;
    }
};

} // namespace cp_stress_gen
