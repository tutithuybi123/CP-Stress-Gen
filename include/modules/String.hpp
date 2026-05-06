#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <set>
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

    [[nodiscard]] static std::string thue_morse(
        const size_type n,
        const char a = 'a',
        const char b = 'b'
    ) {
        std::string result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(popcount(i) % 2 == 0 ? a : b);
        }
        return result;
    }

    [[nodiscard]] static std::pair<std::string, std::string> thue_morse_pair(
        const size_type k,
        const char a = 'a',
        const char b = 'b'
    ) {
        core::require(k <= 20, "String::thue_morse_pair k is too large for this helper");
        const size_type n = size_type{1} << k;
        std::string first = thue_morse(n, a, b);
        std::string second = thue_morse(n, b, a);
        return std::make_pair(first, second);
    }

    [[nodiscard]] static std::string fibonacci_word(
        const size_type n,
        const char a = 'a',
        const char b = 'b'
    ) {
        if (n == 0) {
            return {};
        }
        std::string previous(1, b);
        std::string current(1, a);
        while (current.size() < n) {
            std::string next = current + previous;
            previous = current;
            current = next;
        }
        current.resize(n);
        return current;
    }

    [[nodiscard]] static std::string de_bruijn(const std::string& alphabet, const size_type k) {
        core::require(!alphabet.empty(), "String::de_bruijn alphabet must not be empty");
        core::require_positive(k, "String::de_bruijn k must be positive");
        core::require(alphabet.size() <= 8, "String::de_bruijn alphabet is too large for this helper");

        std::set<char> unique(alphabet.begin(), alphabet.end());
        core::require(unique.size() == alphabet.size(), "String::de_bruijn alphabet characters must be unique");

        size_type sequence_length = 1;
        for (size_type i = 0; i < k; ++i) {
            core::require(sequence_length <= 1000000 / alphabet.size(), "String::de_bruijn output would be too large");
            sequence_length *= alphabet.size();
        }

        std::vector<int> a(alphabet.size() * k + 1, 0);
        std::string sequence;
        de_bruijn_dfs_general(1, 1, k, alphabet, a, sequence);
        sequence.append(k - 1, alphabet.front());
        return sequence;
    }

    [[nodiscard]] static std::string border_chain(const size_type n, const char ch = 'a') {
        return std::string(n, ch);
    }

    [[nodiscard]] static std::string periodic_blocks(const std::string& pattern, const size_type blocks) {
        core::require(!pattern.empty(), "String::periodic_blocks pattern must not be empty");
        std::string result;
        result.reserve(pattern.size() * blocks);
        for (size_type i = 0; i < blocks; ++i) {
            result += pattern;
        }
        return result;
    }

    [[nodiscard]] static std::string runs(const std::vector<std::pair<char, size_type>>& lengths) {
        std::string result;
        for (const auto& part : lengths) {
            result.append(part.second, part.first);
        }
        return result;
    }

    [[nodiscard]] static std::string kmp_worst_prefix(const size_type n, const char repeated = 'a', const char breaker = 'b') {
        if (n == 0) {
            return {};
        }
        std::string result(n, repeated);
        result[n - 1] = breaker;
        return result;
    }

    [[nodiscard]] static std::string anti_z(const size_type n, const char ch = 'a') {
        return std::string(n, ch);
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

    [[nodiscard]] static int popcount(size_type value) noexcept {
        int result = 0;
        while (value > 0) {
            result += static_cast<int>(value & 1u);
            value >>= 1u;
        }
        return result;
    }

    static void de_bruijn_dfs_general(
        const size_type t,
        const size_type p,
        const size_type k,
        const std::string& alphabet,
        std::vector<int>& a,
        std::string& sequence
    ) {
        if (t > k) {
            if (k % p == 0) {
                for (size_type i = 1; i <= p; ++i) {
                    sequence.push_back(alphabet[static_cast<size_type>(a[i])]);
                }
            }
            return;
        }

        a[t] = a[t - p];
        de_bruijn_dfs_general(t + 1, p, k, alphabet, a, sequence);
        for (int value = a[t - p] + 1; value < static_cast<int>(alphabet.size()); ++value) {
            a[t] = value;
            de_bruijn_dfs_general(t + 1, t, k, alphabet, a, sequence);
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

    [[nodiscard]] static unsigned long long hash_u64(const std::string& value, const unsigned long long base) noexcept {
        unsigned long long result = 0;
        for (const unsigned char ch : value) {
            result = result * base + static_cast<unsigned long long>(ch);
        }
        return result;
    }

    [[nodiscard]] static bool same_hash_u64(
        const std::string& left,
        const std::string& right,
        const unsigned long long base
    ) noexcept {
        return hash_u64(left, base) == hash_u64(right, base);
    }

    [[nodiscard]] static std::pair<std::string, std::string> thue_morse_pair_power2(
        const std::size_t k,
        const char a = 'a',
        const char b = 'b'
    ) {
        core::require(k >= 10, "StringHash::thue_morse_pair_power2 requires k >= 10 for 64-bit overflow hashes");
        return String::thue_morse_pair(k, a, b);
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
