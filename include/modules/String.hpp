#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class String {
public:
    using size_type = std::size_t;

    explicit String(const size_type size) : size_(size) {}

    String& alphabet(const std::string& chars) {
        core::require(!chars.empty(), "String::alphabet requires a non-empty alphabet");
        mode_ = Mode::Random;
        alphabet_ = chars;
        return *this;
    }

    String& lowercase() {
        return alphabet("abcdefghijklmnopqrstuvwxyz");
    }

    String& uppercase() {
        return alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

    String& digits() {
        return alphabet("0123456789");
    }

    String& binary() {
        return alphabet("01");
    }

    String& range(const char left, const char right) {
        core::require(left <= right, "String::range requires left <= right");
        mode_ = Mode::Random;
        alphabet_.clear();
        for (int ch = static_cast<unsigned char>(left); ch <= static_cast<unsigned char>(right); ++ch) {
            alphabet_.push_back(static_cast<char>(ch));
        }
        return *this;
    }

    String& palindrome() noexcept {
        mode_ = Mode::Palindrome;
        return *this;
    }

    String& almost_palindrome(const size_type changes) {
        core::require(changes <= size_ / 2, "String::almost_palindrome changes must be <= n / 2");
        mode_ = Mode::AlmostPalindrome;
        changes_ = changes;
        return *this;
    }

    String& periodic(const std::string& pattern) {
        core::require(!pattern.empty(), "String::periodic requires a non-empty pattern");
        mode_ = Mode::Periodic;
        pattern_ = pattern;
        return *this;
    }

    String& prefix_heavy(const char prefix_char = 'a', const char tail_char = 'b') noexcept {
        mode_ = Mode::PrefixHeavy;
        prefix_char_ = prefix_char;
        tail_char_ = tail_char;
        return *this;
    }

    String& border_heavy() noexcept {
        mode_ = Mode::BorderHeavy;
        return *this;
    }

    String& runs(const size_type run_length) {
        core::require_positive(run_length, "String::runs run length must be positive");
        mode_ = Mode::Runs;
        run_length_ = run_length;
        return *this;
    }

    String& periodic_with_noise(const std::string& pattern, const size_type changes) {
        core::require(!pattern.empty(), "String::periodic_with_noise requires a non-empty pattern");
        core::require(changes <= size_, "String::periodic_with_noise changes must be <= n");
        mode_ = Mode::PeriodicWithNoise;
        pattern_ = pattern;
        changes_ = changes;
        return *this;
    }

    String& random_palindromic_blocks(const size_type block_size) {
        core::require_positive(block_size, "String::random_palindromic_blocks block size must be positive");
        mode_ = Mode::PalindromicBlocks;
        block_size_ = block_size;
        return *this;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] static std::string de_bruijn_binary(const size_type k) {
        core::require_positive(k, "String::de_bruijn_binary k must be positive");
        core::require(k <= 20, "String::de_bruijn_binary k is too large for this helper");

        std::vector<int> a(2 * k + 1, 0);
        std::string sequence;
        de_bruijn_dfs(1, 1, k, a, sequence);
        sequence.append(k - 1, '0');
        return sequence;
    }

    [[nodiscard]] std::string build(core::Random& rng) const {
        core::require(!alphabet_.empty(), "String generator alphabet must not be empty");
        if (mode_ == Mode::Palindrome || mode_ == Mode::AlmostPalindrome) {
            return build_palindrome(rng, mode_ == Mode::AlmostPalindrome);
        }
        if (mode_ == Mode::Periodic) {
            return build_periodic();
        }
        if (mode_ == Mode::PrefixHeavy) {
            return build_prefix_heavy();
        }
        if (mode_ == Mode::BorderHeavy) {
            return std::string(size_, alphabet_.front());
        }
        if (mode_ == Mode::Runs) {
            return build_runs();
        }
        if (mode_ == Mode::PeriodicWithNoise) {
            return build_periodic_with_noise(rng);
        }
        if (mode_ == Mode::PalindromicBlocks) {
            return build_palindromic_blocks(rng);
        }

        std::string result;
        result.reserve(size_);
        for (size_type i = 0; i < size_; ++i) {
            const size_type index = rng.integer<size_type>(0, alphabet_.size() - 1);
            result.push_back(alphabet_[index]);
        }
        return result;
    }

    [[nodiscard]] std::string build() const {
        core::Random rng = core::Random::from_time();
        return build(rng);
    }

private:
    enum class Mode {
        Random,
        Palindrome,
        AlmostPalindrome,
        Periodic,
        PrefixHeavy,
        BorderHeavy,
        Runs,
        PeriodicWithNoise,
        PalindromicBlocks
    };

    size_type size_;
    Mode mode_{Mode::Random};
    std::string alphabet_{"abcdefghijklmnopqrstuvwxyz"};
    std::string pattern_;
    size_type changes_{0};
    size_type run_length_{1};
    size_type block_size_{1};
    char prefix_char_{'a'};
    char tail_char_{'b'};

    [[nodiscard]] char random_char(core::Random& rng) const {
        const size_type index = rng.integer<size_type>(0, alphabet_.size() - 1);
        return alphabet_[index];
    }

    [[nodiscard]] std::string build_palindrome(core::Random& rng, const bool mutate) const {
        std::string result(size_, '\0');
        for (size_type i = 0; i < (size_ + 1) / 2; ++i) {
            const char ch = random_char(rng);
            result[i] = ch;
            result[size_ - 1 - i] = ch;
        }

        if (mutate) {
            for (size_type i = 0; i < changes_; ++i) {
                const size_type pos = i;
                char ch = random_char(rng);
                if (alphabet_.size() > 1) {
                    while (ch == result[size_ - 1 - pos]) {
                        ch = random_char(rng);
                    }
                }
                result[pos] = ch;
            }
        }
        return result;
    }

    [[nodiscard]] std::string build_periodic() const {
        std::string result;
        result.reserve(size_);
        for (size_type i = 0; i < size_; ++i) {
            result.push_back(pattern_[i % pattern_.size()]);
        }
        return result;
    }

    [[nodiscard]] std::string build_prefix_heavy() const {
        if (size_ == 0) {
            return {};
        }
        std::string result(size_, prefix_char_);
        result[size_ - 1] = tail_char_;
        return result;
    }

    [[nodiscard]] std::string build_runs() const {
        std::string result;
        result.reserve(size_);
        for (size_type i = 0; i < size_; ++i) {
            const size_type run_index = (i / run_length_) % alphabet_.size();
            result.push_back(alphabet_[run_index]);
        }
        return result;
    }

    [[nodiscard]] std::string build_periodic_with_noise(core::Random& rng) const {
        std::string result = build_periodic();
        for (size_type i = 0; i < changes_ && !result.empty(); ++i) {
            const size_type pos = i * size_ / changes_;
            char ch = random_char(rng);
            if (alphabet_.size() > 1) {
                while (ch == result[pos]) {
                    ch = random_char(rng);
                }
            }
            result[pos] = ch;
        }
        return result;
    }

    [[nodiscard]] std::string build_palindromic_blocks(core::Random& rng) const {
        std::string result(size_, '\0');
        for (size_type begin = 0; begin < size_; begin += block_size_) {
            const size_type end = std::min(size_, begin + block_size_);
            for (size_type left = begin, right = end - 1; left <= right; ++left, --right) {
                const char ch = random_char(rng);
                result[left] = ch;
                result[right] = ch;
                if (right == 0) {
                    break;
                }
            }
        }
        return result;
    }

    static void de_bruijn_dfs(
        const size_type t,
        const size_type p,
        const size_type k,
        std::vector<int>& a,
        std::string& sequence
    ) {
        if (t > k) {
            if (k % p == 0) {
                for (size_type i = 1; i <= p; ++i) {
                    sequence.push_back(static_cast<char>('0' + a[i]));
                }
            }
            return;
        }

        a[t] = a[t - p];
        de_bruijn_dfs(t + 1, p, k, a, sequence);
        for (int value = a[t - p] + 1; value < 2; ++value) {
            a[t] = value;
            de_bruijn_dfs(t + 1, t, k, a, sequence);
        }
    }
};

class StringHash {
public:
    [[nodiscard]] static long long hash_string(const std::string& value, const long long base, const long long mod) {
        core::require(base > 0, "StringHash::hash_string base must be positive");
        core::require(mod > 1, "StringHash::hash_string mod must be greater than 1");

        long long result = 0;
        const long long normalized_base = normalize(base, mod);
        for (const unsigned char ch : value) {
            result = add_mod(mul_mod(result, normalized_base, mod), normalize(static_cast<long long>(ch), mod), mod);
        }
        return result;
    }

    [[nodiscard]] static bool same_hash(
        const std::string& left,
        const std::string& right,
        const long long base,
        const long long mod
    ) {
        return hash_string(left, base, mod) == hash_string(right, base, mod);
    }

    [[nodiscard]] static std::pair<std::string, std::string> collision_like_pair(const std::size_t n) {
        core::require(n > 0, "StringHash::collision_like_pair length must be positive");
        std::string first(n, 'a');
        std::string second(n, 'a');
        first[n - 1] = 'b';
        second[0] = 'b';
        return std::make_pair(first, second);
    }

private:
    [[nodiscard]] static long long normalize(long long value, const long long mod) noexcept {
        value %= mod;
        if (value < 0) {
            value += mod;
        }
        return value;
    }

    [[nodiscard]] static long long add_mod(const long long left, const long long right, const long long mod) noexcept {
        if (left >= mod - right) {
            return left - (mod - right);
        }
        return left + right;
    }

    [[nodiscard]] static long long mul_mod(long long left, long long right, const long long mod) noexcept {
        long long result = 0;
        left = normalize(left, mod);
        right = normalize(right, mod);
        while (right > 0) {
            if (right & 1ll) {
                result = add_mod(result, left, mod);
            }
            right >>= 1ll;
            if (right > 0) {
                left = add_mod(left, left, mod);
            }
        }
        return result;
    }
};

} // namespace cp_stress_gen
