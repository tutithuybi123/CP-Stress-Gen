#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Printer out(std::cout);

    out.line("organ_pipe");
    out.vector(cp_stress_gen::anti::SortKiller::organ_pipe(9));

    out.line("alternating_high_low");
    out.vector(cp_stress_gen::anti::SortKiller::alternating_high_low(10));

    out.line("broom_tree");
    const auto broom = cp_stress_gen::anti::TreeKiller::broom(8, 4);
    out.line(8);
    out.edges(broom);

    out.line("dense_dag");
    const auto dag = cp_stress_gen::anti::GraphKiller::dense_dag(5);
    out.line(5, dag.size());
    out.edges(dag);

    out.line("kmp_prefix_pattern");
    out.line(cp_stress_gen::anti::StringKiller::kmp_prefix_pattern(12));
    return 0;
}
