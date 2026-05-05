#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <cstddef>
#include <utility>
#include <string>

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

    [[nodiscard]] size_type size() const noexcept {
        return size_;
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
        PrefixHeavy
    };

    size_type size_;
    Mode mode_{Mode::Random};
    std::string alphabet_{"abcdefghijklmnopqrstuvwxyz"};
    std::string pattern_;
    size_type changes_{0};
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
};

} // namespace cp_stress_gen
