#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <cstddef>
#include <string>

namespace cp_stress_gen {

class String {
public:
    using size_type = std::size_t;

    explicit String(const size_type size) : size_(size) {}

    String& alphabet(const std::string& chars) {
        core::require(!chars.empty(), "String::alphabet requires a non-empty alphabet");
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
        alphabet_.clear();
        for (int ch = static_cast<unsigned char>(left); ch <= static_cast<unsigned char>(right); ++ch) {
            alphabet_.push_back(static_cast<char>(ch));
        }
        return *this;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] std::string build(core::Random& rng) const {
        core::require(!alphabet_.empty(), "String generator alphabet must not be empty");
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
    size_type size_;
    std::string alphabet_{"abcdefghijklmnopqrstuvwxyz"};
};

} // namespace cp_stress_gen

