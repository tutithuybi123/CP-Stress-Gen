#pragma once

#include <array>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace cp_stress_gen::core {

class Random {
public:
    using result_type = std::uint64_t;

    static constexpr result_type min() noexcept { return 0; }
    static constexpr result_type max() noexcept { return std::numeric_limits<result_type>::max(); }

    constexpr explicit Random(result_type seed_value = default_seed_value) noexcept {
        seed(seed_value);
    }

    [[nodiscard]] static Random from_time() noexcept {
        const auto now = static_cast<result_type>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        return Random(now ^ 0x9E3779B97F4A7C15ull);
    }

    constexpr void seed(result_type seed_value) noexcept {
        SplitMix64 mixer(seed_value);
        state_[0] = mixer.next();
        state_[1] = mixer.next();
        state_[2] = mixer.next();
        state_[3] = mixer.next();
    }

    [[nodiscard]] constexpr result_type operator()() noexcept {
        return next_u64();
    }

    [[nodiscard]] constexpr result_type next_u64() noexcept {
        const result_type result = rotl(state_[1] * 5ull, 7) * 9ull;
        const result_type t = state_[1] << 17;

        state_[2] ^= state_[0];
        state_[3] ^= state_[1];
        state_[1] ^= state_[2];
        state_[0] ^= state_[3];

        state_[2] ^= t;
        state_[3] = rotl(state_[3], 45);

        return result;
    }

    [[nodiscard]] constexpr std::uint32_t next_u32() noexcept {
        return static_cast<std::uint32_t>(next_u64() >> 32);
    }

    template <std::integral Int>
    [[nodiscard]] constexpr Int integer(Int left, Int right) noexcept {
        if (right < left) {
            const Int tmp = left;
            left = right;
            right = tmp;
        }

        using Unsigned = std::make_unsigned_t<Int>;
        const Unsigned lo = static_cast<Unsigned>(left);
        const Unsigned width = static_cast<Unsigned>(static_cast<Unsigned>(right) - lo);

        if (width == std::numeric_limits<Unsigned>::max()) {
            return static_cast<Int>(next_unsigned<Unsigned>());
        }

        return static_cast<Int>(lo + bounded_unsigned<Unsigned>(width + Unsigned{1}));
    }

    template <std::floating_point Real = double>
    [[nodiscard]] constexpr Real real() noexcept {
        return static_cast<Real>((next_u64() >> 11) * inv_two_pow_53);
    }

    template <std::floating_point Real>
    [[nodiscard]] constexpr Real real(Real left, Real right) noexcept {
        if (right < left) {
            const Real tmp = left;
            left = right;
            right = tmp;
        }
        return left + (right - left) * real<Real>();
    }

    [[nodiscard]] constexpr bool boolean() noexcept {
        return (next_u64() & 1ull) != 0;
    }

    [[nodiscard]] constexpr bool boolean(const double probability_true) noexcept {
        if (probability_true <= 0.0) {
            return false;
        }
        if (probability_true >= 1.0) {
            return true;
        }
        return real<double>() < probability_true;
    }

    constexpr void jump() noexcept {
        jump_impl(jump_constants);
    }

    constexpr void long_jump() noexcept {
        jump_impl(long_jump_constants);
    }

private:
    struct SplitMix64 {
        result_type value;

        constexpr explicit SplitMix64(const result_type seed_value) noexcept : value(seed_value) {}

        [[nodiscard]] constexpr result_type next() noexcept {
            result_type z = (value += 0x9E3779B97F4A7C15ull);
            z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
            z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
            return z ^ (z >> 31);
        }
    };

    static constexpr result_type default_seed_value = 0x853C49E6748FEA9Bull;
    static constexpr double inv_two_pow_53 = 1.0 / 9007199254740992.0;
    static constexpr std::array<result_type, 4> jump_constants{
        0x180EC6D33CFD0ABAull,
        0xD5A61266F0C9392Cull,
        0xA9582618E03FC9AAull,
        0x39ABDC4529B1661Cull
    };
    static constexpr std::array<result_type, 4> long_jump_constants{
        0x76E15D3EFEFDCBBFull,
        0xC5004E441C522FB3ull,
        0x77710069854EE241ull,
        0x39109BB02ACBE635ull
    };

    std::array<result_type, 4> state_{};

    [[nodiscard]] static constexpr result_type rotl(const result_type x, const int k) noexcept {
        return (x << k) | (x >> (64 - k));
    }

    template <std::unsigned_integral Unsigned>
    [[nodiscard]] constexpr Unsigned next_unsigned() noexcept {
        if constexpr (sizeof(Unsigned) <= sizeof(std::uint32_t)) {
            return static_cast<Unsigned>(next_u32());
        } else {
            return static_cast<Unsigned>(next_u64());
        }
    }

    template <std::unsigned_integral Unsigned>
    [[nodiscard]] constexpr Unsigned bounded_unsigned(const Unsigned bound) noexcept {
        if (bound <= Unsigned{1}) {
            return Unsigned{0};
        }

        const Unsigned threshold = static_cast<Unsigned>(-bound) % bound;
        for (;;) {
            const Unsigned value = next_unsigned<Unsigned>();
            if (value >= threshold) {
                return static_cast<Unsigned>(value % bound);
            }
        }
    }

    constexpr void jump_impl(const std::array<result_type, 4>& constants) noexcept {
        result_type s0 = 0;
        result_type s1 = 0;
        result_type s2 = 0;
        result_type s3 = 0;

        for (const result_type constant : constants) {
            for (int bit = 0; bit < 64; ++bit) {
                if ((constant & (1ull << bit)) != 0) {
                    s0 ^= state_[0];
                    s1 ^= state_[1];
                    s2 ^= state_[2];
                    s3 ^= state_[3];
                }
                (void)next_u64();
            }
        }

        state_[0] = s0;
        state_[1] = s1;
        state_[2] = s2;
        state_[3] = s3;
    }
};

using Xoshiro256StarStar = Random;

} // namespace cp_stress_gen::core
