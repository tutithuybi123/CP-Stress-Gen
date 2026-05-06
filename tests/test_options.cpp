#include "cp_stress_gen.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

cp_stress_gen::core::Options make_options(const int argc, const char* const argv[]) {
    return cp_stress_gen::core::Options(argc, const_cast<char**>(argv));
}

} // namespace

int main() {
    {
        const char* argv[] = {"gen", "--n", "10", "--seed=42", "--mode", "hard", "--hard"};
        auto opt = make_options(7, argv);
        assert(opt.has("n"));
        assert(opt.has("--seed"));
        assert(opt.get<int>("n", 1) == 10);
        assert(opt.get<long long>("seed", 0) == 42);
        assert(opt.get<std::string>("mode", "random") == "hard");
        assert(opt.flag("hard"));
        assert(opt.get<int>("missing", 123) == 123);
    }

    {
        const char* argv[] = {"gen", "-h"};
        auto opt = make_options(2, argv);
        assert(opt.flag("help"));
        assert(opt.flag("-h"));
    }

    {
        const char* argv[] = {"gen", "--help"};
        auto opt = make_options(2, argv);
        assert(opt.flag("--help"));
    }

    {
        const char* argv[] = {"gen", "--enabled=false", "--ratio=0.25"};
        auto opt = make_options(3, argv);
        assert(!opt.flag("enabled", true));
        assert(opt.get<double>("ratio", 1.0) > 0.24);
        assert(opt.get<double>("ratio", 1.0) < 0.26);
    }

    {
        const char* argv[] = {"gen", "--n", "10", "--hard"};
        auto opt = make_options(4, argv);
        opt.require_no_unknown({"n", "hard"});
    }

    bool thrown = false;
    try {
        const char* argv[] = {"gen"};
        auto opt = make_options(1, argv);
        (void)opt.required<int>("n");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        const char* argv[] = {"gen", "--n", "abc"};
        auto opt = make_options(3, argv);
        (void)opt.get<int>("n", 1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        const char* argv[] = {"gen", "--n", "10", "--n=20"};
        (void)make_options(4, argv);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        const char* argv[] = {"gen", "--unknown"};
        auto opt = make_options(2, argv);
        opt.require_no_unknown({"n"});
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        const char* argv[] = {"gen", "--mode"};
        auto opt = make_options(2, argv);
        (void)opt.required<std::string>("mode");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_options\n";
    return 0;
}
