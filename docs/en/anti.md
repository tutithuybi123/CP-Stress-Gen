# Safe Anti Patterns

The `cp_stress_gen::anti` namespace contains simple pattern helpers. These names describe shapes; they do not guarantee that a specific algorithm will fail.

## Sort Patterns

- `SortKiller::reversed(n, first)`
- `SortKiller::nearly_sorted(n, swaps, rng)`
- `SortKiller::many_duplicates(n, distinct, rng)`

## Tree Patterns

- `TreeKiller::chain(n)`
- `TreeKiller::star(n)`
- `TreeKiller::chain_with_branches(n, branches, rng)`

## Graph Patterns

- `GraphKiller::dense(n, rng)`
- `GraphKiller::long_dag_chain(n)`
- `GraphKiller::sparse_connected(n, rng)`

## String Patterns

- `StringKiller::repeated(n, ch)`
- `StringKiller::alternating(n, a, b)`
- `StringKiller::prefix_heavy(count, length)`

Hash-collision generators, Dijkstra-specific patterns, and other risky adversarial modules are not implemented.
